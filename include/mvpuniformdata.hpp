#ifndef MVPUNIFORMDATA_HPP
#define MVPUNIFORMDATA_HPP
#include <glm/mat4x4.hpp>

namespace lowpoly3d {

struct MVPUniformData {
	glm::mat4 mvp;
};

}

#endif //MVPUNIFORMDATA_HPP