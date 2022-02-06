#ifndef MINIMUM_BOUNDING_SPHERE_HPP
#define MINIMUM_BOUNDING_SPHERE_HPP

namespace lowpoly3d
{

template<typename fpt, std::size_t dim>
using TPoint = glm::vec<dim, fpt>;

template<typename fpt, std::size_t dim> struct TSphere;
template<typename fpt, std::size_t dim> struct TTriangle;

// Returns a minimum bounding sphere over a triangle
template<typename fpt, std::size_t dim>
TSphere<fpt, dim> mbs(TTriangle<fpt, dim> const& triangle);

// Finds a minimum bounding sphere that encloses two spheres a and b
template<typename fpt, std::size_t dim>
TSphere<fpt, dim> mbs(
	TSphere<fpt, dim> const& a,
	TSphere<fpt, dim> const& b);

} // End of namespace lowpoly3d

#endif // MINIMUM_BOUNDING_SPHERE_HPP