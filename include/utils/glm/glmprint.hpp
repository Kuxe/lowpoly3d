#ifndef GLMPRINT_HPP
#define GLMPRINT_HPP

#include <glm/gtx/string_cast.hpp>

template<typename Out, int T, typename U, glm::precision P>
Out& operator<<(Out& out, const glm::vec<T, U, P>& g) {
    return out << glm::to_string(g);
}

#endif // GLMPRINT_HPP