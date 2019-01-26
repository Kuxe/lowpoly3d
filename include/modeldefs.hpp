#ifndef MODELDEFS_HPP
#define MODELDEFS_HPP

#include <glm/vec3.hpp>
#include "geometric_primitives/triangle.hpp"

namespace lowpoly3d {

using Vertex = glm::vec3;
using Color = glm::tvec3<uint8_t>;
using TriangleIndices = glm::tvec3<uint16_t>;

}

#endif //MODELDEFS_HPP