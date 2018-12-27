#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <array>
#include <vector>
#include <ostream>
#include <glm/glm.hpp>

#include "geometric_primitives/plane.hpp"
#include "geometric_primitives/point.hpp"

#include "utils/not_implemented_exception.hpp"

namespace lowpoly3d {

template<typename TFloatingPointType, std::size_t TDimension>
struct TSphere {
	using floating_point_type = TFloatingPointType;
	static constexpr std::size_t dimension = TDimension;
	using vec_type = ::glm::vec<dimension, floating_point_type>;
	using point_type = TPoint<floating_point_type, dimension>;
	vec_type p;
	floating_point_type r;

	TSphere() = default;
	constexpr TSphere(const vec_type& p, const floating_point_type r) : p(p), r(r) {
		assert(!std::isinf(r));
		assert(!std::isinf(p.x) && !std::isinf(p.y) && !std::isinf(p.z));
		assert(!std::isnan(r));
		assert(!std::isnan(p.x) && !std::isnan(p.y) && !std::isnan(p.z));

		/* 	Check that each component of p is not due to floating-point errors.
			That is, if p.x != 0 then assert that p.x is greater than
			some really really small number.
			I don't think these checks make any sense. It is really possible
			that two floating point numbers are extremely close to each others,
			then these tests to not make any sense since components of "p"
			might be very small (and legitimately) so*/
		/*assert(p.x == 0.0 || std::abs(p.x) >= epsilon);
		assert(p.y == 0.0 || std::abs(p.y) >= epsilon);
		assert(p.z == 0.0 || std::abs(p.z) >= epsilon);
		assert(r == 0.0 || r >= epsilon);*/
	}
	bool operator==(const TSphere& other) const;
	bool operator!=(const TSphere& other) const;
	bool encloses(const TSphere& other) const;
	bool enclosed(const TSphere& other) const;

	// Returns true if this sphere is the MBS over the provided set of points
	bool isMBSof(const std::vector<point_type>& points) const;

	constexpr TSphere(const TSphere& s) = default;

	//TODO: Rename? What is the "size" of a sphere? Volume? Area? Radius?
	floating_point_type size() const;
}; //End of Sphere

using Sphere = TSphere<float, 3>;

template<typename floating_point_type, std::size_t dimension>
std::ostream& operator<<(std::ostream& os, const TSphere<floating_point_type, dimension>& obj) {
    os << "(p={" << obj.p.x << ", " << obj.p.y << ", " << obj.p.z << "}, r=" << obj.r << ")";
    return os;
}

template<typename floating_point_type, std::size_t dimension>
typename TSphere<floating_point_type, dimension>::floating_point_type signed_distance(
	const TSphere<floating_point_type, dimension>& a,
	const TSphere<floating_point_type, dimension>& b) {
	return ::glm::distance(a.p, b.p) - a.r - b.r;
}

// Returns the signed distance (=distance or penetration depth)
Sphere::floating_point_type signed_distance(
	const Sphere& a,
	const Sphere& b,
	const ::glm::mat4& a_transform,
	const ::glm::mat4& b_transform);

template<typename floating_point_type, std::size_t dimension>
bool colliding(
	const TSphere<floating_point_type, dimension>& a,
	const TSphere<floating_point_type, dimension>& b) {
	return ::glm::dot(a.p, b.p) < (a.r + b.r)*(a.r + b.r);
}

bool colliding(const Sphere& a, const Sphere& b, const ::glm::mat4& a_transform, const ::glm::mat4& b_transform);

// Returns a minimum bounding sphere over a triangle defined by vertices a, b and c
template<typename floating_point_type, std::size_t dimension>
TSphere<floating_point_type, dimension> mbs(
	const typename TSphere<floating_point_type, dimension>::point_type& a,
	const typename TSphere<floating_point_type, dimension>::point_type& b,
	const typename TSphere<floating_point_type, dimension>::point_type& c);

// Returns a minimum bounding sphere over a triangle
template<typename floating_point_type, std::size_t dimension>
TSphere<floating_point_type, dimension> mbs(
	const std::array<typename TSphere<floating_point_type, dimension>::vec_type, 3>& triangle) {
	return mbs<floating_point_type, dimension>(triangle[0], triangle[1], triangle[2]);
}

// Finds a minimum bounding sphere that encloses two spheres a and b
template<typename floating_point_type, std::size_t dimension>
TSphere<floating_point_type, dimension> mbs(
	const TSphere<floating_point_type, dimension>& a,
	const TSphere<floating_point_type, dimension>& b) {

	using vec_type = typename TSphere<floating_point_type, dimension>::vec_type;

	const vec_type diff = b.p - a.p;
	const floating_point_type d = ::glm::length(diff);
	// FIXME: d can be zero, causing a divide by zero
	// If one sphere is enclosed by the other sphere, return the bigger sphere
	const floating_point_type radiidiff = std::abs(a.r - b.r);
	if (d <= radiidiff) {
		// One sphere is fully enclosed in the other, return the biggest sphere
		return a.r > b.r ? a : b; 
	} else {
		const vec_type unitdiff = diff / d;
		const vec_type e2 = b.p + unitdiff * b.r;
		const vec_type e1 = a.p - unitdiff * a.r;
		const vec_type midpoint = Sphere::floating_point_type(0.5)*(e1 + e2);

		const floating_point_type enclosing_radius = Sphere::floating_point_type(0.5) * (d + a.r + b.r); // Since glm::distance(e1, e2) / 2.0f = (d + a.r + b.r) / 2.0f
		assert(enclosing_radius >= a.r);
		assert(enclosing_radius >= b.r);
		assert(Sphere(midpoint, enclosing_radius).encloses(a));
		assert(Sphere(midpoint, enclosing_radius).encloses(b));
		return {midpoint, enclosing_radius};
	}
}

// Computes the minimum bounding sphere over a set of points in O(n^2)-time
template<typename floating_point_type, std::size_t dimension>
TSphere<floating_point_type, dimension> mbsNaive(
	const std::vector<typename TSphere<floating_point_type, dimension>::point_type>& points) {

	throw NotImplementedException();
}

	
}; //End of lowpoly3d

#endif //SPHERE_HPP