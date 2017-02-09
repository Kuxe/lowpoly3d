#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

namespace lowpoly3d {

class Perlin {
private:
	size_t imageSize;
	std::vector<float> image;
public:

	Perlin(size_t imageSize, size_t startOctave, size_t endOctave, int seed) : imageSize(imageSize), image(imageSize*imageSize) {
		std::random_device rd;
		std::mt19937 gen(rd());
		gen.seed(seed);
		std::uniform_real_distribution<> dis(0, 1);

		//Iterate over octaves (low frequencies => high amplitude and high frequency => low amplitude)
		for(size_t currentOctave = startOctave; currentOctave <= endOctave; currentOctave++) {
			const size_t amplitude = powf(2.0f, endOctave - currentOctave);
			std::vector<glm::vec2> noisemap(imageSize*imageSize);
			std::generate(noisemap.begin(), noisemap.end(), [&] { return glm::vec2(dis(gen), dis(gen)); });

			//Loop through perlin image and compute value at each pixel
			for(size_t i = 0; i < imageSize*imageSize; i++) {
				const glm::vec2 p = glm::vec2(i%imageSize, i/imageSize) * ((powf(2.0f, currentOctave)-1.0f)/imageSize)+1.0f;

				const float c1x = floorf(p.x), c1y = floorf(p.y);
				const float c2x = ceilf(p.x), c2y = c1y;
				const float c3x = c1x, c3y = ceilf(p.y);
				const float c4x = c2x, c4y = c3y;
				const float u = p.x - c1x, v = p.y - c1y;

				const float dot1 = glm::dot({u, v}, noisemap[c1y * imageSize + c1x]);
				const float dot2 = glm::dot({p.x-c2x, p.y-c2y}, noisemap[c2y * imageSize + c2x]);
				const float dot3 = glm::dot({p.x-c3x, p.y-c3y}, noisemap[c3y * imageSize + c3x]);
				const float dot4 = glm::dot({p.x-c4x, p.y-c4y}, noisemap[c4y * imageSize + c4x]);

				constexpr auto lerp = [](float a, float b, float t) { return (1.0f - t) * a + t * b; };
				const float x1 = lerp(dot1, dot2, u), x2 = lerp(dot3, dot4, u);
				image[i] += amplitude * lerp(x1, x2, v);
			}
		}
	}

	/** Return value of perlin-noise at coordinate **/
	float get(size_t x, size_t y) const { return x < imageSize && y < imageSize ? image[y * imageSize + x] : 0.0f;
	}

	float operator()(size_t x, size_t y) const { return get(x, y); }
};

}

#endif //PERLIN_HPP