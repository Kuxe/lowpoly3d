#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <cstddef> // std::size_t

#include "geometric_primitives/point.hpp"
#include "geometric_primitives/plane.hpp"

namespace lowpoly3d {

template<typename value_type, std::size_t dimension>
struct TTriangle {
	using point_type = TPoint<value_type, dimension>;

	union {
		point_type p1, p2, p3;
		std::array<point_type, 3> points;
	};

	TTriangle(point_type const& p1, point_type const& p2, point_type const& p3)
		: points({p1, p2, p3}) { }

	TTriangle(point_type const& points)
		: points(points) { }

	// Returns a plane that is paralell to this triangle
	Plane parallel() const {
		return { p1, glm::cross(p1 - p2, p1 - p3) };
	}

	// Returns the i:th point of this triangle
	point_type const& operator[](std::size_t i) const {
		return points[i];
	}
};

using Triangle = TTriangle<float, 3>;

} // End of namespace lowpoly3d

#endif // TRIANGLE_HPP