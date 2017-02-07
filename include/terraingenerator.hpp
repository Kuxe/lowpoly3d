#ifndef TERRAINGENERATOR_HPP
#define TERRAINGENERATOR_HPP

#include "modelgenerator.hpp"

namespace lowpoly3d {

class TerrainGenerator : public ModelGenerator {
private:
	uint16_t numVerticesPerSide;
	float tileWidth;
public:
	TerrainGenerator(const uint16_t numVerticesPerSide = 200, const float tileWidth = 1.0f);
	Model generate() override;
};

}

#endif //TERRAINGENERATOR_HPP