#include <algorithm> // std::all_of
#include <cmath>
#include <cassert>
#include <numeric>

#include "geometric_primitives/intersects.hpp"
#include "geometric_primitives/sphere.hpp"
#include "geometric_primitives/triangle.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "minimum_bounding_sphere.hpp"

#include "utils/glm/glmprint.hpp"
#include "utils/not_implemented_exception.hpp"

namespace lowpoly3d {

template<typename floating_point_type, std::size_t dimension>
bool TSphere<floating_point_type, dimension>::operator==(
	const TSphere& other) const {
	return p == other.p && r == other.r;	
}

template<typename floating_point_type, std::size_t dimension>
bool TSphere<floating_point_type, dimension>::operator!=(
	const TSphere& other) const {
	return !(*this == other);
}

template<typename floating_point_type, std::size_t dimension>
bool TSphere<floating_point_type, dimension>::encloses(
	const TSphere& other) const {
	return other.r <= ::glm::distance(p, other.p) + r + std::numeric_limits<float>::epsilon();
}

template<typename floating_point_type, std::size_t dimension>
bool TSphere<floating_point_type, dimension>::enclosed(
	const TSphere& other) const {
	return other.encloses(*this);
}

template<typename floating_point_type, std::size_t dimension>
bool TSphere<floating_point_type, dimension>::contains(
	const point_type& point,
	floating_point_type eps) const {
	return glm::distance(point, p) <= r + eps;
}

template<typename floating_point_type, std::size_t dimension>
bool TSphere<floating_point_type, dimension>::contains(
	const triangle_type& triangle,
	floating_point_type eps) const {
	return std::all_of(
		triangle.cbegin(),
		triangle.cend(),
	[this, eps](auto const& point) {
		return contains(point, eps);
	});
}

template<typename floating_point_type, std::size_t dimension>
bool TSphere<floating_point_type, dimension>::isMBSof(
	const TTriangle<floating_point_type, dimension>& triangle) const {
	return *this == mbs<floating_point_type, dimension>(triangle);
}

template<typename floating_point_type, std::size_t dimension>
floating_point_type TSphere<floating_point_type, dimension>::size() const { return r; }

/* Linker error if not defined when declared albeit explicit instantiations
template<typename floating_point_type, std::size_t dimension>
typename TSphere<floating_point_type, dimension>::floating_point_type signed_distance(
	const TSphere<floating_point_type, dimension>& a,
	const TSphere<floating_point_type, dimension>& b) {
	return glm::distance(a.p, b.p) - a.r - b.r;
}*/

Sphere::floating_point_type signed_distance(
	const Sphere& a,
	const Sphere& b,
	const ::glm::mat4& a_transform,
	const ::glm::mat4& b_transform) {
	return ::glm::distance(
		Sphere::vec_type(a_transform * ::glm::vec4(a.p, 1.0)),
		Sphere::vec_type(b_transform * ::glm::tvec4<Sphere::floating_point_type>(b.p, 1.0))) - a.r - b.r;
}

bool colliding(const Sphere& a, const Sphere& b, const ::glm::mat4& a_transform, const ::glm::mat4& b_transform) {
	const Sphere::vec_type atrans = Sphere::vec_type(a_transform * ::glm::tvec4<Sphere::floating_point_type>(a.p, 1.0));
	const Sphere::vec_type btrans = Sphere::vec_type(b_transform * ::glm::tvec4<Sphere::floating_point_type>(b.p, 1.0));
	const Sphere::vec_type diff = atrans - btrans;
	const auto diffdot = ::glm::dot(diff, diff);
	const auto radiussq = (a.r + b.r)*(a.r + b.r);
	return diffdot < radiussq;
}

// Explicit instantiations of sphere
template struct TSphere<float, 3>;
template struct TSphere<double, 3>;
	
}; //End of namespace lowpoly3d
