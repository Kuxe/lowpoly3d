#ifndef GLMPRINT_HPP
#define GLMPRINT_HPP

#include <glm/gtx/string_cast.hpp>

// TODO: Add more operarotrs / find generic way to generate << for all glm types

std::ostream& operator<<(std::ostream& out, const glm::vec3& g) {
    return out << glm::to_string(g);
}

#endif // GLMPRINT_HPP