#ifndef MODELUNIFORMDATA_HPP
#define MODELUNIFORMDATA_HPP
#include <glm/glm.hpp>

struct ModelUniformData {
	glm::mat4 model, mvp, sunmvp;
};

#endif //MODELUNIFORMDATA_HPP