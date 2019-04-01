#include "utils/throw_if.hpp"

void throw_no_such_triangle_if_geq(std::size_t idx, std::size_t size) {
	throw_if<NoSuchTriangleException>([idx, size] { return idx >= size; }, idx, size);
}