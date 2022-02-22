#ifndef ARROW_HPP
#define ARROW_HPP

#include "geometric_primitives/cone.hpp"
#include "geometric_primitives/cylinder.hpp"

#include "glm/gtx/string_cast.hpp"

#include <ostream>

namespace lowpoly3d {

/* An Arrow, expressed as a Cylinder joined by a Cone.
 * The cylinder defines the arrow and the cone is a function of the cylinder. */
template<typename floating_point_type, std::size_t dim>
class TArrow final {
public:

	using cylinder_type = TCylinder<floating_point_type, dim>;
	using cone_type = TCone<floating_point_type, dim>;

	// Create an arrow that is tightly bounded by a cylinder 'bound'
	// This allows for intuitive constructor of arrows in terms of a cylinder,
	// where a bound=(p1=(0, 1, 0), p2=(0, 0, 0), r=0.2) generates an arrow
	// with tip at origo and base at (0, 1, 0), whose "cone-head" has radius 0.2.
	TArrow(cylinder_type&& bound)
		: cylinder(createCylinder(bound))
		, cone(createCone(bound))
	{}

	auto const& getCylinder() const { return cylinder; }
	auto const& getCone() const { return cone; }

	floating_point_type length() const { return cone.getHeight() + cylinder.getHeight(); }

private:


	static cylinder_type createCylinder(cylinder_type const& bound)
	{
		auto const& centerline = bound.getCenterline();
		auto const p1 = centerline.p1;
		auto const p2 = p1 + (3.0f / 5.0f) * glm::normalize(centerline.p2 - centerline.p1);
		auto const radius = 1.0f/3.0f * bound.getRadius();
		return cylinder_type{{p1, p2}, radius};
	}

	static cone_type createCone(cylinder_type const& bound)
	{
		auto const& centerline = bound.getCenterline();
		auto const p1 = centerline.p1 + (3.0f / 5.0f) * glm::normalize(centerline.p2 - centerline.p1);
		auto const p2 = p1 + (2.0f / 5.0f) * glm::normalize(centerline.p2 - centerline.p1);
		auto const radius = bound.getRadius();
		return cone_type{{p1, p2}, radius};
	}

	cylinder_type cylinder;
	cone_type cone;
};

using Arrow2d = TArrow<double, 2>;
using Arrow2f = TArrow<float, 2>;
using Arrowd = TArrow<double, 3>;
using Arrowf = TArrow<float, 3>;
using Arrow = Arrowf;

template<typename floating_point_type, std::size_t dim>
std::ostream& operator<<(std::ostream& os, TArrow<floating_point_type, dim> const& arrow) {
	os << "(cylinder=" << arrow.getCylinder() << ",cone=" << arrow.getCone() << ")";
	return os;
}

} // End of namespace lowpoly3d

#endif // DIRECTION_HPP