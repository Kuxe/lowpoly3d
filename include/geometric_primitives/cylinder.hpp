#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "geometric_primitives/linesegment.hpp"

#include "glm/gtx/rotate_vector.hpp"

#include <functional>

namespace lowpoly3d {

/* A non-solid cylinder represented as a line-segment with a radius */
template<typename floating_point_type, std::size_t dim>
class TCylinder {
public:
	using fpt = floating_point_type;
	using point_type = TPoint<fpt, dim>;
	using linesegment_type = TLineSegment<fpt, dim>;

	TCylinder(linesegment_type const& centerline, fpt radius) 
		: centerline(centerline), radius(radius) { }

	/* Returns a parametrization(rad, t) of this cylinder,
	 * with rad in [0, 2pi) and t in [0, 1).
	 * WARNING: Completely untested as of 2022-02-21 (does glm::orientation do what I think it does, i.e create a rotation matrix that aligns two vectors?)
	 */
	[[nodiscard]] std::function<TPoint<fpt, 3>(fpt, fpt)> parametrization_surface() const {
		auto const rot = glm::orientation(glm::normalize(centerline.p2 - centerline.p1), glm::vec<dim, fpt>(fpt(0), fpt(1), fpt(0)));

		return [
			rot = glm::orientation(glm::normalize(centerline.p2 - centerline.p1), glm::vec<dim, fpt>(fpt(0), fpt(1), fpt(0))),
			local_parametrization = parametrization_surface_local()
		] (fpt theta, fpt t) -> TPoint<fpt, 3> {
			return rot * local_parametrization(theta, t);
		};
	}

	/* Returns a parametrization(t) of this cylinder's centerline */
	std::function<TPoint<fpt, dim>(fpt)> parametrization_centerline() const
	{
		return centerline.parametrization();
	}

	fpt getRadius() const { return radius; }
	fpt getHeight() const { return centerline.length(); }

	linesegment_type const& getCenterline() const { return centerline; }

private:
	/* Returns a parametrization(rad, t) of this cylinder in ,
	 * with rad in [0, 2pi) and t in [0, 1) */
	std::function<TPoint<fpt, 3>(fpt, fpt)> parametrization_surface_local() const
	{
		return [r = radius, h = centerline.length()](fpt theta, fpt t) -> TPoint<fpt, 3> {
			return {r*std::cos(theta), t*h, r*std::sin(theta) };
		};
	}

	linesegment_type centerline;
	fpt radius;
};

using Cylinderd = TCylinder<double, 3>;
using Cylinderf = TCylinder<float, 3>;
using Cylinder = Cylinderf;

template<typename floating_point_type, std::size_t dim>
std::ostream& operator<<(std::ostream& os, TCylinder<floating_point_type, dim> const& cylinder) {
	os << "(centerline=" << cylinder.getCenterline() << ",radius=" << cylinder.getRadius() << ")";
	return os;
}

} // End of namespace lowpoly3d

#endif // CYLINDER_HPP