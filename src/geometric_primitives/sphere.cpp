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

	/* Check if MBS is midpoint of longest side, if so, return MBS at the midpoint */
	floating_point_type const abside = glm::distance2(a, b);
	floating_point_type const acside = glm::distance2(a, c);
	floating_point_type const bcside = glm::distance2(b, c);

	using point_type = typename TSphere<floating_point_type, dimension>::point_type;
	using pair_point_type = std::pair<point_type, point_type>;
	pair_point_type farthestApartPoints;
	point_type otherPoint;
	floating_point_type longestSide;

	if(abside > acside) {
		if(abside > bcside) {
			farthestApartPoints = pair_point_type{a, b};
			otherPoint = c;
			longestSide = abside;
		} else {
			farthestApartPoints = pair_point_type{b, c};
			otherPoint = a;
			longestSide = bcside;
		}
	} else {
		if(acside > bcside) {
			farthestApartPoints = pair_point_type{a, c};
			otherPoint = b;
			longestSide = acside;
		} else {
			farthestApartPoints = pair_point_type{b, c};
			otherPoint = a;
			longestSide = bcside;
		}
	}

	auto const midpoint = floating_point_type(0.5)*(farthestApartPoints.first + farthestApartPoints.second);
	auto const radius = floating_point_type(0.5)*longestSide;
	if(glm::distance(midpoint, otherPoint) <= radius) {
		return {midpoint, radius};
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
	 * of all 3 planes, which yields a point (if the three planes have different normals)
	 * 
	 * Hence, computing the center of the MBS is equal to that of computing the
	 * intersection between three planes, which is equal to computing the
	 * intersection between two planes (= a line) and then computing the intersection
	 * between that line and the last plane.
	 * 
	 * Finally, the radius is computed using either of (1-3). Done. */ 

	/* TODO: intersection(p1, p2, p3) uses intersection(intersection(p1, p2), p3) internally
	 * which give NaN for some reason. However, chaning to use cramer instead of intersection
	 * causes determinant to be zero (could this hint of the problem?). Aha.
	 * Draw a picture of an equilateral triangle. The intersection of all planes will
	 * be a line (not a point). So what happens is that intersection(p1, p2) gives
	 * a line that is parallel to the plane p3. Documentation of line-plane test
	 * states that NaN-point is returned if the line is parallel to the plane...
	 * 
	 * So the problem is really with the method since, i.e equations (1-3) that do not
	 * uniquely determine a point for all possible planes.
	 * 
	 * Further, equation (1-3) does not even give the smallest bounding sphere
	 * Consider T=(-10, 0, 0,), (+10, 0, 0), (0, 1, 0)). Obviously, the smallest bounding sphere
	 * is the sphere with radius 10 in (0, 0, 0). The problem with (1-3) is that these equations
	 * yield the center of a sphere with all three vertices intersecting the sphere,
	 * but most often one vertex out of three lies within the sphere...
	 * 
	 * Hmm.
	 * 
	 * Lemma 1: Any MBS over a triangle has at least two of the triangle vertices on its surface.
	 * Proof: This is obvious.
	 * 
	 * Lemma 2: The center of the MBS must lie on the equidistant plane between (the) two farther apart points
	 * Proof: Obvious from Lemma 1.
	 * 
	 * So now the question is "where" on the such a equidistant plane the points should lie. Obviously it must
	 * lie on the plane that contains the triangle.
	 * 
	 * "Best case" is that it lies on the midpoint between the two vertices that are the farthest apart.
	 * However, it is possible that the center must "slide" along the line towards the third vertex.
	 * How far must it slide? I must slide exactly so far that the sphere "hits" the third vertex.
	 *
	 * A case that captures this is T={{-3,0,0}, {+3,0,0}, {1,4,0}}.
	 * A sphere between {-3,0,0} and {3,0,0} of radius 3 is to small to contain {1,4,0}
	 * So slide it along equidistant plane of farthest apart points, and increase radius
	 * as you slide s.t the two farthest points are still on the sphere, until you hit
	 * the third point. But then I have to slide it such that equation (1-3) holds.
	 * 
	 * So I think this is a MBS function:
	 * Create sphere with center equal to midpoint of farther apart points
	 * s.t it contains those two points exactly. If third point is also contained
	 * in that sphere, then we're done.
	 * 
	 * Otherwise, calculate center and radius according to (1-3).
	 * But then I still have the problem that (1-3) yields NaN.
	 * 
	 * Still not confident that the algorithm above is correct. EDIT: I drew some images and now I am confident.
	 * It is correct. 
	 * 
	 * */
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
