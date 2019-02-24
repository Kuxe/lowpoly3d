#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <algorithm>
#include <cstddef> // std::size_t
#include <functional> // std::function

#include "geometric_primitives/direction.hpp"
#include "geometric_primitives/point.hpp"
#include "geometric_primitives/plane.hpp"

#include <glm/gtx/string_cast.hpp> // glm::to_string

namespace lowpoly3d {

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
		return floating_point_type(0.5) * glm::length(glm::cross(p2 - p1, p3 - p1));
	}

	// Returns the normal of this triangle, with vertices winding CCW about the normal
	TDirection<floating_point_type> normal() const {
		return {glm::cross(p2 - p1, p3 - p1)};
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