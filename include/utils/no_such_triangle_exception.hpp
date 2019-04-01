#ifndef NO_SUCH_TRIANGLE_EXCEPTION_HPP
#define NO_SUCH_TRIANGLE_EXCEPTION_HPP

#include <stdexcept> // std::logic_error
#include <string>

struct NoSuchTriangleException : public std::out_of_range {
	NoSuchTriangleException(std::size_t idx, std::size_t size);
};

#endif // NO_SUCH_TRIANGLE_EXCEPTION_HPP