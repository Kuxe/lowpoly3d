#ifndef LINE_HPP
#define LINE_HPP

#include <cstddef> // std::size_t

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <type_traits>

#include "utils/glm/are_parallel.hpp"
#include "utils/glm/glmutils.hpp"

#include "geometric_primitives/point.hpp"

namespace lowpoly3d {

template<typename floating_point_type, std::size_t dimension>
struct TLine;

namespace detail {
	/* Performs a binary operation between the line normal and the point
	 * in a frame where the line point is at zero. */
	template<typename fpt, typename binary_op>
	constexpr fpt LocalLineNormalBinop(
		TLine<fpt, 2> const& line,
		TPoint<fpt, 2> const& point,
		binary_op const& binop)
	{
		auto const& direction = line.getDirection();
		return binop(TPoint<fpt, 2>{-direction.y, direction.x}, point - line.getPoint());
	};
	
	/* Function object that checks if a point of dimension 2 is above a line of dimension 2 */
	template<typename fpt, std::size_t dim> struct LineAbove {};
	template<typename fpt>
	struct LineAbove<fpt, 2> {
		using line_type = TLine<fpt, 2>;
		using point_type = TPoint<fpt, 2>;
		constexpr bool operator()(line_type const& line, point_type const& point) const {
			return LocalLineNormalBinop(line, point, [](auto const& p1, auto const& p2) {
				return glm::dot(p1, p2) < 0;
			});
		}
	};

	/* Function object that checks if a point of dimension 2 is below a line of dimension 2 */
	template<typename fpt, std::size_t dim> struct LineBelow {};
	template<typename fpt>
	struct LineBelow<fpt, 2> {
		using line_type = TLine<fpt, 2>;
		using point_type = TPoint<fpt, 2>;
		constexpr bool operator()(line_type const& line, point_type const& point) const {
			return LocalLineNormalBinop(line, point, [](auto const& p1, auto const& p2) {
				return glm::dot(p1, p2) > 0;
			});
		}
	};
} // End of namespace detail

/* TLine is a generic line in d-dimensions, represented by a unit direction
 * vector d and a point intersecting the line */
template<typename floating_point_type, std::size_t dimension>
struct TLine {
	using line_type = TLine<floating_point_type, dimension>;
	using point_type = TPoint<floating_point_type, dimension>;
	using vec_type = glm::vec<dimension, floating_point_type>;

	constexpr TLine(const point_type& p, const vec_type& d) : p(p), d(glm::normalize(d)) { }

	[[nodiscard]] const vec_type& getDirection() const { return d; }
	[[nodiscard]] const point_type& getPoint() const { return p; }

	// Return true if "point" lies on this line
	[[nodiscard]] bool contains(const point_type& point, floating_point_type tolerance = 1e-5) const {
		const auto crossed = glm::cross(point - p, d);
		return glm::dot(crossed, crossed) <= tolerance;
	}

	// Returns true if "direction" is parallel to this line
	[[nodiscard]] bool isParallelTo(const vec_type& direction) const {
		return areParallel<floating_point_type, dimension>(getDirection(), direction);
	}

	// Returns true if "line" is parallel to this line
	[[nodiscard]] bool isParallelTo(const line_type& line) const {
		return isParallelTo(line.getDirection());
	}

	// Returns true if the point is above this line. Only available for dimension=2.
	[[nodiscard]] bool above(TPoint<floating_point_type, dimension> const& point) const {
		return detail::LineAbove<floating_point_type, dimension>()(*this, point);
	}

	// Returns true if the point is below this line. Only available for dimension=2.
	[[nodiscard]] bool below(TPoint<floating_point_type, dimension> const& point) const {
		return detail::LineBelow<floating_point_type, dimension>()(*this, point);
	}

	// Returns a parametrization of this line
	[[nodiscard]] std::function<TPoint<floating_point_type, dimension>(floating_point_type)> parametrization() const {
		auto const point = getPoint();
		auto const direction = getDirection();
		return [point, direction](floating_point_type t) { return point + t * direction; };
	}

	// Returns the point at time t of the parametrization of this line
	[[nodiscard]] TPoint<floating_point_type, dimension> parametrization(floating_point_type t) const {
		return parametrization()(t);
	}

private:
	point_type p;
	vec_type d;
};

// Returns true if l1 and l2 represent the same undirected line
template<typename floating_point_type, std::size_t dimension>
bool almostEqual(
	TLine<floating_point_type, dimension> l1,
	TLine<floating_point_type, dimension> l2,
	floating_point_type tolerance = 1e-5) {
	return l1.isParallelTo(l2) && l1.contains(l2.getPoint(), tolerance);
}

using Line   = TLine<float , 3>;
using Lined  = TLine<double, 3>;
using Linef  = TLine<float , 3>;
using Line2  = TLine<float , 2>;
using Line2d = TLine<double, 2>;
using Line2f = TLine<float , 2>;

template<typename floating_point_type, std::size_t dimension>
std::ostream& operator<<(std::ostream& out, const TLine<floating_point_type, dimension>& line) {
	const auto& p = line.getPoint();
	const auto& d = line.getDirection();
	out << "(p=" << glm::to_string(p).c_str() << ", d=" << glm::to_string(d).c_str() << ")";
	return out;
}

} // End of namespace lowpoly3d

#endif // LINE_HPP