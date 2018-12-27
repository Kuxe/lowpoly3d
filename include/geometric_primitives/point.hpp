#ifndef POINT_HPP
#define POINT_HPP

#include <array>
#include <numeric>
#include <glm/glm.hpp>

#include "utils/strong_type.hpp"

namespace lowpoly3d {

struct strong_point_type;
template<typename value_type, std::size_t dimension>
using TPoint = glm::vec<dimension, value_type>;// TODO: Use this instead StrongType<glm::vec<dimension, value_type>, strong_point_type>;

template<typename value_type, std::size_t dimension>
TPoint<value_type, dimension> midpoint(
	const TPoint<value_type, dimension>& point1,
	const TPoint<value_type, dimension>& point2) {

	return value_type(0.5)*(point1 + point2);
};

// Returns true if ||p1-p2|| <= eps
template<typename value_type, std::size_t dimension>
bool almostEqual(
	const TPoint<value_type, dimension>& p1,
	const TPoint<value_type, dimension>& p2,
	value_type eps = std::numeric_limits<value_type>::epsilon()) {

	return glm::distance(p1, p2) <= eps;
}

using Pointf = TPoint<float, 3>;
using Pointd = TPoint<double, 3>;
using Point = Pointf;

} // End of namespace lowpoly3d

#endif // POINT_HPP