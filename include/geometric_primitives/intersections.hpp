#ifndef INTERSECTIONS_HPP
#define INTERSECTIONS_HPP

#include "geometric_primitives/line.hpp"
#include "geometric_primitives/plane.hpp"
#include "geometric_primitives/point.hpp"

namespace lowpoly3d {

template<typename fpt, std::size_t dim>
struct TLineSegment;

/* intersections.hpp contains a bulk of various intersection
 * tests between geometric primitives that returns the
 * geometry of intersection (for instance, triangle
 * triangle intersection return LineSegment). */

namespace detail {

/* Intersections is a struct that wraps a lot of pure functions
 * This is just to make explicit instantiation easier. */
template<typename fpt, std::size_t dim> 
struct Intersections
{
	static TPoint<fpt, dim> intersection(TPlane<fpt, dim> const&, TLine<fpt, dim> const&);
};

} // End of namespace detail

/* Line-plane intersection that returns the point of intersection.
 * Returns (NaN) if either:
 * 		1. No intersection exists
 * 		2. Infinite many intersections exists */
template<typename fpt, std::size_t dim>
TPoint<fpt, dim> intersection(
	TPlane<fpt, dim> const& plane,
	 TLine<fpt, dim> const& line) {
	return detail::Intersections<fpt, dim>::intersection(plane, line);
}

/* LineSegment-LineSegment intersection. Returns the common point if it exists.
 * Returns NaN if there are infinitly many intersection points
 * or if there is no intersection points */
TPoint< float, 2> intersection(TLineSegment< float, 2> const& l1, TLineSegment< float, 2> const& l2);
TPoint<double, 2> intersection(TLineSegment<double, 2> const& l1, TLineSegment<double, 2> const& l2);

/* Plane-plane intersection yields a line. If the two planes are parallell,
 * then no line of intersection exists. In that case, return a NaN-line consisting
 * of a NaN-point and a NaN-direction */
TLine< float, 3> intersection(TPlane< float, 3> const& p1, TPlane< float, 3> const& p2);
TLine<double, 3> intersection(TPlane<double, 3> const& p1, TPlane<double, 3> const& p2);

/* Line-line intersection in 2D that returns the point of intersection.
 * Returns a point with two quiet NaN components if the two lines are parallel */
TPoint< float, 2> intersection(TLine< float, 2> const& l1, TLine< float, 2> const& l2);
TPoint<double, 2> intersection(TLine<double, 2> const& l1, TLine<double, 2> const& l2);

/* Line-plane intersection that returns the point of intersection */
template<typename fpt, std::size_t dim>
TPoint<fpt, dim> intersection(TLine<fpt, dim> const& line, TPlane<fpt, dim> const& plane) {	return intersection(plane, line); }

/* Returns the common point of the three input planes p1, p2, p3.
 * If no such point exists, returns a NaN point */
template<typename fpt, std::size_t dim>
TPoint<fpt, dim> intersection(TPlane<fpt, dim> const& p1, TPlane<fpt, dim> const& p2, TPlane<fpt, dim> const& p3) { return intersection(intersection(p1, p2), p3); }

/* Solves a system of three linear equations by (naively) using Cramer's rule. */
glm::vec<3,  float, glm::qualifier::defaultp> cramer(glm::mat<3, 3,  float, glm::qualifier::defaultp> const& A, glm::vec<3,  float, glm::qualifier::defaultp> const& b);
glm::vec<3, double, glm::qualifier::defaultp> cramer(glm::mat<3, 3, double, glm::qualifier::defaultp> const& A, glm::vec<3, double, glm::qualifier::defaultp> const& b);

/* Finds an intersection of three planes using Cramer's rule.
 * Precondition: n1, n2 and n3 are mutually non-parallel.
 * If this condition is not satisfied, than a division by zero will happen,
 * this is why this method is unsafe. */
glm::vec<3,  float, glm::qualifier::defaultp> cramer(TPlane< float, 3> p1, TPlane< float, 3> p2, TPlane< float, 3> p3);
glm::vec<3, double, glm::qualifier::defaultp> cramer(TPlane<double, 3> p1, TPlane<double, 3> p2, TPlane<double, 3> p3);

} // End of namespace lowpoly3d

#endif // INTERSECTIONS_HPP