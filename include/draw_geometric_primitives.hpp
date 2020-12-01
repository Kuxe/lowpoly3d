#ifndef DRAW_GEOMETRIC_PRIMITIVES_HPP
#define DRAW_GEOMETRIC_PRIMITIVES_HPP

#include "geometric_primitives/point.hpp"

namespace lowpoly3d {

class Scene;

// Draws a point in world-space onto the given scene
void draw(Scene& iScene, Point const& iPoint);

} // End of namespace lowpoly3d

#endif // DRAW_GEOMETRIC_PRIMITIVE_HPP