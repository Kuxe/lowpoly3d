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

	constexpr TLine(const vec_type& d, const point_type& p) : d(d), p(p) { }

	const vec_type& getDirection() const { return d; }
	const point_type& getPoint() const { return p; }

	// Return true if "point" lies on this line
	bool contains(const point_type& point) const {
		const auto crossed = glm::cross(point - p, d);
		return glm::dot(crossed, crossed) <= std::numeric_limits<floating_point_type>::epsilon();
	}

private:
	vec_type d;
	point_type p;
};

// Returns true if l1 and l2 represent the same undirected line
template<typename floating_point_type, std::size_t dimension>
bool almostEqual(
	TLine<floating_point_type, dimension> l1,
	TLine<floating_point_type, dimension> l2) {
	
	// Translate lines to origin and create positioned difference vectors at
	// both points. If these positioned difference vectors are parallel, then
	// l1 and l2 are parallel. Check if parallel using cross product.
	const auto crossed = glm::cross(l1.getDirection(), l2.getDirection() - l2.getPoint() - l1.getPoint());
	return glm::dot(crossed, crossed) <= std::numeric_limits<floating_point_type>::epsilon();
}

using Line = TLine<float, 3>;

template<typename floating_point_type, std::size_t dimension>
std::ostream& operator<<(std::ostream& out, const TLine<floating_point_type, dimension>& line) {
	const auto& d = line.getDirection();
	const auto& p = line.getPoint();
	out << "(d=" << glm::to_string(d).c_str() << ", p=" << glm::to_string(p).c_str() << ")";
	return out;
}

} // End of namespace lowpoly3d

#endif // LINE_HPP