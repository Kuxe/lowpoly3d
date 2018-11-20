#ifndef LINE_SEGMENT_HPP
#define LINE_SEGMENT_HPP


#include "geometric_primitives/point.hpp"

#include "utils/lerp.hpp" // lerp

namespace lowpoly3d {

template<typename value_type, std::size_t dimension>
struct TLineSegment {
	using point_type = TPoint<value_type, dimension>;
    point_type start, end;

	TLineSegment(const point_type& start, const point_type& end)
        : start(start), end(end) {

        assert(start != end &&
            "Start and end of linesegment are equal so it must be a point!");
    }

	value_type midpoint() const {
		return 0.5*(end + start);
	}

	std::function<point_type(double)> parametrization() {
        return [this](double t) {
            assert(t >= 0.0 && t <= 1.0);
            lerp(start, end, t);
        };
    }
};

using LineSegment = TLineSegment<glm::vec3, 3>;

} // End of namespace lowpoly3d

#endif // LINE_SEGMENT_HPP