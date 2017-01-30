#ifndef PLANEGENERATOR_HPP
#define PLANEGENERATOR_HPP

#include <glm/vec3.hpp>
#include "modelgenerator.hpp"

namespace lowpoly3d {

class PlaneGenerator : public ModelGenerator {
private:
	glm::vec3 normal;
	Color color;
	uint8_t subdivides;
public:
	PlaneGenerator(const glm::vec3& normal = {0.0, 1.0, 0.0}, const Color& color = {255, 0, 255}, const uint8_t subdivies = 0);
	Model generate() override;
};

}

#endif //PLANEGENERATOR_HPP