#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include <functional>

#include "geometric_primitives/point.hpp"

namespace lowpoly3d {

/* A non-solid representation of a cylinder aligned with its local z-axis,
 * extending half of its height above and below its local xy-plane */
template<typename floating_point_type>
class TCylinder {
public:
	using fpt = floating_point_type;

	TCylinder(fpt radius, fpt height) 
		: radius(radius), height(height) { }

	/* Returns a parametrization(rad, t) of this cylinder,
	 * with rad in [0, 2pi) and t in [0, 1) */
	[[nodiscard]] std::function<TPoint<fpt, 3>(fpt, fpt)> parametrization() const {
		return [r = radius, h = height](fpt theta, fpt t) -> TPoint<fpt, 3> {
			return {r*std::cos(theta), r*std::sin(theta), t*h - fpt(0.5) * h };
		};
	}
private:
	fpt radius, height;
};

using Cylinder = TCylinder<double>;
using Cylinderd = TCylinder<double>;
using Cylinderf = TCylinder<float>;

} // End of namespace lowpoly3d

#endif // CYLINDER_HPP