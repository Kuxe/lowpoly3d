#include <algorithm>
#include <cstddef> // std::size_t

#include "geometric_primitives/direction.hpp"
#include "geometric_primitives/intersections.hpp"
#include "geometric_primitives/line.hpp"
#include "geometric_primitives/point.hpp"
#include "geometric_primitives/plane.hpp"
#include "geometric_primitives/triangle.hpp"

#include <glm/gtx/string_cast.hpp> // glm::to_string

#include <sstream> // std::ostream

#include "utils/glm/vector_projection.hpp"
#include "utils/solve.hpp"

namespace lowpoly3d
{

namespace detail {

/******************************************************************************************/
/* Specialize the TTriangle methods method in terms of partially specialized free functions */
/******************************************************************************************/

template<typename fpt, std::size_t dim>
struct TriangleArea {};

template<typename fpt>
struct TriangleArea<fpt, 3> {
	fpt operator()(
		TPoint<fpt, 3> const& p1,
		TPoint<fpt, 3> const& p2,
		TPoint<fpt, 3> const& p3) const
	{
		return fpt(0.5) * glm::length(glm::cross(p2 - p1, p3 - p1));
	}
};

template<typename fpt>
struct TriangleArea<fpt, 2> {
	fpt operator()(
		TPoint<fpt, 2> const& p1,
		TPoint<fpt, 2> const& p2,
		TPoint<fpt, 2> const& p3) const
	{
		auto const side1 = p2 - p1;
		auto const side2 = p3 - p1;
		return fpt(0.5) * std::abs((side1.x * side2.y) - (side1.y * side2.x));
	}
};


template<typename fpt, std::size_t dim>
struct TriangleContains {};

template<typename fpt>
struct TriangleContains<fpt, 3> {
	fpt operator()(TTriangle<fpt, 3> const& triangle, TPoint<fpt, 3> const& point) const
	{
		using tri_type = TTriangle<fpt, 3>;
		auto const tri1 = tri_type { point, triangle.p1, triangle.p2 };
		auto const tri2 = tri_type { point, triangle.p1, triangle.p3 };
		auto const tri3 = tri_type { point, triangle.p2, triangle.p3 };
		// FIXME: This method will break when the triangle degenerates into a line segment,
		// and the point lies on the line formed from the line segment.
		return std::abs(tri1.area() + tri2.area() + tri3.area() - triangle.area() <= std::numeric_limits<fpt>::epsilon());
	}
};

template<typename fpt>
struct TriangleContains<fpt, 2> {
	fpt operator()(TTriangle<fpt, 2> const& triangle, TPoint<fpt, 2> const& point) const
	{
		auto const ts = solveScalingVecs(triangle.p3 - triangle.p1, triangle.p2 - triangle.p1, point - triangle.p1);
		auto const sumt = ts.x + ts.y;

		auto inRangeEps = [](
			fpt variable,
			fpt start,
			fpt end,
			fpt eps
		){
			assert(start <= end);
			assert(eps >= 0);
			return start - eps <= variable && variable <= end + eps; 
		};

		fpt const eps = std::numeric_limits<fpt>::epsilon();
		return
			inRangeEps(ts.x, 0, 1, eps) &&
			inRangeEps(ts.y, 0, 1, eps) &&
			inRangeEps(sumt, 0, 1, eps);
	}
};

template<typename fpt, std::size_t dim>
struct Circumcenter { }; // Not implemented for arbitrary dimension

template<typename fpt>
struct Circumcenter<fpt, 3> {
	using point_type = TPoint<fpt, 3>;

