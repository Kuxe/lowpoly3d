#ifndef ARE_PARALLEL_HPP
#define ARE_PARALLEL_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace lowpoly3d {

namespace detail {

template<typename fpt, int dim>
struct AreParallel {}; // No default implementation for parallel lines

template<typename fpt>
struct AreParallel<fpt, 2> {
	constexpr bool operator()(
		glm::vec<2, fpt> const& d1,
		glm::vec<2, fpt> const& d2)
	{
		return std::abs(glm::dot(d1, glm::vec<2, fpt>(-d2.y, d2.x))) <= fpt{ 1e-6 };
	}
};

template<typename fpt>
struct AreParallel<fpt, 3> {
	constexpr bool operator()(
		glm::vec<3, fpt> const& d1,
		glm::vec<3, fpt> const& d2)
	{
		return glm::all(glm::epsilonEqual(glm::cross(d1, d2), glm::vec<3, fpt>(0, 0, 0), fpt{ 1e-6 }));
	}
};

} // End of namespace detail

template<typename fpt, int dim>
bool areParallel(
	glm::vec<dim, fpt> const& v1,
	glm::vec<dim, fpt> const& v2)
{
	return detail::AreParallel<fpt, dim>()(v1, v2);
}

}

#endif // ARE_PARALLEL_HPP