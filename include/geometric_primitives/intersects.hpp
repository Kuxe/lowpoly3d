#ifndef INTERSECTS_HPP
#define INTERSECTS_HPP

#include <glm/detail/type_vec.hpp>

/* intersects.hpp contains a bulk of various boolean
 * intersection-tests between geometric primitives 
 * 
 * Note: Some of these functions will throw NotImplementedException.
 * The domain of template*/

namespace lowpoly3d {

// Forward declarations
template<typename fpt, std::size_t dim> struct TLineSegment;
template<typename fpt, std::size_t dim> class TPlane;
template<typename value_type, std::size_t dimension>
using TPoint = glm::vec<dimension, value_type>;
template<typename fpt, std::size_t dim> struct TTriangle;

namespace detail {

/* Intersects is a struct that wraps a lot of pure functions
 * This is just to make explicit instantiation easier. */
template<typename fpt, std::size_t dim> 
struct Intersects
{
	static bool intersects(TLineSegment<fpt, dim>,TPoint<fpt, dim>);
};

} // End of namespace detail

bool intersects(TLineSegment<float, 3> const& ls, TPlane<float, 3> const& p);
bool intersects(TLineSegment<double, 3> const& ls, TPlane<double, 3> const& p);

template<typename fpt, std::size_t dim>
bool intersects(
	TLineSegment<fpt, dim> const& ls,
	      TPoint<fpt, dim> const& p)
{
	return detail::Intersects<fpt, dim>::intersects(ls ,p);
}

bool intersects(TLineSegment< float, 2> const& ls1, TLineSegment< float, 2> const& ls2);
bool intersects(TLineSegment<double, 2> const& ls1, TLineSegment<double, 2> const& ls2);
bool intersects(TLineSegment< float, 3> const& ls,     TTriangle< float, 3> const& t);
bool intersects(TLineSegment<double, 3> const& ls,     TTriangle<double, 3> const& t);
bool intersects(   TTriangle< float, 3> const& t,         TPoint< float, 3> const& p);
bool intersects(   TTriangle<double, 3> const& t,         TPoint<double, 3> const& p);
bool intersects(   TTriangle< float, 3> const& t1,     TTriangle< float, 3> const& t2);
bool intersects(   TTriangle<double, 3> const& t1,     TTriangle<double, 3> const& t2);

/* Intersects functions are commutative, so here follows functions
 * where order of arguments have been swapped */

bool intersects(TPlane< float, 3> const& p, TLineSegment< float, 3> const& ls);
bool intersects(TPlane<double, 3> const& p, TLineSegment<double, 3> const& ls);

template<typename fpt, std::size_t dim> 
bool intersects(TPoint<fpt, dim> const& p, TLineSegment<fpt, dim> const& ls) { return intersects(ls, p); }

bool intersects(TTriangle< float, 3> const& t, TLineSegment< float, 3> const& ls);
bool intersects(TTriangle<double, 3> const& t, TLineSegment<double, 3> const& ls);
bool intersects(   TPoint< float, 3> const& p,    TTriangle< float, 3> const& t);
bool intersects(   TPoint<double, 3> const& p,    TTriangle<double, 3> const& t);

}

#endif //INTERSECTS_HPP