	point_type operator()(TTriangle<fpt, 3> const& triangle) const {
		/* To figure out whats going on here:
		 * Let A, B, C be a triangle.
		 * By convention, let all bisectors point inward.
		 * Draw two bisectors bisector(AB), bisector(CA) in the triangle
		 * The intersection of bisector(AB) with bisector(CA) is the circumcenter
		 * Project AC/2 onto bisector(AB). Denote this projection point as 'P'.
		 * Form AC/2 - P. Denote this vector as v.
		 * Project v onto -bisector(AB) but do a funny projection where the rejection vector is
		 * perpendicular to v instead of -bisector(AB). Note that this point is exactly the
		 * intersection of bisector(AB) and bisector(CA) i.e the circumcenter. Done. */
		auto const t1t0 = triangle[1] - triangle[0];
		auto t1t0InwardBisector = glm::cross(normal(triangle), t1t0);
		auto const t0t2midpoint = fpt(0.5)*(triangle[0] + triangle[2]);
		return projectOnto2(t0t2midpoint - projectOnto(t0t2midpoint, t1t0InwardBisector), -t1t0InwardBisector);
	}
};

} // End of namespace detail

template<typename fpt, std::size_t dim>
TTriangle<fpt, dim>::TTriangle(
	TTriangle<fpt, dim>::point_type const& p1,
	TTriangle<fpt, dim>::point_type const& p2,
	TTriangle<fpt, dim>::point_type const& p3)
	: points({p1, p2, p3}) { }

template<typename fpt, std::size_t dim>
TTriangle<fpt, dim>::TTriangle(TTriangle<fpt, dim>::points_type const& points)
	: points(points) { }

template<typename fpt, std::size_t dim>
TTriangle<fpt, dim>::TTriangle(TTriangle<fpt, dim> const&) = default;
template<typename fpt, std::size_t dim>
TTriangle<fpt, dim>::TTriangle(TTriangle<fpt, dim>&&) = default;

template<typename fpt, std::size_t dim> typename TTriangle<fpt, dim>::              iterator TTriangle<fpt, dim>::  begin()       noexcept { return points.  begin(); }
template<typename fpt, std::size_t dim> typename TTriangle<fpt, dim>::              iterator TTriangle<fpt, dim>::    end()       noexcept { return points.    end(); }
template<typename fpt, std::size_t dim> typename TTriangle<fpt, dim>::        const_iterator TTriangle<fpt, dim>:: cbegin() const noexcept { return points. cbegin(); }
template<typename fpt, std::size_t dim> typename TTriangle<fpt, dim>::        const_iterator TTriangle<fpt, dim>::   cend() const noexcept { return points.   cend(); }
template<typename fpt, std::size_t dim> typename TTriangle<fpt, dim>::      reverse_iterator TTriangle<fpt, dim>:: rbegin()       noexcept { return points. rbegin(); }
template<typename fpt, std::size_t dim> typename TTriangle<fpt, dim>::      reverse_iterator TTriangle<fpt, dim>::   rend()       noexcept { return points.   rend(); }
template<typename fpt, std::size_t dim> typename TTriangle<fpt, dim>::const_reverse_iterator TTriangle<fpt, dim>::crbegin() const noexcept { return points.crbegin(); }
template<typename fpt, std::size_t dim> typename TTriangle<fpt, dim>::const_reverse_iterator TTriangle<fpt, dim>::  crend() const noexcept { return points.  crend(); }

template<typename fpt, std::size_t dim>
typename TTriangle<fpt, dim>::point_type const& TTriangle<fpt, dim>::operator[](std::size_t i) const {
	return points[i];
}

template<typename fpt, std::size_t dim>
TTriangle<fpt, dim> TTriangle<fpt, dim>::transform(glm::mat<dim, dim, fpt> const& m) const {
	points_type transformedPoints;
	std::transform(std::begin(points), std::end(points), std::begin(transformedPoints), [&m] (auto const& vertex) {
		return m * vertex;
	});
	return transformedPoints;
}

template<typename fpt, std::size_t dim>
TTriangle<fpt, dim> TTriangle<fpt, dim>::transform(glm::mat<dim+1, dim+1, fpt> const& m) const {
	points_type transformedPoints;
	std::transform(cbegin(), cend(), std::begin(transformedPoints), [&m] (auto const& vertex) {
		auto const v = glm::vec<dim+1, fpt>(vertex, 1);
		return glm::vec<dim, fpt>(m * v);
	});
	return {transformedPoints};
}

template<typename fpt, std::size_t dim>
std::function<typename TTriangle<fpt, dim>::point_type(fpt, fpt, fpt)> TTriangle<fpt, dim>::getBarycentricParametrization() const {
	return [this](fpt t1, fpt t2, fpt t3) {
		return t1*p1 + t2*p2 + t3*p3;
	};
}

template<typename fpt, std::size_t dim>
fpt TTriangle<fpt, dim>::area() const {
	return detail::TriangleArea<fpt, dim>()(p1, p2, p3);
}

template<typename fpt, std::size_t dim>
bool TTriangle<fpt, dim>::contains(TPoint<fpt, dim> const& point) const {
	return detail::TriangleContains<fpt, dim>()(*this, point);
}

template<typename fpt, std::size_t dim>
bool TTriangle<fpt, dim>::degenerate() const {
	return area() <= std::numeric_limits<fpt>::epsilon();
}

template class TTriangle< float, 2>;
template class TTriangle< float, 3>;
template class TTriangle<double, 2>;
template class TTriangle<double, 3>;

template<typename fpt>
glm::vec<3, fpt> normal(TTriangle<fpt, 3> const& t) {
	return glm::normalize(glm::cross(t.p2 - t.p1, t.p3 - t.p1));
}

template glm::vec<3, float> normal(TTriangle< float, 3> const&);
template glm::vec<3, double> normal(TTriangle<double, 3> const&);

template<typename fpt, typename point_type = typename TTriangle<fpt, 2>::point_type>
TLine<fpt, 2> bisector2d_generic(
	point_type const& p0,
	point_type const& p1)
{
	auto const midpoint = (p0 + p1) / fpt(2);
	auto const diff = p1 - p0;
	auto const vector_orthogonal_to_edge = point_type(diff.y, -diff.x);
	auto const point_along_bisector = midpoint + vector_orthogonal_to_edge;
	return TLine<fpt, 2>(midpoint, point_along_bisector);
}

template<typename fpt>
TLine<fpt, 2> bisector_01(TTriangle<fpt, 2> const& triangle)
{
	return bisector2d_generic<fpt>(triangle.p1, triangle.p2);
}

template<typename fpt>
TLine<fpt, 2> bisector_12(TTriangle<fpt, 2> const& triangle)
{
	return bisector2d_generic<fpt>(triangle.p2, triangle.p3);
}

template<typename fpt>
TLine<fpt, 2> bisector_20(TTriangle<fpt, 2> const& triangle)
{
	return bisector2d_generic<fpt>(triangle.p3, triangle.p1);
}

template<typename fpt>
glm::vec<3, fpt> edge_normal_12(TTriangle<fpt, 3> const& triangle)
{
	return glm::normalize(glm::cross(triangle.p2 - triangle.p1, normal(triangle)));
}

template<typename fpt>
glm::vec<3, fpt>edge_normal_23(TTriangle<fpt, 3> const& triangle)
{
	return glm::normalize(glm::cross(triangle.p3 - triangle.p2, normal(triangle)));
}

template<typename fpt>
glm::vec<3, fpt> edge_normal_31(TTriangle<fpt, 3> const& triangle)
{
	return glm::normalize(glm::cross(triangle.p1 - triangle.p3, normal(triangle)));
}

template TLine<float, 2> bisector_01(TTriangle< float, 2> const&);
template TLine<double, 2> bisector_01(TTriangle<double, 2> const&);
template TLine<float, 2> bisector_12(TTriangle< float, 2> const&);
template TLine<double, 2> bisector_12(TTriangle<double, 2> const&);
template TLine<float, 2> bisector_20(TTriangle< float, 2> const&);
template TLine<double, 2> bisector_20(TTriangle<double, 2> const&);
template glm::vec<3, float> edge_normal_12(TTriangle< float, 3> const&);
template glm::vec<3, double> edge_normal_12(TTriangle<double, 3> const&);
template glm::vec<3, float> edge_normal_23(TTriangle< float, 3> const&);
template glm::vec<3, double> edge_normal_23(TTriangle<double, 3> const&);
template glm::vec<3, float> edge_normal_31(TTriangle< float, 3> const&);
template glm::vec<3, double> edge_normal_31(TTriangle<double, 3> const&);

template<typename fpt>
TPlane<fpt, 3> parallel(TTriangle<fpt, 3> const& t) {
	return { t.p1, glm::cross(t.p1 - t.p2, t.p1 - t.p3) };
}

template TPlane< float, 3> parallel(TTriangle< float, 3> const&);
template TPlane<double, 3> parallel(TTriangle<double, 3> const&);

template<typename fpt>
TTriangle<fpt, 2> project(TTriangle<fpt, 3> const& t, TPlane<fpt, 3> const& plane) {
	return {plane.project(t.p1), plane.project(t.p2), plane.project(t.p3)};
}

template TTriangle< float, 2> project(TTriangle< float, 3> const&, TPlane< float, 3> const& plane);
template TTriangle<double, 2> project(TTriangle<double, 3> const&, TPlane<double, 3> const& plane);

template<typename fpt>
TTriangle<fpt, 2> projectIntoLocal(TTriangle<fpt, 3> const& t, TPlane<fpt, 3> const& plane) {
	return {plane.projectIntoLocal(t.p1), plane.projectIntoLocal(t.p2), plane.projectIntoLocal(t.p3)};
}

template TTriangle< float, 2> projectIntoLocal(TTriangle< float, 3> const&, TPlane< float, 3> const& plane);
template TTriangle<double, 2> projectIntoLocal(TTriangle<double, 3> const&, TPlane<double, 3> const& plane);

template<typename fpt>
TPoint<fpt, 3> circumcenter(TTriangle<fpt, 3> const& triangle) {
	return detail::Circumcenter<fpt, 3>()(triangle);
}

template TPoint< float, 3> circumcenter(TTriangle< float, 3> const&);
template TPoint<double, 3> circumcenter(TTriangle<double, 3> const&);

template<typename fpt, std::size_t dim>
std::ostream& operator<<(std::ostream& os, TTriangle<fpt, dim> const& triangle) {
	os << "(" << glm::to_string(triangle.p1) << "," << glm::to_string(triangle.p2) << "," << glm::to_string(triangle.p3) << ")";
	return os;
}

template std::ostream& operator<<(std::ostream& os, TTriangle< float, 2> const& triangle);
template std::ostream& operator<<(std::ostream& os, TTriangle< float, 3> const& triangle);
template std::ostream& operator<<(std::ostream& os, TTriangle<double, 2> const& triangle);
template std::ostream& operator<<(std::ostream& os, TTriangle<double, 3> const& triangle);

} // End of namespace lowpoly3d;