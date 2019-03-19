#ifndef LINE_SEGMENT_HPP
#define LINE_SEGMENT_HPP


#include "geometric_primitives/point.hpp"

#include "utils/lerp.hpp" // lerp

namespace lowpoly3d {

template<typename floating_point_type, std::size_t dimension>
struct TLineSegment {
	using point_type = TPoint<floating_point_type, dimension>;

	union { point_type start, p1; };
	union { point_type end, p2; };

	TLineSegment(const point_type& start, const point_type& end)
        : start(start), end(end) {

        assert(start != end &&
            "Start and end of linesegment are equal so it must be a point!");
    }

	floating_point_type midpoint() const {
		return 0.5*(end + start);
	}

	std::function<point_type(double)> parametrization() {
        return [this](double t) {
            assert(t >= 0.0 && t <= 1.0);
            lerp(start, end, t);
        };
    }
};

template<typename floating_point_type, std::size_t dimension>
std::ostream& operator<<(std::ostream& out, const TLineSegment<floating_point_type, dimension>& segment) {
	return out << "(p1=" << glm::to_string(segment.p1).c_str() << ", p2=" << glm::to_string(segment.p2).c_str() << ")";
}

using LineSegment  = TLineSegment<float, 3>;
using LineSegmentf = TLineSegment<float, 3>;
using LineSegmentd = TLineSegment<double, 3>;

} // End of namespace lowpoly3d

#endif // LINE_SEGMENT_HPP