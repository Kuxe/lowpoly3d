#ifndef MODELGENERATOR_HPP
#define MODELGENERATOR_HPP

#include "model.hpp"

struct ModelGenerator {
	virtual Model generate() = 0;
};

#endif //MODELGENERATOR