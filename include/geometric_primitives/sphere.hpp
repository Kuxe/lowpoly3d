#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <array>
#include <vector>
#include <ostream>
#include <glm/glm.hpp>

namespace lowpoly3d {

template<typename fpt, std::size_t dim>
using TPoint = glm::vec<dim, fpt>;

template<typename fpt, std::size_t dim>
struct TTriangle;

template<typename TFloatingPointType, std::size_t TDimension>
struct TSphere {
	using floating_point_type = TFloatingPointType;
	static constexpr std::size_t dimension = TDimension;
	using vec_type = ::glm::vec<dimension, floating_point_type>;
	using point_type = TPoint<floating_point_type, dimension>;
	using triangle_type = TTriangle<floating_point_type, dimension>;
	vec_type p;
	floating_point_type r;

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

	// Returns true if point is within this (closed) sphere
	bool contains(
		const point_type& point,
		floating_point_type eps = std::numeric_limits<floating_point_type>::epsilon()) const;
	// Returns true if triangle is within this (closed) sphere
	bool contains(
		const triangle_type& triangle,
		floating_point_type eps = std::numeric_limits<floating_point_type>::epsilon()) const;

	// Returns true if this sphere is the MBS over the provided set of points
	bool isMBSof(const TTriangle<floating_point_type, dimension>& points) const;

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
	
}; //End of lowpoly3d

#endif //SPHERE_HPP