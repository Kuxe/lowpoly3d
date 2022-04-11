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

// Returns true if p1 and p2 are componentwise almost equal with ulp=2
template<typename value_type, auto dimension>
bool almostEqual(
	const TPoint<value_type, dimension>& p1,
	const TPoint<value_type, dimension>& p2,
	value_type tolerance) {

	// Source: https://www.reidatcheson.com/floating%20point/comparison/2019/03/20/floating-point-comparison.html
	auto almostEqual = [](value_type x, value_type y, value_type tolerance) {
		auto const smallest = std::min(std::abs(x), std::abs(y));
		return (smallest == value_type(0) && std::abs(x-y) < tolerance)
			|| std::abs(x-y) / std::max(std::numeric_limits<value_type>::min(), smallest) < tolerance;
	};
	return
		almostEqual(p1.x, p2.x, tolerance) &&
		almostEqual(p1.y, p2.y, tolerance) &&
		almostEqual(p1.z, p2.z, tolerance);
}

using Pointf = TPoint<float, 3>;
using Pointd = TPoint<double, 3>;
using Point = Pointf;
using Point2f = TPoint<float, 2>;
using Point2d = TPoint<double, 2>;
using Point2 = Point2f;

} // End of namespace lowpoly3d

#endif // POINT_HPP