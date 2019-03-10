#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <algorithm>
#include <cstddef> // std::size_t
#include <functional> // std::function

#include "geometric_primitives/direction.hpp"
#include "geometric_primitives/line.hpp"
#include "geometric_primitives/point.hpp"
#include "geometric_primitives/plane.hpp"

#include <glm/gtx/string_cast.hpp> // glm::to_string

namespace lowpoly3d {

template<typename floating_point_type, std::size_t dimension>
struct TTriangle;

namespace detail {

/******************************************************************************************/
/* Specialize the TTriangle methods method in terms of partially specialized free functions */
/******************************************************************************************/

template<typename floating_point_type, std::size_t dimension>
struct TriangleArea {};

template<typename floating_point_type>
struct TriangleArea<floating_point_type, 3> {
	floating_point_type operator()(
		TPoint<floating_point_type, 3> const& p1,
		TPoint<floating_point_type, 3> const& p2,
		TPoint<floating_point_type, 3> const& p3) const
	{
		return floating_point_type(0.5) * glm::length(glm::cross(p2 - p1, p3 - p1));
	}
};

template<typename floating_point_type>
struct TriangleArea<floating_point_type, 2> {
	floating_point_type operator()(
		TPoint<floating_point_type, 2> const& p1,
		TPoint<floating_point_type, 2> const& p2,
		TPoint<floating_point_type, 2> const& p3) const
	{
		auto const side1 = p2 - p1;
		auto const side2 = p3 - p1;
		return floating_point_type(0.5) * std::abs((side1.x * side2.y) - (side1.y * side2.x));
	}
};


template<typename floating_point_type, std::size_t dimension>
struct TriangleContains {};

template<typename floating_point_type>
struct TriangleContains<floating_point_type, 3> {
	floating_point_type operator()(TTriangle<floating_point_type, 3> const& triangle, TPoint<floating_point_type, 3> const& point) const
	{
		using tri_type = TTriangle<floating_point_type, 3>;
		auto const tri1 = tri_type { point, triangle.p1, triangle.p2 };
		auto const tri2 = tri_type { point, triangle.p1, triangle.p3 };
		auto const tri3 = tri_type { point, triangle.p2, triangle.p3 };
		return std::abs(tri1.area() + tri2.area() + tri3.area() - triangle.area() <= std::numeric_limits<floating_point_type>::epsilon());
	}
};

template<typename floating_point_type>
struct TriangleContains<floating_point_type, 2> {
	floating_point_type operator()(TTriangle<floating_point_type, 2> const& triangle, TPoint<floating_point_type, 2> const& point) const
	{
		using line_type = TLine<floating_point_type, 2>;

		std::array<line_type const, 3> const lines {
			line_type{triangle.p1, triangle.p2 - triangle.p1},
			line_type{triangle.p2, triangle.p3 - triangle.p2},
			line_type{triangle.p3, triangle.p1 - triangle.p3}
		};

		// TODO: Maybe create class "ConvexPolygon" that has a "contains" method
		// which implements this algorithm below? Then we could just instantiate
		// a ConvexPolygon object here and call "contains" directly on it instead.
		return std::all_of(lines.cbegin(), lines.cend(), [&point](line_type const& line) {
			return line.above(point);
		});
	}
};

} // End of namespace detail

template<typename floating_point_type, std::size_t dimension>
struct TTriangle {
	using point_type = TPoint<floating_point_type, dimension>;
	using points_type = std::array<point_type, 3>;
	using iterator = typename points_type::iterator;
	using const_iterator = typename points_type::const_iterator;
	using reverse_iterator = typename points_type::reverse_iterator;
	using const_reverse_iterator = typename points_type::const_reverse_iterator;

	union {
		struct { point_type p1, p2, p3; };
		points_type points;
	};

	static_assert(sizeof(points) == sizeof(p1) + sizeof(p2) + sizeof(p3));

	TTriangle(points_type const& points)
		: points(points) { }

	TTriangle(point_type const& p1, point_type const& p2, point_type const& p3)
		: points({p1, p2, p3}) { }

