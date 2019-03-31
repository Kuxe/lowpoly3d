/* Contains several free-functions, all of which solve some kind of equation */

#ifndef SOLVE_HPP
#define SOLVE_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/vector_query.hpp> // glm::areCollinear

#include "geometric_primitives/point.hpp"

namespace lowpoly3d {

/* Solves:
 *
 *   a . v = p
 *   b . v = q
 * 
 * for v in R2, with a,b,p,q given vectors in R2. */
template<typename floating_point_type>
TPoint<floating_point_type, 2> solveDotSystem2D(
	TPoint<floating_point_type, 2> const& a,
	TPoint<floating_point_type, 2> const& b,
	floating_point_type const& p,
	floating_point_type const& q)
{
	auto const transposedMat = glm::tmat2x2<floating_point_type>{a.x, b.x, a.y, b.y};
	auto const pq = glm::tvec2<floating_point_type>(p, q);
	return glm::inverse(transposedMat) * pq;
}

/* Solves v*r + w*s = c for r and s in R1, where V, W and C in R2.
 * Returns (r, s) if solution exist, otherwise returns (NaN, NaN).
 * A solution exists if V and W are linearly independent. Geometrically, this
 * function scales two vectors V and W by r and s such that they hit C. */
template<typename floating_point_type>
TPoint<floating_point_type, 2> solveScalingVecs(
	TPoint<floating_point_type, 2> const& v,
	TPoint<floating_point_type, 2> const& w,
	TPoint<floating_point_type, 2> const& c,
	floating_point_type eps = std::numeric_limits<floating_point_type>::epsilon())
{
	using vec2 = TPoint<floating_point_type, 2>;

	// Handle special case where v,w are collinear --- c may still be spanned by v and w.
	if(glm::areCollinear(v, w, eps)) {
		auto const NaN = std::numeric_limits<floating_point_type>::quiet_NaN();
		auto const vvdot = glm::dot(v, v);
		if(vvdot != 0) {
			auto const vcdot = glm::dot(v, c);
			auto const r = vcdot / vvdot;
			return glm::all(glm::epsilonEqual(r * v, c, eps)) // True iff v, c collinear as well
				? vec2{r, 0}
				: vec2{NaN, NaN};
		}

		auto const wwdot = glm::dot(w, w);
		if(wwdot != 0) {
			auto const wcdot = glm::dot(w, c);
			auto const s = wcdot / wwdot;
			return glm::all(glm::epsilonEqual(s * w, c, eps)) // True iff w, c collinear as well
				? vec2{0, s}
				: vec2{NaN, NaN};
		}

		// v=w=zerovec, so they can only span the zeropoint
		return glm::all(glm::epsilonEqual(c, vec2(0, 0), eps))
			? vec2{0, 0}
			: vec2{NaN, NaN};
	}

	return solveDotSystem2D(vec2{v.x, w.x}, vec2{v.y, w.y}, c.x, c.y);
}

} // End of namespace lowpoly3d

#endif // SOLVE_HPP