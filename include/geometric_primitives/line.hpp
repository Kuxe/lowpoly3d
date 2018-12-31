#ifndef LINE_HPP
#define LINE_HPP

#include <cstddef> // std::size_t

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <type_traits>

#include "utils/glmutils.hpp"

#include "geometric_primitives/point.hpp"

namespace lowpoly3d {

/* TLine is a generic line in d-dimensions, represented by a unit direction
 * vector d and a point intersecting the line */
template<typename floating_point_type, std::size_t dimension>
struct TLine {
	using line_type = TLine<floating_point_type, dimension>;
	using point_type = TPoint<floating_point_type, dimension>;
	using vec_type = glm::vec<dimension, floating_point_type>;

	constexpr TLine(const point_type& p, const vec_type& d) : p(p), d(glm::normalize(d)) { }

	[[nodiscard]] const vec_type& getDirection() const { return d; }
	[[nodiscard]] const point_type& getPoint() const { return p; }

	// Return true if "point" lies on this line
	[[nodiscard]] bool contains(const point_type& point) const {
		const auto crossed = glm::cross(point - p, d);
		return glm::dot(crossed, crossed) <= std::numeric_limits<floating_point_type>::epsilon();
	}

	// Returns true if "direction" is parallel to this line
	[[nodiscard]] bool isParallelTo(const vec_type& direction) const {
		return
			glm::length(glm::cross(getDirection(), direction)) <=
			std::numeric_limits<floating_point_type>::epsilon();
	}

	// Returns true if "line" is parallel to this line
	[[nodiscard]] bool isParallelTo(const line_type& line) const {
		return isParallelTo(line.getDirection());
	}

private:
	point_type p;
	vec_type d;
};

// Returns true if l1 and l2 represent the same undirected line
template<typename floating_point_type, std::size_t dimension>
bool almostEqual(
	TLine<floating_point_type, dimension> l1,
	TLine<floating_point_type, dimension> l2) {
	return l1.isParallelTo(l2) && l1.contains(l2.getPoint());
}

using Line = TLine<float, 3>;

template<typename floating_point_type, std::size_t dimension>
std::ostream& operator<<(std::ostream& out, const TLine<floating_point_type, dimension>& line) {
	const auto& p = line.getPoint();
	const auto& d = line.getDirection();
	out << "(p=" << glm::to_string(p).c_str() << ", d=" << glm::to_string(d).c_str() << ")";
	return out;
}

} // End of namespace lowpoly3d

#endif // LINE_HPP