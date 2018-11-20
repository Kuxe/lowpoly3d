#ifndef LINE_HPP
#define LINE_HPP

#include <cstddef> // std::size_t
#include <type_traits>

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

private:
	vec_type d;
	point_type p;
};

using Line = TLine<float, 3>;

} // End of namespace lowpoly3d

#endif // LINE_HPP