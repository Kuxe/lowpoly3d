#ifndef CONE_HPP
#define CONE_HPP

#include "geometric_primitives/linesegment.hpp"
#include "glm/gtx/euler_angles.hpp"

#include <glm/gtx/rotate_vector.hpp>

namespace lowpoly3d
{

template<typename floating_point_type, std::size_t dim>
using TPoint = glm::vec<dim, floating_point_type>;

// Represent a cone as a linesegment with a radius, linearly decreasing from
// start to end of the linesegment
template<typename floating_point_type, std::size_t dim>
class TCone
{
public:

	using point_type = TPoint<floating_point_type, dim>;
	using line_segment_type = TLineSegment<floating_point_type, dim>;

	TCone(line_segment_type const& linesegment, floating_point_type radius)
		: linesegment(linesegment), radius(radius)
	{
		assert(radius > floating_point_type(0));
	}

	floating_point_type getRadius() const { return radius; }
	floating_point_type getHeight() const { return linesegment.length(); }

	line_segment_type const& getCenterline() const { return linesegment; }

	// Returns a parametrization of the cone 'f(radian, t)'
	std::function<point_type(floating_point_type, floating_point_type)> parametrization() const
	{
		auto const orientation = glm::mat3(glm::orientation(glm::normalize(linesegment.p2 - linesegment.p1), {0, 1, 0}));
		return
			[radius = radius, height = getHeight(), orientation = orientation]
			(floating_point_type radian, floating_point_type t) -> point_type {
				return orientation * point_type{radius*std::cos(radian), t*height, radius*std::sin(radian)};
			};
	}

private:
	line_segment_type linesegment;
	floating_point_type radius;
};

using Cone2f = TCone<float, 2>;
using Cone2d = TCone<double, 2>;
using Cone2  = Cone2f;
using Conef = TCone<float, 3>;
using Coned = TCone<double, 3>;
using Cone  = Conef;

template<typename floating_point_type, std::size_t dim>
std::ostream& operator<<(std::ostream& os, TCone<floating_point_type, dim> const& cone) {
	os << "(centerline=" << cone.getCenterline() << ",radius=" << cone.getRadius() << ")";
	return os;
}

} // End of namespace lowpoly3d

#endif // CONE_HPP