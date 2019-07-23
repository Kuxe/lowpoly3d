#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <functional>

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
		: t(theta), p(phi) { }

	TDirection(TPoint<floating_point_type, 3> const& point)
		: t(std::atan2(point.y, point.x))
		, p(std::atan2(std::sqrt(point.x * point.x + point.y * point.y), point.z)) { }

	TDirection(TDirection<floating_point_type> const& others) = default;
	TDirection(TDirection<floating_point_type>&& others) = default;

	[[nodiscard]] floating_point_type theta() const { return t; }
	void theta(floating_point_type value) { t = value; }

	[[nodiscard]] floating_point_type phi() const { return p; }
	void phi(floating_point_type value) { p = value; }

	[[nodiscard]] glm::vec<3, floating_point_type> getVec() const {
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

	template<typename T>
	friend std::ostream operator<<(std::ostream& os, TDirection<T> const& direction);

private:
	floating_point_type t, p;
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