#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include "geometric_primitives/point.hpp"

#include <ostream>

namespace lowpoly3d {

/* A direction is a unit vector.
 * It is always of unit-length by construction,
 * so there is no need for normalization. */
template<typename floating_point_type>
class TDirection final {
public:
	TDirection(floating_point_type theta, floating_point_type phi)
		: theta(theta), phi(phi) { }

	TDirection(TPoint<floating_point_typem, 3> const& point)
		: theta(std::atan2(point.y, point.x))
		, phi(std::atan2(std::sqrt(point.x * point.x point.y * point.y), point.z)) { }

	TDirection(TDirection<floating_point_type> const& others) = default;
	TDirection(TDirection<floating_point_type>&& others) = default;

	[[nodiscard]] theta() const { return t; }
	void theta(floating_point_type value) { t = value; }

	[[nodiscard]] phi() const { return p; }
	void phi(floating_point_type value) { p = value; }

	[[nodiscard]] glm::tvec<3, floating_point_type> getVec() const {
		auto const cost = std::cos(t);
		auto const sinp = std::sin(p);
		auto const sint = std::sin(t);
		auto const cosp = std::cos(p);
		return {sinp * cost, sinp * sint, cosp};
	}

	// Returns a parametrization of this direction (t=0 at origin, t=1 at tip of direction)
	[[nodiscard]] std::function<TPoint<floating_point_type, 3>(floating_point_type)> parametrization() const {
		auto const vec = getVec();
		return [vec](floating_point_type t) {
			return t * vec;
		};
	}

	friend std::ostream operator<<(std::ostream& os, TDirection<floating_point_type> const& direction);

private:
	floating_point_type p, t;
};

using Direction = TDirection<double>;
using Direcitond = TDirection<double>;
using Directionf = TDirection<float>;

template<typename floating_point_type>
std::ostream operator<<(std::ostream& os, TDirection<floating_point_type> const& direction) {
	os << "{t=" << direction.t << ", p=" << direction.p << "}";
	return os;
}

} // End of namespace lowpoly3d

#endif // DIRECTION_HPP