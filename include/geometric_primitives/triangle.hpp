#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <array> // std::array
#include <functional> // std::function
#include <iosfwd> // forwrd declare std::ostream

namespace lowpoly3d {

template<typename fpt, std::size_t dim>
struct TPlane;

template<typename value_type, std::size_t dimension>
using TPoint = glm::vec<dimension, value_type>;

template<typename fpt, std::size_t dim>
struct TTriangle;

template<typename fpt>
struct TDirection;

template<typename fpt, std::size_t dim>
struct TLine;

template<typename fpt, std::size_t dim>
struct TTriangle {
	using point_type = TPoint<fpt, dim>;
	using points_type = std::array<point_type, 3>;
	using iterator = typename points_type::iterator;
	using const_iterator = typename points_type::const_iterator;
	using reverse_iterator = typename points_type::reverse_iterator;
	using const_reverse_iterator = typename points_type::const_reverse_iterator;

	union {
		struct { point_type p1, p2, p3; };
		points_type points;
	};

	static_assert(sizeof(points) == sizeof(p1) + sizeof(p2) + sizeof(p3));

	TTriangle(points_type const& points);
	TTriangle(point_type const& p1, point_type const& p2, point_type const& p3);

	TTriangle(TTriangle<fpt, dim> const&);
	TTriangle(TTriangle<fpt, dim>&&);

	              iterator   begin()       noexcept;
                  iterator     end()       noexcept;
	        const_iterator  cbegin() const noexcept;
	        const_iterator    cend() const noexcept;
	      reverse_iterator  rbegin()       noexcept;
	      reverse_iterator    rend()       noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator   crend() const noexcept;

	// Returns the i:th point of this triangle
	point_type const& operator[](std::size_t i) const;

	// Transforms all vertices of this triangle given a transformation m
	TTriangle<fpt, dim> transform(glm::mat<dim, dim, fpt> const& m) const;

	// Transforms all vertices of this triangle given a homogenous transformation m
	TTriangle<fpt, dim> transform(glm::mat<dim+1, dim+1, fpt> const& m) const;

	// Returns a function in three barycentric coordinates for this triangle
	std::function<point_type(fpt, fpt, fpt)> getBarycentricParametrization() const;

	// Returns the area of this triangle
	fpt area() const;

	// Returns true if this (closed) triangle contains the given point
	bool contains(TPoint<fpt, dim> const& point) const;

	// Returns true if this triangle is degenerate
	bool degenerate() const;
};

// Returns the normal of triangle, with vertices winding CCW about the normal
template<typename fpt>
glm::vec<3, fpt> normal(TTriangle<fpt, 3> const& triangle);

// Returns the bisector for edge p0-p1
template<typename fpt>
TLine<fpt, 2> bisector_01(TTriangle<fpt, 2> const& triangle);

// Returns the bisector for edge p1-p2
template<typename fpt>
TLine<fpt, 2> bisector_12(TTriangle<fpt, 2> const& triangle);

// Returns the bisector for edge p2-p0
template<typename fpt>
TLine<fpt, 2> bisector_20(TTriangle<fpt, 2> const& triangle);

// Returns the edge-normal pointing away from the triangle for edge p0-p1
template<typename fpt>
glm::vec<3, fpt> edge_normal_12(TTriangle<fpt, 3> const& triangle);

// Returns the edge-normal pointing away from the triangle for edge p1-p2
template<typename fpt>
glm::vec<3, fpt> edge_normal_23(TTriangle<fpt, 3> const& triangle);

// Returns the edge-normal pointing away from the triangle for edge p2-p0
template<typename fpt>
glm::vec<3, fpt> edge_normal_31(TTriangle<fpt, 3> const& triangle);

// Returns a plane that is paralell to triangle
template<typename fpt>
TPlane<fpt, 3> parallel(TTriangle<fpt, 3> const& triangle);

// Projects triangle onto given plane
template<typename fpt>
TTriangle<fpt, 2> project(TTriangle<fpt, 3> const& triangle, TPlane<fpt, 3> const& plane);

// Projects triangle onto given plane and returns a point expressed in a local coordinate system of the plane
template<typename fpt>
TTriangle<fpt, 2> projectIntoLocal(TTriangle<fpt, 3> const& triangle, TPlane<fpt, 3> const& plane);

// Returns the circumcenter of triangle. TODO: Should be implemented for arbitrary dimension (not only 3)
template<typename fpt>
TPoint<fpt, 3> circumcenter(TTriangle<fpt, 3> const& triangle);

template<typename fpt>
auto const& vertices(TTriangle<fpt, 3> const& triangle)
{
	return triangle.points;
}

template<typename fpt, std::size_t dim>
std::ostream& operator<<(std::ostream& os, TTriangle<fpt, dim> const& triangle);

using Trianglef = TTriangle<float, 3>;
using Triangled = TTriangle<double, 3>;
using Triangle = Trianglef;
using Triangle2f = TTriangle<float, 2>;
using Triangle2d = TTriangle<double, 2>;
using Triangle2 = Triangle2f;

} // End of namespace lowpoly3d

#endif // TRIANGLE_HPP