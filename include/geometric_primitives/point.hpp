#ifndef POINT_HPP
#define POINT_HPP

#include <array>
#include <glm/glm.hpp>

namespace lowpoly3d {

template<typename value_type, std::size_t dimension>
using TPoint = glm::vec<dimension, value_type>;

template<typename value_type, std::size_t dimension>
TPoint<value_type, dimension> midpoint(
	const TPoint<value_type, dimension>& point1,
	const TPoint<value_type, dimension>& point2) {

	return value_type(0.5)*(point1 + point2);
};

using Pointf = TPoint<float, 3>;
using Pointd = TPoint<double, 3>;
using Point = Pointf;

} // End of namespace lowpoly3d

#endif // POINT_HPP