#ifndef LINE_HPP
#define LINE_HPP

#include <cstddef> // std::size_t
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
	const TLine<floating_point_type, dimension>& l1,
	const TLine<floating_point_type, dimension>& l2) {
	
	if(!areParallel<floating_point_type, 3>(l1.getDirection(), l2.getDirection())) return false;

	// TODO: They are parallel but must also ensure that they intersect same point
	return false;
}

using Line = TLine<float, 3>;

} // End of namespace lowpoly3d

#endif // LINE_HPP