#ifndef TERRAINGENERATOR_HPP
#define TERRAINGENERATOR_HPP

#include "modelgenerator.hpp"

class TerrainGenerator : public ModelGenerator {
public:
	Model generate() override;
};

#endif //TERRAINGENERATOR_HPP