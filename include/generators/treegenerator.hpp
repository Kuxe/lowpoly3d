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
#include "node.hpp"

namespace lowpoly3d {

using namespace lsys;

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
		//TODO: Before implementing turtle, should have debugging support in lowpoly3d for drawing lines and such


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
		State state;
		using Line = std::pair<glm::vec3, glm::vec3>;
		for(size_t i = 1; i < lsys.read().size(); i++) {
			const Module& module = lsys.read()[i];
			switch(module.symbol) {
				case 'A': {
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
				} break;
				case ']': {
					assert(
						state_stack.size() > 0 &&
						"ERROR: Turtle tried to pop stack, but nothing to pop! \
						The word must be ill-defined with more ']' than '['");
					state = state_stack.top();
					state_stack.pop();
				} break;
			}
		}

		//Turn the tree into std::vector
		std::vector<Line> lines;

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
		//ANS: Because the L-system produces 

		return output;
	}
};

} //End of namespace lowpoly3d

#endif //TREEGENERATOR_HPP