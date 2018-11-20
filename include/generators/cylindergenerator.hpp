#ifndef CYLINDERGENERATOR_HPP
#define CYLINDERGENERATOR_HPP

#include "modelgenerator.hpp"

namespace lowpoly3d {

/** Generates cylinder **/
struct CylinderGenerator : public ModelGenerator {
	const Color color;
	const size_t pies;
	CylinderGenerator(const Color& color = {255, 0, 255}, const size_t pies = 16);
	Model generate() override;
};

}

#endif //CYLINDERGENERATOR_HPP