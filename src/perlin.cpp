#include "perlin.hpp"
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include "misc.hpp"

namespace lowpoly3d {

void Perlin::addOctave(float amplitude, float frequency) {
	std::vector<glm::vec2> noisemap(imageSize*imageSize);

	auto nextr = [this] { return seed = (seed * 0xB105) % 233; };

	std::generate(noisemap.begin(), noisemap.end(), [&nextr] {
		return glm::vec2(nextr(), nextr());
	});

	//Loop through perlin image and compute value at each pixel
	for(size_t i = 0; i < imageSize*imageSize; i++) {
		const glm::vec2 p = glm::vec2(i%imageSize, i/imageSize) * ((frequency-1.0f)/imageSize)+1.0f;

		const float c1x = floorf(p.x), c1y = floorf(p.y);
		const float c2x = ceilf(p.x), c2y = c1y;
		const float c3x = c1x, c3y = ceilf(p.y);
		const float c4x = c2x, c4y = c3y;
		const float u = p.x - c1x, v = p.y - c1y;

		const auto lerp = [](float a, float b, float t) { return (1.0f - t) * a + t * b; };
		const float 
		x1 = lerp(
			glm::dot({u, v}, noisemap[c1y * imageSize + c1x]),
			glm::dot({p.x-c2x, p.y-c2y}, noisemap[c2y * imageSize + c2x]), u),
		x2 = lerp(
			glm::dot({p.x-c3x, p.y-c3y}, noisemap[c3y * imageSize + c3x]),
			glm::dot({p.x-c4x, p.y-c4y}, noisemap[c4y * imageSize + c4x]), u);

		image[i] += amplitude * lerp(x1, x2, v);
		max = std::max(max, std::abs(image[i]));
	}
}

Perlin::Perlin(size_t imageSize, float amplitude, float frequency, int seed) : imageSize(imageSize), image(imageSize*imageSize), seed(seed) {
	addOctave(amplitude, frequency);
}

Perlin::Perlin(const Perlin& perlin) : imageSize(perlin.imageSize), image(perlin.image), max(perlin.max), seed(perlin.seed) { }

Perlin::Perlin(size_t size, size_t start, size_t end, int seed) : imageSize(size), image(size*size), seed(seed) {
	for(size_t i = start; i < end; i++) {
		addOctave(powf(2.0f, i), powf(2.0f, end-i));
	}
}

Perlin& Perlin::binop(const Perlin& other, const std::function<float(float, float)>& binaryop) {
	//For each binary operation made per pixel, ensure that max is updated
	inplace_zip(image, other.image, [&](float a, float b) {
		const float binopval = binaryop(a, b);
		max = std::max(max, std::abs(binopval));
		return binopval;
	});
	return *this;
}

Perlin& Perlin::operator=(Perlin perlin) {
	std::swap(imageSize, perlin.imageSize);
	std::swap(image, perlin.image);
	std::swap(max, perlin.max);
	return *this;
}

Perlin& Perlin::operator+=(const Perlin& perlin) { return binop(perlin, [](float a, float b) { return a+b; }); }
Perlin& Perlin::operator-=(const Perlin& perlin) { return binop(perlin, [](float a, float b) { return a-b; }); }
Perlin& Perlin::operator*=(const Perlin& perlin) { return binop(perlin, [](float a, float b) { return a*b; }); }
Perlin& Perlin::operator/=(const Perlin& perlin) { return binop(perlin, [](float a, float b) { return a/b; }); }

float Perlin::get(size_t x, size_t y) const { return x < imageSize && y < imageSize ? image[y * imageSize + x] / max : 0.0f;}
float Perlin::operator()(size_t x, size_t y) const { return get(x, y); }

Perlin operator+(Perlin a, const Perlin& b) { return a += b; }
Perlin operator-(Perlin a, const Perlin& b) { return a -= b; }
Perlin operator*(Perlin a, const Perlin& b) { return a *= b; }
Perlin operator/(Perlin a, const Perlin& b) { return a /= b; }

} //End of namespace lowpoly3d