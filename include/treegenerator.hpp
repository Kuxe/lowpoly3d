#ifndef TREEGENERATOR_HPP
#define TREEGENERATOR_HPP

#include "lsys.hpp"
#include <type_traits> //std::remove_const
#include "cylindergenerator.hpp"
#include <utility> //std::pair
#include <stack>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL //glm::to_string
#include <glm/gtx/transform.hpp> //glm::rotate
#include <glm/gtx/string_cast.hpp> //glm::to_string

namespace lowpoly3d {

using namespace lsys;

template<typename T>
struct Node {
	T value;
	Node<T>* neighbour = nullptr;
	std::vector<Node> children;

	Node(const T& value = T()) : value(value) {}
	~Node() { 
		delete neighbour;
	}

	const T& get() const {
		return value;
	}

	/** Returns the youngest sibling **/
	Node<T>* back() {
		return &children.back();
	}

	Node<T>* addNeighbour(const T& t = T()) {
		neighbour = new Node<T>(t);
		return neighbour;
	}

	Node<T>* addChild(const T& t = T()) {
		children.push_back({t});
		return back();
	}

	/** Apply some function to each element of this tree **/
	template<typename Fcn>
	void for_each(Fcn f) const {
		f(value);
		for(const Node<T>& node : children) {
			node.for_each(f);
		}
		if(neighbour) {
			neighbour->for_each(f);
		}
	}

	//Converts tree to std::vector
	void convert(std::vector<T>& v) const {
		for_each([&v](const T& value) { v.push_back(value); });
	}
};

/** Given a function f: TxT -> U, produce a tree with the same structure as
	source but with a root with default (trash) value and children values
	being f(parent, current) **/
template<typename T, typename U>
void pair_transform(const Node<T>& source, Node<U>* dest, const std::function<U(const T&, const T&)>& f) {
	for(const Node<T>& node : source.children) {
		pair_transform(node, dest->addChild(f(source.value, node.value)), f);
	}
	if(source.neighbour) {
		pair_transform(*source.neighbour, dest->addNeighbour(f(source.value, source.neighbour->value)), f);
	}
}

/** Generates trees **/
class TreeGenerator : public ModelGenerator {
private:

	/** A <=> Drop vertex
		B <=> Drop vertex
		F <=> Forward (parametrized by one variable, length) **/
	Axiom axiom = {{'A', {}}, {'F', {100.0f}}};
	lsys::Lsystem lsys = {axiom, {
		{'F', "F[+FA][-FA]", [](Params p) -> Params { return {p[0], 0.95f*p[0], 0.95f*p[0]}; }, 1.0f},
		{'A', "A", [](Params p) -> Params { return {}; }, 1.0f},
		{'+', "+", [](Params p) -> Params { return {}; }, 1.0f},
		{'-', "-", [](Params p) -> Params { return {}; }, 1.0f}
	}, 'F', 1};

public:

	Model generate() {
		//TODO: Many obvious simple optimizations (dont push_back since size of vectors are known for example!)


		//Turtle state
		struct State {
			glm::mat4 m = glm::mat4(1.0f); //Transform of turtle
		};

		/** TODO: Can not use vector of glm::vec3 and simply push on encountered A
			because A[B][C] should create two lines from AB and AC not AB and BC!
			Should use some tree structure to store points and then do some diff_children
			to generate a new tree with lines **/

		/** Walk the turtle along word and make it drop vertices.
			Every pair of consecutive vertices form a line which determines the orientation
			and length of a cylinder. The union of all cylinders forms the returned model **/
		lsys.next(2);

		std::stack<State> state_stack;
		std::stack<Node<glm::vec3>*> node_stack;
		State state;
		Node<glm::vec3> root;
		Node<glm::vec3>* node = &root;	
		using Line = std::pair<glm::vec3, glm::vec3>;
		bool unset = true; //If node has no value set this is true
		for(size_t i = 1; i < lsys.read().size(); i++) {
			const Module& module = lsys.read()[i];
			switch(module.symbol) {
				case 'A': {
					/** node is guaranteed to exist here but it may not have a value,
						if not then set value at node.
						Otherwise, we must add a neighbour-related node and set the value there **/
					if(unset) {
						node->value = glm::column(state.m, 3);
					} else {
						node = node->addNeighbour(glm::column(state.m, 3));
					}
					unset = false;

				} break;
				case 'F': {
					state.m = glm::translate(state.m, -module.params[0]*glm::vec3(glm::column(state.m, 2)));
				} break;
				case '+': {
					state.m = glm::rotate(3.14f/8.0f, glm::normalize(glm::vec3(glm::column(state.m, 0)))) * state.m;
				} break;
				case '-': {
					state.m = glm::rotate(-3.14f/8.0f, glm::normalize(glm::vec3(glm::column(state.m, 0)))) * state.m;
				} break;
				case '[': {
					state_stack.push(state);
					node_stack.push(node);

					/** Since '[' encountered we must add a child-related node.
						But we do not know the value which it should have just yet,
						since we must proceed to the next module in order to know that,
						so it is unset **/
					node = node->addChild();
					unset = true;
				} break;
				case ']': {
					if(state_stack.size() == 0) {
						std::cout << "ERROR: Turtle tried to pop stack, but nothing to pop! The word must be ill-defined with more ']' than '['" << std::endl;
					}
					state = state_stack.top();
					state_stack.pop();
					node = node_stack.top();
					node_stack.pop();
				} break;
			}
		}

		/** Now we have a tree of points produced by turtle.
			Create the set of lines induced by this tree. **/
		Node<Line> linetree;
		pair_transform<glm::vec3, Line>(root, &linetree, [](const Node<glm::vec3>& node1, const Node<glm::vec3>& node2) -> Line {
			return {node1.value, node2.value};
		});

		//Turn the tree into std::vector
		std::vector<Line> lines;
		linetree.convert(lines);

		/** Produces matrix which aligns vectors in direction of given line
			Note that this is not well-defined for geometries which are not
			symmetric about the y-axis since the geometry after the transform
			might be rotated in arbitrary manner about the vector induced
			by the line **/
		const auto line2mat4 = [](const Line& line) -> glm::mat4 {
			const glm::vec3 v2 = glm::normalize(line.second - line.first);
			const glm::vec3 v1 = glm::cross({0.0f, 1.0f, 0.0f}, v2);
			const glm::vec3 v3 = glm::cross(v1, v2);
			return {{v1, 0.0f}, {v2, 0.0f}, {v3, 0.0f}, {line.first, 1.0f}};
		};

		/** Make copies from basic cylinder. Align copies to lines and then
			append the copies to the output model **/
		CylinderGenerator cg({150, 150, 150}, 16);
		const Model cylinder = cg.generate();
		Model output;
		for(const Line& line : lines) {
			if(line.first != line.second) {
				Model cpy(cylinder);
				output.append(transform(cpy, line2mat4(line)));
			}
		}

		//FIXME: It doesn't seem that the cylinders are translated as they should. Why?

		return output;
	}
};

} //End of namespace lowpoly3d

#endif //TREEGENERATOR_HPP