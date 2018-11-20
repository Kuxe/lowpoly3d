#ifndef CIRCLEGENERATOR_HPP
#define CIRCLEGENERATOR_HPP

#include <glm/vec3.hpp>
#include "modelgenerator.hpp"

namespace lowpoly3d {

class CircleGenerator : public ModelGenerator {
private:
	glm::vec3 normal;
	Color color;
public:
	CircleGenerator(const glm::vec3& normal = {0.0, 1.0, 0.0}, const Color& color = {255, 0, 255});
	Model generate() override;
};

}

#endif //CIRCLEGENERATOR_HPP