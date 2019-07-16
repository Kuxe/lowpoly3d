#include "geometric_primitives/intersections.hpp"
#include "geometric_primitives/intersects.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/epsilon.hpp>

#include "geometric_primitives/linesegment.hpp"
#include "geometric_primitives/plane.hpp"
#include "geometric_primitives/point.hpp"
#include "geometric_primitives/triangle.hpp"

namespace lowpoly3d {
namespace detail {

template<typename fpt>
bool intersects(TLineSegment<fpt, 2> const& l1, TLineSegment<fpt, 2> const& l2)
{
	return !glm::any(glm::isnan(intersection(l1, l2)));
}

template<typename fpt, std::size_t dim>
bool intersects(TLineSegment<fpt, dim> const& segment, TPlane<fpt, dim> const& plane)
{
	return
		!(plane.above(segment.p1) && plane.above(segment.p2)) || 
		!(plane.below(segment.p1) && plane.below(segment.p2));
}

template<typename fpt, std::size_t dim>
bool Intersects<fpt, dim>::intersects(TLineSegment<fpt, dim> l,	TPoint<fpt, dim> p) {
	// Translate the line s.t l.start is at origo
	l.end -= l.start;
	p -= l.start;

	if(dot(p, p) == 0 || p == l.end) return true;

	// Make p and l.end unit vectors. Check if plen is greater than endlen,
	// then p is beyond the segment.
	const auto plen = length(p);
	const auto endlen = length(l.end);
	if(plen > endlen) return false;

	// Normalize p and l.end, if their dot is unity then they are perpendicular.
	return std::abs(dot(p / length(p), l.end / length(l.end)) - 1.0) <=
		std::numeric_limits<double>::epsilon();
}

template<typename fpt, std::size_t dim>
bool intersects(TLineSegment<fpt, dim> const& segment, TTriangle<fpt, dim> const& triangle)
{
	/* (naive but intuitive) Algorithm idea:
	*
	* 1. The segment must at least intersect with paralell plane to the triangle
	* if they should intersect at all. So check if the LineSegment intersects
	* the plane parallel to the triangle.
	* 
	* 2. Check if LineSegment lies on the plane of the triangle, if so,
	* check if both points of the LineSegment are contained by the triangle
	* by projecting the triangle and the LineSegment locally into plane parallel to triangle
	* 
	* 3. Project triangle onto plane that is given by a normal parallel
	* the segment and p1 of LineSegment --- the LineSegment becomes the point
	* p1 in that plane.
	* 
	* 4. If the projected triangle contains p1, then the LineSegment
	* intersects the triangle. */

	// 1.
	auto const trianglePlane = triangle.parallel();
	if(!intersects(segment, trianglePlane)) return false;

	// 2
	if(trianglePlane.contains(segment.p1) && trianglePlane.contains(segment.p2)) {
		auto const projectedTriangle = triangle.projectIntoLocal(trianglePlane);
		auto const projectedSegmentPoint1 = trianglePlane.projectIntoLocal(segment.p1);
		auto const projectedSegmentPoint2 = trianglePlane.projectIntoLocal(segment.p2);
		return projectedTriangle.contains(projectedSegmentPoint1) || projectedTriangle.contains(projectedSegmentPoint2);
	}

	// 3.
	// Create plane parallell to the line segment
	auto const plane = TPlane<fpt, 3>{segment.p1, segment.p2 - segment.p1};
	auto const projectedTriangle = triangle.projectIntoLocal(plane);
	auto const projectedSegmentPoint = plane.projectIntoLocal(segment.p1);

	// The two points should be projected into the same point since, see step 2 above
	assert(glm::all(
		glm::epsilonEqual(projectedSegmentPoint,
		plane.projectIntoLocal(segment.p2),
		std::numeric_limits<fpt>::epsilon())
	));

	// 4.
	return projectedTriangle.contains(projectedSegmentPoint);
}


template<typename fpt>
bool intersects(TTriangle<fpt, 3> const& t, TPoint<fpt, 3> const& p) {
	return t.contains(p);
}

template<typename fpt>
bool intersects(TTriangle<fpt, 3> const& t1, TTriangle<fpt, 3> const& t2)
{
	/* A triangle T1 intersects another triangle T2 iff any line segment of T1 intersect T2 (or vice versa)
	* But I don't really need to check vice-versa, just make sure that we test the line segments of
	* the smaller triangle against the larger triangle --- this way, we're never at risk of the following situation:
	*
	* |\
	* | \
	* |  \
	* |   \
	* | |\ \
	* | |_\ \
	* |______\
	* 
	* where one small triangle is contained by the larger triangle and we would've been testing
	* the larger triangles line segments against the smaller triangle, which wouldt work... but as stated above,
	* we're good as long as we compare smaller triangle line segments against the larger triangle. */
	if(t1.area() < t2.area()) {
		auto const segment1 = TLineSegment<fpt, 3>(t1.p1, t1.p2);
		auto const segment2 = TLineSegment<fpt, 3>(t1.p2, t1.p3);
		auto const segment3 = TLineSegment<fpt, 3>(t1.p3, t1.p1);
		return
			::lowpoly3d::intersects(segment1, t2) ||
			::lowpoly3d::intersects(segment2, t2) || 
			::lowpoly3d::intersects(segment3, t2);
	}
	else {
		auto const segment1 = TLineSegment<fpt, 3>(t2.p1, t2.p2);
		auto const segment2 = TLineSegment<fpt, 3>(t2.p2, t2.p3);
		auto const segment3 = TLineSegment<fpt, 3>(t2.p3, t2.p1);
		return
			::lowpoly3d::intersects(segment1, t1) ||
			::lowpoly3d::intersects(segment2, t1) || 
			::lowpoly3d::intersects(segment3, t1);
	}
}

// Explicit instantiation definitions
template class Intersects< float, 1>;
template class Intersects< float, 2>;
template class Intersects< float, 3>;
template class Intersects<double, 1>;
template class Intersects<double, 2>;
template class Intersects<double, 3>;

} // End of namespace detail

/* These functions call into their template function. This is just to avoid code duplication of definitions */

bool intersects(TLineSegment< float, 2> const& ls1, TLineSegment< float, 2> const& ls2) { return detail::intersects(ls1, ls2); }
bool intersects(TLineSegment<double, 2> const& ls1, TLineSegment<double, 2> const& ls2) { return detail::intersects(ls1, ls2); }
bool intersects(TLineSegment< float, 3> const& ls,        TPlane< float, 3> const& p)   { return detail::intersects(ls, p); }
bool intersects(TLineSegment<double, 3> const& ls,        TPlane<double, 3> const& p)   { return detail::intersects(ls, p); }
bool intersects(TLineSegment< float, 3> const& ls,     TTriangle< float, 3> const& t)   { return detail::intersects(ls, t); }
bool intersects(TLineSegment<double, 3> const& ls,     TTriangle<double, 3> const& t)   { return detail::intersects(ls, t); }
bool intersects(   TTriangle< float, 3> const& t,         TPoint< float, 3> const& p)   { return detail::intersects(t, p); }
bool intersects(   TTriangle<double, 3> const& t,         TPoint<double, 3> const& p)   { return detail::intersects(t, p); }
bool intersects(   TTriangle< float, 3> const& t1,     TTriangle< float, 3> const& t2)  { return detail::intersects(t1, t2); }
bool intersects(   TTriangle<double, 3> const& t1,     TTriangle<double, 3> const& t2)  { return detail::intersects(t1, t2); }

// Commutes
bool intersects(   TPlane< float, 3> const& p, TLineSegment< float, 3> const& ls) { return detail::intersects(ls, p); }
bool intersects(   TPlane<double, 3> const& p, TLineSegment<double, 3> const& ls) { return detail::intersects(ls, p); }
bool intersects(TTriangle< float, 3> const& t, TLineSegment< float, 3> const& ls) { return detail::intersects(ls, t); }
bool intersects(TTriangle<double, 3> const& t, TLineSegment<double, 3> const& ls) { return detail::intersects(ls, t); }
bool intersects(   TPoint< float, 3> const& p,    TTriangle< float, 3> const& t)  { return detail::intersects(t, p); }
bool intersects(   TPoint<double, 3> const& p,    TTriangle<double, 3> const& t)  { return detail::intersects(t, p); }

} // End of namespace lowpoly3d