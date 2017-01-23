#ifndef TERRAINGENERATOR_HPP
#define TERRAINGENERATOR_HPP

#include "modelgenerator.hpp"

namespace lowpoly3d {

class TerrainGenerator : public ModelGenerator {
public:
	Model generate() override;
};

}

#endif //TERRAINGENERATOR_HPP