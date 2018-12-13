#ifndef INTERSECTIONS_HPP
#define INTERSECTIONS_HPP

#include <cmath> // NAN

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
template<typename value_type>
constexpr bool intersects(const TLineSegment<value_type, 3>& segment, const Plane& plane) {
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
 * then no line of intersection exists. In that case, the direction of the
 * returned line is the zero vector */
template<typename floating_point_type, std::size_t dimension>
TLine<floating_point_type, dimension> intersection(
	const TPlane<floating_point_type, dimension>& plane1,
	const TPlane<floating_point_type, dimension>& plane2) {

	static_assert(dimension == 3, "Plane-plane intersection only implemented for dim=3");
	using vec3_type = glm::vec<dimension, floating_point_type>;
	const auto zerovec = vec3_type(0.0f, 0.0f, 0.0f);

	const auto& n1 = plane1.getNormal();
	const auto& n2 = plane2.getNormal();

	static constexpr auto eps = std::numeric_limits<floating_point_type>::epsilon();
	if(glm::areCollinear(n1, n2, eps)) {
		return {zerovec, zerovec};
	}

	const auto direction = glm::cross(n1, n2);

	/* ax + by + cz = d and
	 * ex + fy + gz = h
	 * 
	 * with n1=(a,b,c), n2=(e,f,g)
	 * is equivalent to solving
	 * 
	 * ax + by + cz - d = 0 and
	 * ex + fy + gz - h = 0
	 * 
	 * same as two dot-products in 4D.
	 * So two dot-products in 4D equal zero => a solution v=(x, y, z, 1)
	 * satisfies v_|_(a,b,c,d) and v_|_(e,f,g,h).
	 * So just pick an arbitrary third 4D-vector, for instance (0, 0, 0, 1)
	 * and find a vector v thats orthogonal to these three vectors.
	 * Then vx, vy, vz will be a solution to the original equation
	 * TODO: Figure out if I need to divide by w */


	using vec4_type = glm::vec<4, floating_point_type>;
	const vec4_type v1 = {n1.x, n1.y, n1.z, -plane1.getD()};
	const vec4_type v2 = {n2.x, n2.y, n2.z, -plane2.getD()};
	const vec4_type v3 = {0.0f, 0.0f, 0.0f, 1.0f};

	using mat3_type = glm::tmat3x3<floating_point_type>;
	
	vec4_type v4 = {
		+glm::determinant(mat3_type {
			{v1[1], v1[2], v1[3]},
			{v2[1], v2[2], v2[3]},
			{v3[1], v3[2], v3[3]}
		}),
		-glm::determinant(mat3_type {
			{v1[0], v1[2], v1[3]},
			{v2[0], v2[2], v2[3]},
			{v3[0], v3[2], v3[3]}
		}),
		+glm::determinant(mat3_type {
			{v1[0], v1[1], v1[3]},
			{v2[0], v2[1], v2[3]},
			{v3[0], v3[1], v3[3]}
		}),
		-glm::determinant(mat3_type {
			{v1[0], v1[1], v1[2]},
			{v2[0], v2[1], v2[2]},
			{v3[0], v3[1], v3[2]}
		})};


	// A vector that is orthogonal to n1 and n2 lies on both planes => a line
	// That vector is n1 cross n2.
	// Need only take an arbitrary point within both planes to determine the
	// line uniquely
	assert(glm::length(n1) == 1.0f);
	assert(glm::length(n2) == 1.0f);
	return {direction, floating_point_type(1.0)/v4[3] * vec3_type(v4[0], v4[1], v4[2])};
}

/* Returns the point of intersection between given plane and line.
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
	 * Then plug t back into line equation to get the point. Done. */

	const auto dot = glm::dot(plane.getNormal(), line.getDirection());
	if(std::abs(dot) < std::numeric_limits<floating_point_type>::epsilon()) {
		return TPoint<floating_point_type, dimension>(NAN);
	}

	// t can be interpreted as time until intersection assuming unit speed in some unit (such as m/s)
	const auto t = plane.getD() - glm::dot(plane.getNormal(), line.getPoint()) / dot;
	return line.getPoint() + t * line.getDirection();
}

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

template<typename floating_point_type, std::size_t dimension>
constexpr bool intersect(
	const TPlane<floating_point_type, dimension>& plane,
	const TLineSegment<floating_point_type, dimension>& segment) {
    return intersect(segment, plane);
}

template<typename floating_point_type, std::size_t dimension>
constexpr bool intersect(
	const TLineSegment<floating_point_type, dimension>& segment,
	const TTriangle<floating_point_type, dimension>& triangle) {
    // The segment must at least intersect with paralell plane to the triangle
    // if they should intersect at all
    if(intersect(segment, triangle.parallel())) {
        // Proceed to check if the line segment actually intersects the triangle
        // TODO: Implement
		throw NotImplementedException();
    }
    return false;
}

template<typename floating_point_type, std::size_t dimension>
constexpr bool intersect(
	const TTriangle<floating_point_type, dimension>& triangle,
	const TLineSegment<floating_point_type, dimension>& segment) {
    return intersect(segment, triangle);
}

template<typename value_type>
constexpr bool intersect(const TTriangle<value_type, 3>& t1, const TTriangle<value_type, 3>& t2) {
    // A triangle T1 intersects another triangle T2 iff any line segment of T1 intersect T2 (or vice versa)
    return
        intersect(LineSegment(t1.p1, t1.p2), t2) ||
        intersect(LineSegment(t1.p2, t1.p3), t2) || 
        intersect(LineSegment(t1.p3, t1.p1), t2);
}

}

#endif //INTERSECTIONS_HPP