#include "utils/no_such_triangle_exception.hpp"

#include <sstream>

NoSuchTriangleException::NoSuchTriangleException(std::size_t idx, std::size_t size) :
	std::out_of_range([idx, size] {
		std::ostringstream oss;
		oss << "Triangle on idx=" << idx << " does not exist. There are #" << size << " triangles.";
		return oss.str();
	}()){}