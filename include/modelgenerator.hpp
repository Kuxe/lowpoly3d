#ifndef MODELGENERATOR_HPP
#define MODELGENERATOR_HPP

#include "model.hpp"

namespace lowpoly3d {

/** Model generators generate vertices, colors and indices
	used for indexed drawing. It is within the model generators
	that procedural generation takes place.  **/
struct ModelGenerator { virtual Model generate() = 0; };

}

#endif //MODELGENERATOR