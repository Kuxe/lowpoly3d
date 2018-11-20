#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <cstddef> // std::size_t

namespace lowpoly3d {

template<typename value_type, std::size_t dimension>
struct TTriangle {
    TPoint<value_type, dimension> p1, p2, p3;

    // Returns a plane that is paralell to this triangle
    Plane parallel() const {
        return { p1, glm::cross(p1 - p2, p1 - p3) };
    }
};

using Triangle = TTriangle<float, 3>;

} // End of namespace lowpoly3d

#endif // TRIANGLE_HPP