#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

namespace lowpoly3d {

template<typename floating_point_type>
struct Rectangle {
	floating_point_type width, height;

	floating_point_type area() const {
		return width*height;
	}
};

} // End of namespace lowpoly3d

#endif // RECTANGLE_HPP