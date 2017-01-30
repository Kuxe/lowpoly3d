#ifndef WORLDUNIFORMDATA_HPP
#define WORLDUNIFORMDATA_HPP
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace lowpoly3d {

struct WorldUniformData {
	const glm::mat4 view, projection;
	const glm::vec4 sunPos, timeOfDayColor;
	const glm::vec2 windowResolution;
};

}

#endif //WORLDUNIFORMDATA_HPP