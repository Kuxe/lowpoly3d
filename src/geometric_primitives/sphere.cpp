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
bool TSphere<floating_point_type, dimension>::contains(
	const point_type& point) const {
	return glm::distance(point, p) <= r;
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

	/* Check if MBS is midpoint of longest side, if so, return MBS at the midpoint */
	floating_point_type const abside2 = glm::distance2(a, b);
	floating_point_type const acside2 = glm::distance2(a, c);
	floating_point_type const bcside2 = glm::distance2(b, c);

	using point_type = typename TSphere<floating_point_type, dimension>::point_type;
	using pair_point_type = std::pair<point_type, point_type>;
	pair_point_type farthestApartPoints;
	point_type otherPoint;
	floating_point_type longestSide;

	if(abside2 > acside2) {
		if(abside2 > bcside2) {
			farthestApartPoints = pair_point_type{a, b};
			otherPoint = c;
			longestSide = abside2;
		} else {
			farthestApartPoints = pair_point_type{b, c};
			otherPoint = a;
			longestSide = bcside2;
		}
	} else {
		if(acside2 > bcside2) {
			farthestApartPoints = pair_point_type{a, c};
			otherPoint = b;
			longestSide = acside2;
		} else {
			farthestApartPoints = pair_point_type{b, c};
			otherPoint = a;
			longestSide = bcside2;
		}
	}

	auto const midpoint = floating_point_type(0.5)*(farthestApartPoints.first + farthestApartPoints.second);
	auto const radius = floating_point_type(0.5)*std::sqrt(longestSide);
	TSphere<floating_point_type, dimension> ret(midpoint, radius);
	if(ret.contains(otherPoint)) {
		return ret;
	}

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
	 * of all 3 planes, which yields a point (if the three planes have different normals) */ 
	ret.p = intersection(
		getEquidistantPlane<floating_point_type, dimension>(a, b),
		getEquidistantPlane<floating_point_type, dimension>(a, c),
		getEquidistantPlane<floating_point_type, dimension>(b, c));
	ret.r = glm::length(a - ret.p);
	return ret;
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
