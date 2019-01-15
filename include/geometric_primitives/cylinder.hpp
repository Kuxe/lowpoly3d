#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "geometric_primitives/point.hpp"

namespace lowpoly3d {

/* A non-solid representation of a cylinder aligned with its local z-axis,
 * extending half of its height above and below its local xy-plane */
template<typename floating_point_type>
class TCylinder {
public:
	TCylinder(floating_point_type radius, floating_point_type height) 
		: r(radius), h(height) { }

	/* Returns a parametrization(rad, t) of this cylinder,
	 * with rad in [0, 2pi) and t in [0, 1) */
	[[nodiscard]] std::function<TPoint<floating_point_type, 3>(floating_point_type, floating_point_type)> parametrization() const {
		return [radius, height](floating_point_type theta, t) -> TPoint<floating_point_type, 3> {
			return {radius*std::cos(theta), radius*std::sin(theta), t*height - floating_point_type(0.5) * height };
		};
	}
private:
	floating_point_type radius, height;
};

using Cylinder = TCylinder<double>;
using Cylinderd = TCylinder<double>;
using Cylinderf = TCylinder<float>;

} // End of namespace lowpoly3d

#endif // CYLINDER_HPP