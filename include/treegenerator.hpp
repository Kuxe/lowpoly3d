#ifndef TREEGENERATOR_HPP
#define TREEGENERATOR_HPP

#include "lsys.hpp"
#include <type_traits> //std::remove_const
#include "cylindergenerator.hpp"
#include <utility> //std::pair
#include <stack>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL //glm::to_string
#include <glm/gtx/string_cast.hpp> //glm::to_string

namespace lowpoly3d {

using namespace lsys;

template<typename T>
struct Node {
	T value;
	Node* right, left;

	Node(const T& value = T()) : value(value) { }
	~Node() {
		delete right;
		delete left;
	}

	void addLeft(const T& t) {
		left = new Node(value);
	}
	void addRight(const T& t) {
		right = new Node(value);
	}

	void removeLeft() {
		delete left;
	}

	void removeRight() {
		delete right;
	}
};

/** Given a function f: TxT -> U, produce a tree with the same structure as
	source but with a root with default (trash) value and children values
	being f(parent, current) **/
template<typename T, typename U>
void children_diff(const Node<T>& source, Node<U>* dest, const std::function<U(const T&, const T&)>& f) {
	if(source.right) {
		dest->addRight(new Node<U>(f(source.val, source.right.val)));
		children_diff(source.right, dest->right, f);
	}
	if(source.left) {
		dest->addLeft(new Node<U>(f(source.val, source.left.val)));
		children_diff(source.left, dest.left, f);
	}
}

/** Generates trees **/
class TreeGenerator : public ModelGenerator {
private:

	/** A <=> Drop vertex
		B <=> Drop vertex
		F <=> Forward (parametrized by one variable, length) **/
	Axiom axiom = {{'A', {}}, {'F', {10.0f}}};
	lsys::Lsystem lsys = {axiom, {
		{'F', "F[+AF][-AF]", [](Params p) -> Params { return {p[0], p[0]/2.0f, p[0]/2.0f}; }, 1.0f},
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

		std::stack<State> stack;
		State state;

		/** Walk the turtle along word and make it drop vertices.
			Every pair of consecutive vertices form a line which determines the orientation
			and length of a cylinder. The union of all cylinders forms the returned model **/
		lsys.next(1);
		std::cout << lsys.str() << std::endl;
		std::vector<glm::vec3> points;
		for(const Module& module : lsys.read()) {
			switch(module.symbol) {
				case 'A': {
					points.push_back({glm::column(state.m, 3)});
				} break;
				case 'F': {
					state.m = glm::translate(state.m, -module.params[0]*glm::vec3(glm::column(state.m, 2)));
				} break;
				case '+': {
					glm::vec3 xaxis = glm::column(state.m, 0);
					state.m = glm::rotate(state.m, 3.14f/4.0f, xaxis);
				} break;
				case '-': {
					glm::vec3 xaxis = glm::column(state.m, 0);
					state.m = glm::rotate(state.m, -3.14f/4.0f, xaxis);
				} break;
				case '[': {
					stack.push(state);
				} break;
				case ']': {
					if(stack.size() == 0) {
						std::cout << "ERROR: Turtle tried to pop stack, but nothing to pop! The word must be ill-defined with more ']' than '['" << std::endl;
					}
					state = stack.top();
					stack.pop();
				} break;
			}
		}

		using Line = std::pair<glm::vec3, glm::vec3>;
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

		//Make copies from basic cylinder who are aligned by lines
		CylinderGenerator cg({150, 150, 150}, 16);
		const Model cylinder = cg.generate();
		Model output;
		for(size_t i = 0; i < points.size(); i+=2) {
			std::cout << "Line: " << glm::to_string(points[i]) << ", " << glm::to_string(points[i+1]) << std::endl;
			Model cpy(cylinder);
			output.append(transform(cpy, line2mat4({points[i], points[i+1]})));
		}
		return output;
	}
};

} //End of namespace lowpoly3d

#endif //TREEGENERATOR_HPP