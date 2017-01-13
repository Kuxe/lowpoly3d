#ifndef WORLDUNIFORMDATA_HPP
#define WORLDUNIFORMDATA_HPP
#include <glm/glm.hpp>

struct WorldUniformData {
	const glm::mat4 view, projection;
	const glm::vec3 sunPos, timeOfDayColor;
	const glm::vec2 windowResolution;
};

#endif //WORLDUNIFORMDATA_HPP