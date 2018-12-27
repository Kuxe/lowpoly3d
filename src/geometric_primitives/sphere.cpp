#include "geometric_primitives/sphere.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <cassert>
#include <iostream>
#include <numeric>

#include "utils/not_implemented_exception.hpp"
#include "geometric_primitives/intersections.hpp"

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
bool TSphere<floating_point_type, dimension>::isMBSof(
	const std::vector<point_type>& points) const {
	return *this == mbsNaive<floating_point_type, dimension>(points);
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

// Returns a minimum bounding sphere over a triangle defined by vertices a, b and c
template<typename floating_point_type, std::size_t dimension>
TSphere<floating_point_type, dimension> mbs(
	const typename TSphere<floating_point_type, dimension>::point_type& a,
	const typename TSphere<floating_point_type, dimension>::point_type& b,
	const typename TSphere<floating_point_type, dimension>::point_type& c) {

	/* Let t=(p1,p2,3), then the center and radius of its MBS is given by:
	 * 
	 * (1)	||p1-c|| = r
	 * (2)	||p2-c|| = r
	 * (3)	||p3-c|| = r
	 * 
	 * Then:
	 * =>	||p1-c|| = ||p2-c||, all solutions for c give a plane 
	 * =>	||p1-c|| = ||p3-c||, all solutions for c give another plane
	 * =>	||p2-c|| = ||p3-c||, all solutions for c give the last plane
	 * 
	 * So there exist exactly one c that satisfy (1-3), namely the intersection
	 * of all 3 planes, which yields a point (if the three planes have different normals)
	 * 
	 * Hence, computing the center of the MBS is equal to that of computing the
	 * intersection between three planes, which is equal to computing the
	 * intersection between two planes (= a line) and then computing the intersection
	 * between that line and the last plane.
	 * 
	 * Finally, the radius is computed using either of (1-3). Done. */ 

	const TPoint<floating_point_type, dimension> point = intersection(
		getEquidistantPlane<floating_point_type, dimension>(a, b),
		getEquidistantPlane<floating_point_type, dimension>(a, c),
		getEquidistantPlane<floating_point_type, dimension>(b, c));

	const auto r = glm::length(a - point);

	// Be paranoid about equations (1-3) being satisfied
	assert(std::abs(r - glm::length(a - point)) < std::numeric_limits<floating_point_type>::epsilon());
	assert(std::abs(r - glm::length(b - point)) < std::numeric_limits<floating_point_type>::epsilon());
	assert(std::abs(r - glm::length(c - point)) < std::numeric_limits<floating_point_type>::epsilon());

	return TSphere<floating_point_type, dimension>(point, r);
}

// Explicit instantiations of sphere
template struct TSphere<float, 3>;
template struct TSphere<double, 3>;

template TSphere<float, 3> mbs(
	const typename TSphere<float, 3>::point_type& a,
	const typename TSphere<float, 3>::point_type& b,
	const typename TSphere<float, 3>::point_type& c);

template TSphere<double, 3> mbs(
	const typename TSphere<double, 3>::point_type& a,
	const typename TSphere<double, 3>::point_type& b,
	const typename TSphere<double, 3>::point_type& c);
	
}; //End of namespace lowpoly3d
