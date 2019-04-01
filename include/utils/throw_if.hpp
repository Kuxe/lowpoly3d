#ifndef THROW_IF
#define THROW_IF

#include <functional>

#include "utils/no_such_triangle_exception.hpp"

template<typename Exception, typename ...Args>
void throw_if(std::function<bool()> const& predicate, Args... args) {
	if(predicate()) throw Exception(args...);
}

/* Throws NoSuchTriangle-exception if idx >= size */
void throw_no_such_triangle_if_geq(std::size_t idx, std::size_t size);

#endif //THROW_IF