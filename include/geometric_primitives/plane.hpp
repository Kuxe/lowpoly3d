#ifndef PLANE_HPP
#define PLANE_HPP

#include <glm/glm.hpp>

#include "geometric_primitives/point.hpp"

#include "utils/strong_type.hpp"
#include "utils/not_implemented_exception.hpp"

namespace lowpoly3d {

/* A mathematical plane represented by its unit-length normal
 * and a point on said plane */
template<typename floating_point_type, std::size_t dimension>
class TPlane {
	using point_type = glm::vec<dimension, floating_point_type>;
	point_type p, n;

public:

	static point_type getPointOnPlane(const point_type& normalizedNormal, floating_point_type d) {
		/*
			n1*x + n2*y + n3*z = d, chose a point (x,y,z) = s*(n1,n2,n3) s.t equation is satisfied =>
			s(n1*n1 + n2*n2 + n3*n3) = d =>
			s*dot(n, n) = d, n normalized =>
			s = d

			Hence dn is a point on the plane.
			dn is clearly the point with the shortest distance to origo. This is geometrically
			intuitive. If you're a point at origo and want to reach the plane as quickly as possible,
			then obviously you would move along the normal of the plane --- moving in some slightly different
			direction would be like moving along a hypotenuse of a triangle instead of moving along a cathetus.
		*/
		return d * normalizedNormal;
	}

	TPlane(const point_type& point, const point_type& normal) : p(point), n(glm::normalize(normal)) { }
	TPlane(const point_type& normal, floating_point_type d) : p(getPointOnPlane(glm::normalize(normal), d)), n(glm::normalize(normal)) { }
	TPlane(const TPlane& other) : TPlane(other.p, other.n) { }
	TPlane(TPlane&& other) : p(std::move(other.p)), n(std::move(other.n)) { }
	virtual ~TPlane() { }

	TPlane& swap(TPlane plane) {
		point_type ptmp = p, ntmp = n;
		p = plane.p;
		n = plane.n;
		plane.p = ptmp;
		plane.n = ntmp;
		return *this;
	}

	TPlane& operator=(TPlane plane) {
		return swap(plane);
	}

	const point_type& getPoint() const { return p; }
	const point_type& getNormal() const { return n; }

	/* Returns the parameter d typically found in the plane equation, 
	 * i.e ax + by + cz + d = 0 */
	floating_point_type getD() const {
		return -glm::dot(n, p);
	}

	// Returns true if point is above the plane, otherwise false
	bool above(const point_type& point) const {
		return glm::dot(n, point - p) > 0.0f;
	}

	// Returns true if point is below the plane, otherwise false
	bool below(const point_type& point) const {
		return glm::dot(n, point - p) < 0.0f;
	}

	// Returns true if point lies on the plane, otherwise false
	bool contains(const point_type& point) const {
		// It should really be + since we want to check the difference between negative d and dot-product.
		return std::abs(glm::dot(n, point) + getD()) <= std::numeric_limits<floating_point_type>::epsilon();
	}

	// Returns the distance to origo
	floating_point_type distance2origo() const {
		/* See "proof" within getPointOnPlane(). Since normal is of unit length,
		 * it follows trivially that 'd' is also the shortest distance to origo */
		return std::abs(getD());
	}

	// Returns true if "plane" is parallel to this plane
	[[nodiscard]] bool isParallelTo(const TPlane<floating_point_type, dimension>& plane) const {
		return std::abs(glm::dot(getNormal(), plane.getNormal())) - floating_point_type(1) <= std::numeric_limits<floating_point_type>::epsilon();
	}
};

/* 	Returns a plane P s.t ||point1-c|| = ||point2-c|| for all c in P
	whose normal is taken to point from point1 to point2. Return value
	is undefined if ||point1 - point2|| = 0. */
template<typename floating_point_type, std::size_t dimension>
TPlane<floating_point_type, dimension> getEquidistantPlane(
	const TPoint<floating_point_type, dimension>& point1,
	const TPoint<floating_point_type, dimension>& point2) {
	const glm::vec3 diff = point2 - point1;
	
	return {midpoint<floating_point_type, dimension>(point1, point2), glm::normalize(diff)};
}

using Plane = TPlane<float, 3>;

} // End of namespace lowpoly3d

#endif //PLANE_HPP