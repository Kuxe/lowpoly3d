#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "geometric_primitives/parallelogram.hpp"

namespace lowpoly3d {

// Represents a rectangle as a special-case of a parallelogram.
// See 'Parallelogram'-docs for more info.
template<typename floating_point_type, std::size_t dim>
struct TRectangle : public TParallelogram<floating_point_type, dim> {

	using point_type = typename TParallelogram<floating_point_type, dim>::point_type;
	using triangle_type = typename TParallelogram<floating_point_type, dim>::triangle_type;

	// Constructs a rectangle from a right-angle CCW triangle,
	// where the right angle is at p01
	TRectangle(point_type p0, point_type p1, point_type p2)
		: TParallelogram<floating_point_type, dim>(p0, p1, p2)
	{
		assert(this->isRectangle());
	}

	// Constructs a rectangle from a special-case parallelogram
	TRectangle(TParallelogram<floating_point_type, dim> const& parallelogram)
		: TRectangle(
				parallelogram.triangle.p1,
				parallelogram.triangle.p2,
				parallelogram.triangle.p3
			)
	{ }
};

using Rectanglef = TRectangle<float, 3>;
using Rectangled = TRectangle<double, 3>;
using Rectangle = Rectanglef;
using Rectangle2f = TRectangle<float, 2>;
using Rectangle2d = TRectangle<double, 2>;
using Rectangle2 = Rectangle2f;

} // End of namespace lowpoly3d

#endif // RECTANGLE_HPP