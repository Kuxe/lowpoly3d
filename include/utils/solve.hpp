/* Contains several free-functions, all of which solve some kind of equation */

#ifndef SOLVE_HPP
#define SOLVE_HPP

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

} // End of namespace lowpoly3d

#endif // SOLVE_HPP