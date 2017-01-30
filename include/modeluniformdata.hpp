#ifndef MODELUNIFORMDATA_HPP
#define MODELUNIFORMDATA_HPP
#include <glm/mat4x4.hpp>

namespace lowpoly3d {

struct ModelUniformData {
	glm::mat4 model, mvp, sunmvp;
};

}

#endif //MODELUNIFORMDATA_HPP