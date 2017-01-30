#ifndef MODELDEFS_HPP
#define MODELDEFS_HPP

#include <glm/vec3.hpp>

namespace lowpoly3d {

using Vertex = glm::vec3;
using Color = glm::tvec3<uint8_t>;
using Triangle = glm::tvec3<uint16_t>;

}

#endif //MODELDEFS_HPP