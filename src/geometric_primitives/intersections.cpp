#include "geometric_primitives/intersections.hpp"

#include <cmath> // NAN
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/vector_query.hpp> //glm::areCollinear
#include <glm/gtx/string_cast.hpp>

#include "geometric_primitives/linesegment.hpp"
#include "geometric_primitives/sphere.hpp"
#include "geometric_primitives/triangle.hpp"

#include "utils/solve.hpp"

namespace lowpoly3d {
namespace detail {

template<typename fpt, std::size_t dim>
TPoint<fpt, dim> Intersections<fpt, dim>::intersection(TPlane<fpt, dim> const& plane, TLine<fpt, dim> const& line) {
	auto const t = intersection_line_param(plane, line);
	return t == NAN ? TPoint<fpt, dim>(NAN) : line.parametrization(t);
}

template<typename fpt, std::size_t dim>
fpt Intersections<fpt, dim>::intersection_line_param(TPlane<fpt, dim> const& plane, TLine<fpt, dim> const& line)
{
	/* P+t*d1 = v gives a point v on a line at time t
	 * n dot v = d2 is satisfied for all points v on the plane (n, d2)
	 * 
	 * So solve:
	 * n dot (P+t*d1) = d2 for t in order to find time t of intersection
	 * 
	 * d2 	= n dot (P+t*d1)
	 * 		= n dot P + n dot t*d1
	 * 		= n dot P + t(n dot d1) 
	 * 		=> t = (d2 - n dot P) / n dot d1 */

	const auto dot = glm::dot(plane.getNormal(), line.getDirection());
	if(std::abs(dot) <= std::numeric_limits<fpt>::epsilon()) {
		return NAN;
	}

	// t can be interpreted as time until intersection assuming unit speed in some unit (such as m/s)
	const auto t = (-plane.getD() - glm::dot(plane.getNormal(), line.getPoint())) / dot;
	return t;
}

template<typename fpt, std::size_t dim>
TPoint<fpt, dim> intersection(TTriangle<fpt, dim> const& triangle, TLine<fpt, dim> const& line)
{
	auto const p = intersection(parallel(triangle), line);
	auto const edge12Plane = Plane(triangle.p1, edge_normal_12(triangle));
	auto const edge23Plane = Plane(triangle.p2, edge_normal_23(triangle));
	auto const edge31Plane = Plane(triangle.p3, edge_normal_31(triangle));

	return edge12Plane.below(p) && edge23Plane.below(p) && edge31Plane.below(p)	? p : TPoint<fpt, dim>{NAN};
}

template<typename fpt>
TPoint<fpt, 2> intersection(TLineSegment<fpt, 2> const& l1, TLineSegment<fpt, 2> const& l2)
{
	// Compute two t's in (-inf, +inf) that run along the (infinite) lines induced
	// by each LineSegment. If the two t's lie in [0, 1] then the LineSegments
	// intersect
	auto const ts = solveScalingVecs(l1.p2 - l1.p1, l2.p1 - l2.p2, l2.p1 - l1.p1);

	assert(glm::all(glm::epsilonEqual((l1.p2 - l1.p1)*ts.x + l1.p1, (l2.p2 - l2.p1)*ts.y + l2.p1, std::numeric_limits<fpt>::epsilon())));
	return (l1.p2 - l1.p1) * ts.x + l1.p1;
}

/* Plane-plane intersection yields a line. If the two planes are parallell,
* then no line of intersection exists. In that case, return a NaN-line consisting
* of a NaN-point and a NaN-direction */
template<typename fpt>
TLine<fpt, 3> intersection(TPlane<fpt, 3> const& p1, TPlane<fpt, 3> const& p2)
{
	auto const& n1 = p1.getNormal();
	auto const& n2 = p2.getNormal();

	static constexpr auto eps = std::numeric_limits<fpt>::epsilon();
	if(glm::areCollinear(n1, n2, eps)) {
		auto const NaN = std::numeric_limits<fpt>::quiet_NaN();
		auto const nandirection = glm::vec3{NaN, NaN, NaN};
		auto const nanpoint = Point(NaN, NaN, NaN);
		return {nanpoint, nandirection};
	}

	auto const direction = glm::cross(n1, n2);
	auto const pointOnLine = ::lowpoly3d::cramer(p1, p2, TPlane<fpt, 3>(direction, fpt(0)));

	// A vector that is orthogonal to n1 and n2 lies on both planes => a line
	// That vector is n1 cross n2.
	// Need only take an arbitrary point within both planes to determine the
	// line uniquely
	assert(glm::isNormalized(n1, eps));
	assert(glm::isNormalized(n2, eps));
	return {pointOnLine, direction};
}

/* Line-line intersection in 2D that returns the point of intersection.
 * Returns a point with two quiet NaN components if the two lines are parallel */
template<typename fpt>
TPoint<fpt, 2> intersection(TLine<fpt, 2> const& l1, TLine<fpt, 2> const& l2)
{
	if(l1.isParallelTo(l2)) {
		auto constexpr quietNaN = std::numeric_limits<fpt>::quiet_NaN();
		return TPoint<fpt, 2>(quietNaN, quietNaN);
	}

	/* C1 + d1*t2 = C2 + d2*t2
	*          C = d2*d2 -d1*t2
	* 
	* i.e
	* 
	* Cx = d2x*t2 - d1x*t1 = (-d1x, d2x) dot (t1, t2)
	* Cy = d2y*t2 - d1y*t1 = (-d1y, d2y) dot (t1, t2)
	* 
	* For this we have solveDotSystem2D */

	/* a . v = p */
	/* b . v = q */
	auto const timepoints = solveDotSystem2D(
		{-l1.getDirection().x, l2.getDirection().x},
		{-l1.getDirection().y, l2.getDirection().y},
		l1.getPoint().x - l2.getPoint().x,
		l1.getPoint().y - l2.getPoint().y
	);

	// TODO: Figure out good name for a function that returns time
	// of intersection instead of the intersection itself
	// then factor the functionality in this function to such a
	// function, and make this function a function of that function :)
	return l1.parametrization(timepoints.x);
}

template<typename fpt>
TLineSegment<fpt, 3> intersection(TTriangle<fpt, 3> const& t1, TTriangle<fpt, 3> const& t2) {
	// 1. Take the two planes of t1 and t2 and find their line-intersection.
	// 2. Then take that line-intersection and what segment of it is contained by t1, if any.
	//    It is sufficient to just pick one triangle out of t1 and t2.
	// TODO: Implement
	//auto const line = intersection(parallel(t1), parallel(t2));
	return TLineSegment<fpt, 3>({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
}

/* Solves a system of three linear equations by (naively) using Cramer's rule. */
template<typename fpt>
glm::vec<3, fpt, glm::qualifier::defaultp> cramer(
	glm::mat<3, 3, fpt, glm::qualifier::defaultp> const& A,
	     glm::vec<3, fpt, glm::qualifier::defaultp> const& b)
{
	auto const det = glm::determinant(A);
	if(std::abs(det) <= std::numeric_limits<fpt>::epsilon()) {
		std::ostringstream oss;
		oss << "Determinant is zero, division by zero follows ";
		oss << glm::to_string(glm::column(A, 0)) << ",";
		oss << glm::to_string(glm::column(A, 1)) << ",";
		oss << glm::to_string(glm::column(A, 2)) << ", ";
		oss << "A=" << glm::to_string(A);
		throw std::runtime_error(oss.str());
	}

	/* *** Cramers *** *

		a1 b1 c1  x    d1
		a2 b2 c2  y    d2
		a3 b3 c3  z    d3

		a1 b1 c1  x 0 0    d1 0 0
		a2 b2 c2  y 0 0    d2 0 0
		a3 b3 c3  z 0 0    d3 0 0

		a1 b1 c1  x 0 0    d1 b1 c1
		a2 b2 c2  y 1 0    d2 b2 c2
		a3 b3 c3  z 0 1    d3 b3 c3

		a1 b1 c1  x 1 0    d1 a1 c1
		a2 b2 c2  y 0 0    d2 a2 c2
		a3 b3 c3  z 0 1    d3 a3 c3

		a1 b1 c1  x 0 1    d1 -b1 a1
		a2 b2 c2  y -1 0   d2 -b2 a2
		a3 b3 c3  z 0 0    d3 -b3 a3

	* *** ******* *** */

	glm::tmat3x3<fpt> tmp = A;
	tmp = glm::column(tmp, 0, b);
	auto const detX = glm::determinant(tmp);

	tmp = glm::column(tmp, 1, glm::column(A, 0));
	auto const detY = glm::determinant(tmp);

	tmp = glm::column(tmp, 1, -glm::column(A, 1));
	tmp = glm::column(tmp, 2, glm::column(A, 0));
	auto const detZ = glm::determinant(tmp);

	return {detX / det, -detY / det, detZ / det};
}

/* Finds an intersection of three planes using Cramer's rule.
 * Precondition: n1, n2 and n3 are mutually non-parallel.
 * If this condition is not satisfied, than a division by zero will happen,
 * this is why this method is unsafe. */
template<typename fpt>
glm::vec<3, fpt, glm::qualifier::defaultp> cramer(TPlane<fpt, 3> p1, TPlane<fpt, 3> p2, TPlane<fpt, 3> p3)
{
	auto const& n1 = p1.getNormal();
	auto const& n2 = p2.getNormal();
	auto const& n3 = p3.getNormal();

	glm::tmat3x3<fpt> const A(n1, n2, n3);
	glm::tvec3<fpt> const b {-p1.getD(), -p2.getD(), -p3.getD()};

	/* A is constructed with n1 as first base, n2 as second base etc. However,
	* when solving using cramer we expect the matrix A and vector b to look like:
	* 
	* n1x n1y n1z   x = d1
	* n2x n2y n2z * y = d2
	* n3x n3y n3z   z = d3
	* 
	* instead of 
	* 
	* n1x n2x n3x   x = d1
	* n1y n2y n3y * y = d2
	* n1z n2z n3z   z = d3
	* 
	* so transpose it. */
	return cramer(glm::transpose(A), b);
}

// Explicit instantiations
template class Intersections< float, 1>;
template class Intersections< float, 2>;
template class Intersections< float, 3>;
template class Intersections<double, 1>;
template class Intersections<double, 2>;
template class Intersections<double, 3>;

} // End of namespace detail

/* These functions call into their template function. This is just to avoid code duplication of definitions */

TPoint<float, 2> intersection(TLineSegment<float, 2> const& l1, TLineSegment<float, 2> const& l2) { return detail::intersection(l1, l2); }
TPoint<double, 2> intersection(TLineSegment<double, 2> const& l1, TLineSegment<double, 2> const& l2) { return detail::intersection(l1, l2); }
TLine< float, 3> intersection(TPlane< float, 3> const& p1, TPlane< float, 3> const& p2) { return detail::intersection(p1, p2); }
TLine<double, 3> intersection(TPlane<double, 3> const& p1, TPlane<double, 3> const& p2) { return detail::intersection(p1, p2); }
TPoint< float, 2> intersection(TLine< float, 2> const& l1, TLine< float, 2> const& l2) { return detail::intersection(l1, l2); }
TPoint<double, 2> intersection(TLine<double, 2> const& l1, TLine<double, 2> const& l2) { return detail::intersection(l1, l2); }
TPoint<float, 3> intersection(TTriangle<float, 3> const& t, TLine<float, 3> const& l) { return detail::intersection(t, l); }
TPoint<double, 3> intersection(TTriangle<double, 3> const& t, TLine<double, 3> const& l) { return detail::intersection(t, l); }
TPoint<float, 3> intersection(TLine<float, 3> const& l, TTriangle<float, 3> const& t) { return detail::intersection(t, l); }
TPoint<double, 3> intersection(TLine<double, 3> const& l, TTriangle<double, 3> const& t) { return detail::intersection(t, l); }
TLineSegment<float, 3> intersection(TTriangle<float, 3> const& t1, TTriangle<float, 3> const& t2) { return detail::intersection(t1, t2); }
TLineSegment<double, 3> intersection(TTriangle<double, 3> const& t1, TTriangle<double, 3> const& t2) { return detail::intersection(t1, t2); }

glm::vec<3,  float, glm::qualifier::defaultp> cramer(glm::mat<3, 3,  float, glm::qualifier::defaultp> const& A, glm::vec<3,  float, glm::qualifier::defaultp> const& b) { return detail::cramer(A, b); }
glm::vec<3, double, glm::qualifier::defaultp> cramer(glm::mat<3, 3, double, glm::qualifier::defaultp> const& A, glm::vec<3, double, glm::qualifier::defaultp> const& b) { return detail::cramer(A, b); }
glm::vec<3,  float, glm::qualifier::defaultp> cramer(TPlane< float, 3> p1, TPlane< float, 3> p2, TPlane< float, 3> p3) { return detail::cramer(p1, p2, p3); }
glm::vec<3, double, glm::qualifier::defaultp> cramer(TPlane<double, 3> p1, TPlane<double, 3> p2, TPlane<double, 3> p3) { return detail::cramer(p1, p2, p3); };

} // End of namespace lowpoly3d