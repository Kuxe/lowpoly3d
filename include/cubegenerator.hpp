#ifndef CUBEGENERATOR_HPP
#define CUBEGENERATOR_HPP

#include "modelgenerator.hpp"

struct CubeGenerator : public ModelGenerator {
	const Color color;
	CubeGenerator(const Color& color = {255, 0, 255});
	Model generate() override;
};

#endif //CUBEGENERATOR_HPP