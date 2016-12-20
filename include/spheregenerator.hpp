#ifndef SPHEREGENERATOR_HPP
#define SPHEREGENERATOR_HPP

#include "modelgenerator.hpp"

struct SphereGenerator : public ModelGenerator {
	const Color color;
	const uint8_t subdivides;
	SphereGenerator(const Color& color = {255, 0, 255}, uint8_t subdivides = 0);
	Model generate() override;
};

#endif //SPHEREGENERATOR_HPP