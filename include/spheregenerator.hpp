#ifndef SPHEREGENERATOR_HPP
#define SPHEREGENERATOR_HPP

#include "modelgenerator.hpp"

namespace lowpoly3d {

struct SphereGenerator : public ModelGenerator {
	const Color color;
	const uint8_t subdivides;
	SphereGenerator(const Color& color = {255, 0, 255}, uint8_t subdivides = 0);
	std::string name() const;
	Model generate() override;
};

}

#endif //SPHEREGENERATOR_HPP