#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <stddef.h>
#include <vector>

namespace lowpoly3d {

class Perlin {
private:
	size_t imageSize;
	std::vector<float> image;
public:

	Perlin(size_t imageSize, size_t startOctave, size_t endOctave, int seed);

	/** Return value of perlin-noise at coordinate **/
	float get(size_t x, size_t y) const;
	float operator()(size_t x, size_t y) const;
};

}

#endif //PERLIN_HPP