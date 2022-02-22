#ifndef DRAW_GEOMETRIC_PRIMITIVES_HPP
#define DRAW_GEOMETRIC_PRIMITIVES_HPP

#include "geometric_primitives/cone.hpp"
#include "geometric_primitives/cylinder.hpp"
#include "geometric_primitives/line.hpp"
#include "geometric_primitives/linesegment.hpp"
#include "geometric_primitives/parallelogram.hpp"
#include "geometric_primitives/point.hpp"
#include "geometric_primitives/rectangle.hpp"
#include "geometric_primitives/sphere.hpp"
#include "geometric_primitives/triangle.hpp"

namespace lowpoly3d {

class Scene;

// Draws a point in world-space onto the given scene
void draw(Scene& iScene, Cone const& iCone);
void draw(Scene& iScene, Cylinder const& iCylinder);
void draw(Scene& iScene, Line const& iLine);
void draw(Scene& iScene, LineSegment const& iLineSegment);
void draw(Scene& iScene, Parallelogram const& iParallelogram);
void draw(Scene& iScene, Point const& iPoint);
void draw(Scene& iScene, Rectangle const& iRectangle);
void draw(Scene& iScene, Sphere const& iSphere);
void draw(Scene& iScene, Triangle const& iTriangle);

} // End of namespace lowpoly3d

#endif // DRAW_GEOMETRIC_PRIMITIVE_HPP