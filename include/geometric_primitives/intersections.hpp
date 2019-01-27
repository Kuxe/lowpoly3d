#ifndef INTERSECTIONS_HPP
#define INTERSECTIONS_HPP

#include <cmath> // NAN
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/vector_query.hpp> //glm::areCollinear
#include <glm/gtx/string_cast.hpp>

#include "geometric_primitives/line.hpp"
#include "geometric_primitives/linesegment.hpp"
#include "geometric_primitives/plane.hpp"
#include "geometric_primitives/triangle.hpp"

/* intersections.hpp contains a bulk of various boolean intersection-tests
 * between geometric primitives. */

namespace lowpoly3d {

// Tests if a point p is within an AABB with one implicit corner in origo and
// another corner given by argument "corner"
template<typename point_type>
bool pointInAABBOrigo(const point_type& p, const point_type& corner) {
    // Check that point is in same quadrant
    if(p.x * corner.x < 0.0 || p.y * corner.y < 0.0) return false;

    // If p is componentwise leq than corner (and they're in same quadrant)
    // then the point is within the AABB of the corner. Done.
    return
        std::abs(p.x) <= std::abs(corner.x) &&
        std::abs(p.y) <= std::abs(corner.y);
}

// A point and a linesegment intersects iff the point lie on the line segment
template<typename floating_point_type, std::size_t dimension>
bool intersects(
	TLineSegment<floating_point_type, dimension> l,
	TPoint<floating_point_type, dimension> p) {
    // Translate the line s.t l.start is at origo
    l.end -= l.start;
    p -= l.start;

    if(dot(p, p) == 0 || p == l.end) return true;
    
    // Make p and l.end unit vectors. Check if plen is greater than endlen,
    // then p is beyond the segment.
    const auto plen = length(p);
    const auto endlen = length(l.end);
    if(plen > endlen) return false;

    // Normalize p and l.end, if their dot is unity then they are perpendicular.
    return std::abs(dot(p / length(p), l.end / length(l.end)) - 1.0) <=
        std::numeric_limits<double>::epsilon();
}

// Returns true if segment intersects plane
template<typename floating_point_type>
constexpr bool intersects(const TLineSegment<floating_point_type, 3>& segment, const TPlane<floating_point_type, 3>& plane) {
    return
        !(plane.above(segment.p1) && plane.above(segment.p2)) || 
        !(plane.below(segment.p1) && plane.below(segment.p2));
}

// point-line intersect commutes
template<typename floating_point_type, std::size_t dimension>
bool intersects(
	const TPoint<floating_point_type, dimension>& p,
	const TLineSegment<floating_point_type, dimension>& l) {
    return intersects(l, p);
}

/* LineSegment-LineSegment intersection. Returns the common point if it exists.
 * Returns NaN if there are infinitly many intersection points
 * or if there is no intersection points */
template<typename floating_point_type, std::size_t dimension>
bool intersects(
    const TLineSegment<floating_point_type, dimension>& l1,
    const TLineSegment<floating_point_type, dimension>& l2) {
	
	// TODO: Implement
	throw NotImplementedException();
}

/* Plane-plane intersection yields a line. If the two planes are parallell,
 * then no line of intersection exists. In that case, return a NaN-line consisting
 * of a NaN-point and a NaN-direction */
template<typename floating_point_type, std::size_t dimension>
TLine<floating_point_type, dimension> intersection(
	const TPlane<floating_point_type, dimension>& plane1,
	const TPlane<floating_point_type, dimension>& plane2) {

	static_assert(dimension == 3, "Plane-plane intersection only implemented for dim=3");

	const auto& n1 = plane1.getNormal();
	const auto& n2 = plane2.getNormal();

	static constexpr auto eps = std::numeric_limits<floating_point_type>::epsilon();
	if(glm::areCollinear(n1, n2, eps)) {
		const auto NaN = std::numeric_limits<floating_point_type>::quiet_NaN();
		const auto nandirection = glm::vec3{NaN, NaN, NaN};
		const auto nanpoint = Point(NaN, NaN, NaN);
		return {nanpoint, nandirection};
	}

	const auto direction = glm::cross(n1, n2);
	const auto pointOnLine = cramer(plane1, plane2, TPlane<floating_point_type, 3>(direction, 0));


	// A vector that is orthogonal to n1 and n2 lies on both planes => a line
	// That vector is n1 cross n2.
	// Need only take an arbitrary point within both planes to determine the
	// line uniquely
	assert(glm::length(n1) == 1.0f);
	assert(glm::length(n2) == 1.0f);
	return {pointOnLine, direction};
}

/* Returns a plane of points that satisfy dot(a, p) = b, where a,b is given
 * and p is any point on the returned plane. */


/* Line-plane intersection that returns the point of intersection.
 * Returns (NaN) if either:
 * 		1. No intersection exists
 * 		2. Infinite many intersections exists */
template<typename floating_point_type, std::size_t dimension>
TPoint<floating_point_type, dimension> intersection(
	const TPlane<floating_point_type, dimension>& plane,
	const TLine<floating_point_type, dimension>& line) {

	/* P+t*d1 = v gives a point v on a line at time t
	 * n dot v = d2 is satisfied for all points v on the plane (n, d2)
	 * 
	 * So solve:
	 * n dot (P+t*d1) = d2 for t in order to find time t of intersection
	 * 
	 * d2 	= n dot (P+t*d1)
	 * 		= n dot P + n dot t*d1
	 * 		= n dot P + t(n dot d1) 
	 * 		=> t = (d2 - n dot P) / n dot d1
	 * 
	 * Then plug t back into line equation to get the point. Done. */

	const auto dot = glm::dot(plane.getNormal(), line.getDirection());
	if(std::abs(dot) <= std::numeric_limits<floating_point_type>::epsilon()) {
		return TPoint<floating_point_type, dimension>(NAN);
	}

	// t can be interpreted as time until intersection assuming unit speed in some unit (such as m/s)
	const auto t = (-plane.getD() - glm::dot(plane.getNormal(), line.getPoint())) / dot;
	return line.parametrization(t);
}

/* Line-plane intersection that returns the point of intersection */
template<typename floating_point_type, std::size_t dimension>
TPoint<floating_point_type, dimension> intersection(
	const TLine<floating_point_type, dimension>& line,
	const TPlane<floating_point_type, dimension>& plane) {
	return intersection(plane, line);
}

/* Returns the common point of the three input planes p1, p2, p3.
 * If no such point exists, returns a NaN point */
template<typename floating_point_type, std::size_t dimension>
TPoint<floating_point_type, dimension> intersection(
	const TPlane<floating_point_type, dimension>& p1,
	const TPlane<floating_point_type, dimension>& p2,
	const TPlane<floating_point_type, dimension>& p3) {
		return intersection(intersection(p1, p2), p3);
}

/* Solves a system of three linear equations by (naively) using Cramer's rule. */
template<typename floating_point_type>
glm::tvec3<floating_point_type> cramer(
	const glm::tmat3x3<floating_point_type>& A,
	const glm::tvec3<floating_point_type>& b)
{
	const auto det = glm::determinant(A);
	if(std::abs(det) <= std::numeric_limits<floating_point_type>::epsilon()) {
		std::ostringstream oss;
		oss << "Determinant is zero, division by zero follows ";
		oss << glm::to_string(glm::column(A, 0)) << ",";
		oss << glm::to_string(glm::column(A, 1)) << ",";
		oss << glm::to_string(glm::column(A, 2)) << ", ";
		oss << "A=" << glm::to_string(A);
		throw std::runtime_error(oss.str());
	}

	/* *** Cramers *** *

		a1 b1 c1  x    d1
		a2 b2 c2  y    d2
		a3 b3 c3  z    d3

		a1 b1 c1  x 0 0    d1 0 0
		a2 b2 c2  y 0 0    d2 0 0
		a3 b3 c3  z 0 0    d3 0 0

		a1 b1 c1  x 0 0    d1 b1 c1
		a2 b2 c2  y 1 0    d2 b2 c2
		a3 b3 c3  z 0 1    d3 b3 c3

		a1 b1 c1  x 1 0    d1 a1 c1
		a2 b2 c2  y 0 0    d2 a2 c2
		a3 b3 c3  z 0 1    d3 a3 c3

		a1 b1 c1  x 0 1    d1 -b1 a1
		a2 b2 c2  y -1 0   d2 -b2 a2
		a3 b3 c3  z 0 0    d3 -b3 a3

	* *** ******* *** */

	glm::tmat3x3<floating_point_type> tmp = A;
	tmp = glm::column(tmp, 0, b);
	const auto detX = glm::determinant(tmp);

	tmp = glm::column(tmp, 1, glm::column(A, 0));
	const auto detY = glm::determinant(tmp);

	tmp = glm::column(tmp, 1, -glm::column(A, 1));
	tmp = glm::column(tmp, 2, glm::column(A, 0));
	const auto detZ = glm::determinant(tmp);

	return {detX / det, -detY / det, detZ / det};
}

/* Finds an intersection of three planes using Cramer's rule.
 * Precondition: n1, n2 and n3 are mutually non-parallel.
 * If this condition is not satisfied, than a division by zero will happen,
 * this is why this method is unsafe. */
template<typename floating_point_type>
glm::tvec3<floating_point_type> cramer(
	TPlane<floating_point_type, 3> p1,
	TPlane<floating_point_type, 3> p2,
	TPlane<floating_point_type, 3> p3)
{
	const auto& n1 = p1.getNormal();
	const auto& n2 = p2.getNormal();
	const auto& n3 = p3.getNormal();

	const glm::tmat3x3<floating_point_type> A(n1, n2, n3);
	const glm::tvec3<floating_point_type> b {-p1.getD(), -p2.getD(), -p3.getD()};

	/* A is constructed with n1 as first base, n2 as second base etc. However,
	 * when solving using cramer we expect the matrix A and vector b to look like:
	 * 
	 * n1x n1y n1z   x = d1
	 * n2x n2y n2z * y = d2
	 * n3x n3y n3z   z = d3
	 * 
	 * instead of 
	 * 
	 * n1x n2x n3x   x = d1
	 * n1y n2y n3y * y = d2
	 * n1z n2z n3z   z = d3
	 * 
	 * so transpose it. */
	return cramer(glm::transpose(A), b);
}

template<typename floating_point_type, std::size_t dimension>
constexpr bool intersects(
	const TPlane<floating_point_type, dimension>& plane,
	const TLineSegment<floating_point_type, dimension>& segment) {
    return intersects(segment, plane);
}

template<typename floating_point_type, std::size_t dimension>
constexpr bool intersects(
	const TLineSegment<floating_point_type, dimension>& segment,
	const TTriangle<floating_point_type, dimension>& triangle) {
    // The segment must at least intersect with paralell plane to the triangle
    // if they should intersect at all
    if(intersects(segment, triangle.parallel())) {
        // Proceed to check if the line segment actually intersects the triangle
        // TODO: Implement
		throw NotImplementedException();
    }
    return false;
}

template<typename floating_point_type, std::size_t dimension>
constexpr bool intersects(
	const TTriangle<floating_point_type, dimension>& triangle,
	const TLineSegment<floating_point_type, dimension>& segment) {
    return intersects(segment, triangle);
}

template<typename floating_point_type>
constexpr bool intersects(const TTriangle<floating_point_type, 3>& t1, const TTriangle<floating_point_type, 3>& t2) {
    // A triangle T1 intersects another triangle T2 iff any line segment of T1 intersect T2 (or vice versa)
    return
        intersects(TLineSegment<floating_point_type, 3>(t1.p1, t1.p2), t2) ||
        intersects(TLineSegment<floating_point_type, 3>(t1.p2, t1.p3), t2) || 
        intersects(TLineSegment<floating_point_type, 3>(t1.p3, t1.p1), t2);
}

}

#endif //INTERSECTIONS_HPP