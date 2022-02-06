#ifndef VECTOR_PROJECTION
#define VECTOR_PROJECTION

#include <glm/detail/qualifier.hpp>

/* Projects a vector 'projectee' orthogonally onto a vector 'v' */
template<int D, typename T, glm::precision P>
glm::vec<D, T, P> projectOnto(glm::vec<D, T, P> const& projectee, glm::vec<D, T, P> v) {
    /* Let projectee = b and v = a, then we want to obtain the point P, see ASCII-art below.
    |        ^ b
    |      /|
    |    /  |
    |  /    |
    |/------|--->
       a'   P  a
    */

    /* a.b = ||a||*||b|| cos(theta) and cos(theta) = ||a'|| / ||b||, hence
	 * a.b = ||a||*||b||*||a'|| / ||b|| = ||a||*||a'||, so
	 * ||a'|| = a.b / ||a||
	 *
	 * Note that P = a / ||a|| * ||a'||, hence
	 * P = a / (||a||*||a||) * a.b = a * (a.b) / (a.a) */

	return v * (glm::dot(v, projectee) / glm::dot(v, v));
}

/* Projects a vector 'projectee' orthogonally onto a vector 'v',
 * where the perpendicular angle is between projectee and the returned point (as opposed to between projectee and v) */
template<int D, typename T, glm::precision P>
glm::vec<D, T, P> projectOnto2(glm::vec<D, T, P> const& projectee, glm::vec<D, T, P> v) {
	auto projectee_dot_projectee = glm::dot(projectee, projectee);
	auto projectee_dot_v = glm::dot(projectee, v);
	return (projectee_dot_projectee * projectee_dot_v) * v;
}

#endif // VECTOR_PROJECTION