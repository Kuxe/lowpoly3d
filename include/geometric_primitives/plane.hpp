#ifndef PLANE_HPP
#define PLANE_HPP

#include <glm/glm.hpp>

#include "geometric_primitives/point.hpp"

#include "utils/not_implemented_exception.hpp"

namespace lowpoly3d {

// A mathematical plane represented by its normal and a point on the plane
template<typename floating_point_type, std::size_t dimension>
class TPlane {
	using point_type = glm::vec<dimension, floating_point_type>;
	point_type p, n;

public:
	TPlane(const point_type& point, const point_type& normal) : p(point), n(normal) { }
	TPlane(const point_type& normal, floating_point_type d) : p(normal * d / glm::dot(normal, normal)), n(normal) { }
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

	/* Returns the parameter d typically found in the plane equation */
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
		return glm::dot(n, point - p) == 0.0f;
	}

	// Returns the distance to origo
	floating_point_type distance2origo() const {
		TPlane<floating_point_type, dimension> tmp(getNormal(), getD());
		return glm::length(tmp.getPoint());
	}
};

/* 	Returns a plane P s.t ||point1-c|| = ||point2-c|| for all c in P
	whose normal is taken to point from point1 to point2. Return value
	is undefined if ||point1 - point2|| = 0. */
template<typename floating_point_type, std::size_t dimension>
TPlane<floating_point_type, dimension> getEquidistantPlane(
	const glm::vec<dimension, floating_point_type>& point1,
	const glm::vec<dimension, floating_point_type>& point2) {
	const glm::vec3 diff = point2 - point1;
	
	return {midpoint<floating_point_type, dimension>(point1, point2), glm::normalize(diff)};
}

using Plane = TPlane<float, 3>;

} // End of namespace lowpoly3d

#endif //PLANE_HPP