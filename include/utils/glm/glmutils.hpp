#ifndef GLMUTILS_HPP
#define GLMUTILS_HPP

#include <functional>
#include <numeric>
#include <glm/glm.hpp> // TODO: Narrow this include to just glm::vec
#include <glm/gtc/matrix_access.hpp> // glm::column
#include <vector>

namespace lowpoly3d {

// Given three vectors in R^4, returns a forth orthogonal vector
template<typename floating_point_type>
glm::vec<4, floating_point_type> orthogonal4(
	const glm::vec<4, floating_point_type>& v1,
	const glm::vec<4, floating_point_type>& v2,
	const glm::vec<4, floating_point_type>& v3
) {
	return orthogonal4(orthogonal4(v1, v2), v3);
}

// WARNING: Untested
template<typename floating_point_type, std::size_t dimension>
std::vector<glm::vec<dimension, floating_point_type>> gramschmidt(
	const std::vector<glm::vec<dimension, floating_point_type>>& v) {

	auto proj = [](
		const glm::vec<dimension, floating_point_type>& v,
		const glm::vec<dimension, floating_point_type>& u) {
		return glm::dot(u, v) / glm::dot(u, u) * u;
	};

	if(v.size() <= 1) return v;

	std::vector<glm::vec<dimension, floating_point_type>> u;
	u.reserve(v.size());
	u[0] = v[0];

	for(std::size_t i = 1; i < v.size(); i++) {
		u[i] = v[i];
		for(std::size_t j = i; j > 0; j--) {
			u[i] -= proj(u[j-1], v[i]);
		}
	}
	return u;
}

// Returns a function that applies f per component
template<typename in_type, typename out_type>
std::function<glm::tvec3<out_type>(glm::tvec3<in_type> const&)> componentwise(std::function<out_type(in_type)> f) {
	return [f](glm::tvec3<in_type> const& v) {
		return glm::tvec3<out_type>(f(v.x), f(v.y), f(v.z));
	};
};

}


#endif // GLMUTILS_HPP