	TTriangle(TTriangle<floating_point_type, dimension> const&) = default;
	TTriangle(TTriangle<floating_point_type, dimension>&&) = default;

	iterator begin() noexcept { return points.begin(); }
	iterator end() noexcept { return points.end(); }
	const_iterator cbegin() const noexcept { return points.cbegin(); }
	const_iterator cend() const noexcept { return points.cend(); }
	reverse_iterator rbegin() noexcept { return points.rbegin(); }
	reverse_iterator rend() noexcept { return points.rend(); }
	const_reverse_iterator crbegin() const noexcept { return points.crbegin(); }
	const_reverse_iterator crend() const noexcept { return points.crend(); }

	// Returns a plane that is paralell to this triangle
	TPlane<floating_point_type, dimension> parallel() const {
		return { p1, glm::cross(p1 - p2, p1 - p3) };
	}

	// Returns the i:th point of this triangle
	point_type const& operator[](std::size_t i) const {
		return points[i];
	}

	// Transforms all vertices of this triangle given a transformation m
	TTriangle<floating_point_type, dimension> transform(glm::mat<dimension, dimension, floating_point_type> const& m) const {
		points_type transformedPoints;
		std::transform(std::begin(points), std::end(points), std::begin(transformedPoints), [&m] (auto const& vertex) {
			return m * vertex;
		});
		return transformedPoints;
	}

	// Transforms all vertices of this triangle given a homogenous transformation m
	TTriangle<floating_point_type, dimension> transform(glm::mat<dimension+1, dimension+1, floating_point_type> const& m) const {
		points_type transformedPoints;
		std::transform(cbegin(), cend(), std::begin(transformedPoints), [&m] (auto const& vertex) {
			auto const v = glm::vec<dimension+1, floating_point_type>(vertex, 1);
			return glm::vec<dimension, floating_point_type>(m * v);
		});
		return {transformedPoints};
	}

	// Returns a function in three barycentric coordinates for this triangle
	std::function<point_type(floating_point_type, floating_point_type, floating_point_type)> getBarycentricParametrization() const {
		return [=](floating_point_type t1, floating_point_type t2, floating_point_type t3) {
			return t1*p1 + t2*p2 + t3*p3;
		};
	}

	// Returns the barycentric coordinates for the given point
	std::tuple<floating_point_type, floating_point_type, floating_point_type> getBarycentricCoordinates(point_type const& point) const {
		// TODO: IMplement. This can be used to determine if a point is interior of this triangle (atleast in 2D, how does it work for 3D?)
		return {0, 0, 0};
	}

	// Returns the area of this triangle
	floating_point_type area() const {
		return detail::TriangleArea<floating_point_type, dimension>()(p1, p2, p3);
	}

	// Returns the normal of this triangle, with vertices winding CCW about the normal
	TDirection<floating_point_type> normal() const {
		return {glm::cross(p2 - p1, p3 - p1)};
	}

	// Projects this triangle onto given plane
	TTriangle<floating_point_type, dimension> project(TPlane<floating_point_type, 3> const& plane) const {
		return {plane.project(p1), plane.project(p2), plane.project(p3)};
	}

	// Projects this triangle onto given plane and returns a point expressed in a local coordinate system of the plane
	TTriangle<floating_point_type, 2> projectIntoLocal(TPlane<floating_point_type, 3> const& plane) const {
		return {plane.projectIntoLocal(p1), plane.projectIntoLocal(p2), plane.projectIntoLocal(p3)};
	}

	// Returns true if this 2D triangle contains the given 2D point
	bool contains(TPoint<floating_point_type, dimension> const& point) const {
		return detail::TriangleContains<floating_point_type, dimension>()(*this, point);
	}

};

template<typename floating_point_type, std::size_t dimension>
std::ostream& operator<<(std::ostream& os, TTriangle<floating_point_type, dimension> const& triangle) {
	os << "(" << glm::to_string(triangle.p1) << "," << glm::to_string(triangle.p2) << "," << glm::to_string(triangle.p3) << ")";
	return os;
}

using Triangle = TTriangle<float, 3>;

} // End of namespace lowpoly3d

#endif // TRIANGLE_HPP