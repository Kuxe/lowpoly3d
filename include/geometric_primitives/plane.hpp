#ifndef PLANE_HPP
#define PLANE_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp> // glm::to_string
#include <glm/gtx/vector_query.hpp> // glm::areCollinear
#include <glm/gtx/polar_coordinates.hpp> // glm::polar, glm::euclidean

#include <functional>
#include <ostream>

#include "geometric_primitives/point.hpp"

#include "utils/glm/glmutils.hpp"
#include "utils/strong_type.hpp"
#include "utils/not_implemented_exception.hpp"

namespace lowpoly3d {

/* A mathematical plane represented by its unit-length normal
 * and a point on said plane */
template<typename floating_point_type, std::size_t dimension>
class TPlane {
public:
	using point_type = glm::vec<dimension, floating_point_type>;

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

	// Projects a given point onto this plane and returns that point
	[[nodiscard]] point_type project(point_type const& point) const {
		/* A point p is on a plane through the origin if n.p + d = 0.
		 * A point P be can be projected onto a plane by forming P + ln
		 * such that n.(P + ln) + d = 0. So solve n.(P + ln) + d = 0 for l,
		 * then the projected point is P + ln.
		 * 
		 * Solving:
		 * 
		 * -d = n.(P+ln) = n.P * l(n.n)
		 *         => l = -(d + n.P) / n.n
		 *  [n normal] = -(d + n.P)
		 * 
		 * Substitute l=-(d + n.P) into P + ln:
		 *  P + l*n = P -(d + n.P)n
		 *          = P - dn - (n.P)n
		 * [d=-n.p] = P + (n.p)n - (n.P)n
		 *          = P + (n.p - n.P)n
		 *          = P + ((p - P).n)n
		 *
		 * So P + l*n = P + ((p - P).n)n */
		return point + glm::dot((getPoint() - point), getNormal())*getNormal();
	}

	// Projects a given point onto this plane and returns the point in a local frame whose z-axis is normal
	[[nodiscard]] TPoint<floating_point_type, 2> projectIntoLocal(point_type const& point) const {
		static_assert(dimension == 3, "projectIntoLocal only implemented for planes in 3D space");

		// If my normal is collinear with 0, 0, 1, then
		// do not choose [0, 0, 1] as a initial vector to cross with,
		// instead choose [0, 1, 0] 
		auto const yaxis = glm::cross(getNormal(),
			! glm::areCollinear(getNormal(), point_type(0, 0, 1), floating_point_type(1e-6))
			? point_type(0, 0, 1)
			: point_type(0, 1, 0)	
		);

		auto const xaxis = glm::cross(yaxis, getNormal());

		assert(glm::isNormalized(xaxis, std::numeric_limits<floating_point_type>::epsilon()));
		assert(glm::isNormalized(yaxis, std::numeric_limits<floating_point_type>::epsilon()));

		/* This is essentially T = [xaxis, yaxis, point], inversed (by transposing, thanks ON),
		 * followed by multiplying with point --- a mapping from world to plane space ---
		 * but since we know we're doing a projection, the z-component of the point is
		 * skipped altogether */
		using vec2 = glm::tvec2<floating_point_type>;
		glm::mat<3, 2, floating_point_type> const world2plane = {
			vec2(xaxis.x, yaxis.x),
			vec2(xaxis.y, yaxis.y),
			vec2(xaxis.z, yaxis.z)
		};

		return world2plane * point;
	}

	// Returns a parametrization of the plane. The parameters are unbounded.
	std::function<point_type(floating_point_type, floating_point_type)> parametrization() const
	{
		// Is there a bijective function from unit vectors in R3 to unit vectors in R3, that is not identiy or negation?
		//
		// Lets figure this out.
		//
		// Represent unit vector v=(x, y, z) in spherical coordinates spherical(v)=(theta, phi)
		// f(spherical(v)) = (theta+pi/2, phi+pi/2) will always yield another unit vector
		// Now convert back to (x, y, z): v'=cartesian(f(spherical(v))).
		// Question: Is v' orthogonal to v? I don't know. Because of this insecurity,
		// I can just cross(v, v') to get a second base-axis, and then cross again to get the third.
		//
		// I doubt this function is bijective though. It does produce vectors
		// that are NOT parallel to original vector (for all unit vectors), which is sufficient.
		// I haven't proved this, but I'll trust my gut on this one.

		auto const polar = glm::polar(n);
		auto const lat = polar[1];
		auto const lon = polar[2];
		auto const v = glm::euclidean(glm::vec<2, floating_point_type>(
			lat + glm::half_pi<floating_point_type>(),
			lon + glm::half_pi<floating_point_type>()
		));

		auto const x = glm::cross(n, v);
		auto const z = glm::cross(x, n);

		return [this, x=x, z=z](floating_point_type u, floating_point_type v) -> point_type {
			return p + u*x + v*z;
		};
	}

private:
	point_type p, n;
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

template<typename floating_point_type, std::size_t dimension>
std::ostream& operator<<(std::ostream& out, const TPlane<floating_point_type, dimension>& plane) {
	const auto& p = plane.getPoint();
	const auto& n = plane.getNormal();
	return out << "(p=" << glm::to_string(p).c_str() << ", n=" << glm::to_string(n).c_str() << ")";
}

} // End of namespace lowpoly3d

#endif //PLANE_HPP