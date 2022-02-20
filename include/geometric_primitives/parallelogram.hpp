#ifndef PARALLELOGRAM_HPP
#define PARALLELOGRAM_HPP

#include "geometric_primitives/triangle.hpp"

#include "glm/detail/qualifier.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/vector_query.hpp"

namespace lowpoly3d {

template<typename value_type, std::size_t dimension>
using TPoint = glm::vec<dimension, value_type>;

// Represent a parallelogram as a triangle
// with vertices v0, v1, v2 where a fourth vertex v4 with the appropiate
// properties is added to make it a parallelogram.
// The vertices v0, v1, v2 are assumed to be in CCW ordering.
template<typename floating_point_type, std::size_t dim>
class TParallelogram {
public:

	using point_type = TPoint<floating_point_type, dim>;
	using triangle_type = TTriangle<floating_point_type, dim>;

	TParallelogram(point_type const& p0, point_type const& p1, point_type const& p2)
		: triangle(p0, p1, p2), p3(p2 + p1 - p0)
	{}

	floating_point_type area() const
	{
		return floating_point_type(2) * triangle.area();
	}

	point_type const& getBottomLeftCorner() const
	{
		return triangle[0];
	}

	point_type const& getBottomRightCorner() const
	{
		return triangle[1];
	}

	point_type const& getTopLeftCorner() const
	{
		return triangle[2];
	}

	point_type const& getTopRightCorner() const
	{
		return p3;
	}

	// Returns the triangle (bottom-left, bottom-right and top-left vertices)
	triangle_type getFirstTriangle() const
	{
		return triangle;
	}

	// Returns the triangle (bottom-right, top-right and top-left vertices)
	triangle_type getSecondTriangle() const
	{
		return {getBottomRightCorner(), getTopRightCorner(), getTopLeftCorner()};
	}

	bool isRectangle() const
	{
		return glm::areOrthogonal(
			getBottomRightCorner() - getBottomLeftCorner(),
			getTopLeftCorner() - getBottomLeftCorner(),
			1E-4f
		);
	}
	
private:
	triangle_type triangle;
	point_type p3;
};

using Parallelogramf = TParallelogram<float, 3>;
using Parallelogramd = TParallelogram<double, 3>;
using Parallelogram = Parallelogramf;
using Parallelogram2f = TParallelogram<float, 2>;
using Parallelogram2d = TParallelogram<double, 2>;
using Parallelogram2 = Parallelogram2f;

} // End of namespace lowpoly3d

#endif // PARALLELOGRAM_HPP