#include "perlin.hpp"
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>

#include "utils/misc.hpp" // inplace_zip
#include "utils/lerp.hpp" // lerp and bilerp

namespace lowpoly3d {

std::vector<glm::vec2> generateRandomGradients(std::size_t resolution, std::uint8_t& seed) {
	std::vector<glm::vec2> randomGradients(resolution*resolution);
	auto nextr = [&seed] { return seed = (seed * 0xB105) % 233; };

	std::generate(randomGradients.begin(), randomGradients.end(), [&nextr] {
		return glm::vec2(nextr(), nextr());
	});
	return randomGradients;
}

void Perlin::addOctave(float amplitude, float frequency) {
	std::vector<glm::vec2> noisemap = generateRandomGradients(resolution, seed);

	//Loop through perlin image and compute value at each pixel
	for(size_t i = 0; i < resolution*resolution; i++) {
		const glm::vec2 p = glm::vec2(i%resolution, i/resolution) * ((frequency-1.0f)/resolution)+1.0f;

		const float c1x = std::floor(p.x), c1y = std::floor(p.y);
		const float c2x = std::ceil(p.x), c2y = c1y;
		const float c3x = c1x, c3y = std::ceil(p.y);
		const float c4x = c2x, c4y = c3y;

		std::size_t 
			i1 = c1y * resolution + c1x,
			i2 = c2y * resolution + c2x,
			i3 = c3y * resolution + c3x,
			i4 = c4y * resolution + c4x;

		assert(i1 < noisemap.size());
		assert(i2 < noisemap.size());
		assert(i3 < noisemap.size());
		assert(i4 < noisemap.size());

		// Compute interpolation parameters for bilinear interpolation
		const float u = p.x - c1x, v = p.y - c1y;
		image[i] += amplitude * bilerp(
			glm::dot({u, v}, noisemap[i1]),
			glm::dot({p.x-c2x, p.y-c2y}, noisemap[i2]),
			glm::dot({p.x-c3x, p.y-c3y}, noisemap[i3]),
			glm::dot({p.x-c4x, p.y-c4y}, noisemap[i4]),
			u, v);

		maxNoiseValue = std::max(maxNoiseValue, std::abs(image[i]));
	}
}

Perlin::Perlin(size_t resolution, float amplitude, float frequency, int seed) : resolution(resolution), image(resolution*resolution), seed(seed) {
	addOctave(amplitude, frequency);
}

Perlin::Perlin(const Perlin& perlin) : resolution(perlin.resolution), image(perlin.image), maxNoiseValue(perlin.maxNoiseValue), seed(perlin.seed) { }

Perlin::Perlin(size_t size, size_t start, size_t end, int seed) : resolution(size), image(size*size), seed(seed) {
	for(size_t i = start; i < end; i++) {
		addOctave(powf(2.0f, i), powf(2.0f, end-i));
	}
}

Perlin& Perlin::binop(const Perlin& other, const std::function<float(float, float)>& binaryop) {
	//For each binary operation made per pixel, ensure that maxNoiseValue is updated
	inplace_zip(image, other.image, [&](float a, float b) {
		const float binopval = binaryop(a, b);
		maxNoiseValue = std::max(maxNoiseValue, std::abs(binopval));
		return binopval;
	});
	return *this;
}

Perlin& Perlin::operator=(Perlin perlin) {
	std::swap(resolution, perlin.resolution);
	std::swap(image, perlin.image);
	std::swap(maxNoiseValue, perlin.maxNoiseValue);
	return *this;
}

Perlin& Perlin::operator+=(const Perlin& perlin) { return binop(perlin, [](float a, float b) { return a+b; }); }
Perlin& Perlin::operator-=(const Perlin& perlin) { return binop(perlin, [](float a, float b) { return a-b; }); }
Perlin& Perlin::operator*=(const Perlin& perlin) { return binop(perlin, [](float a, float b) { return a*b; }); }
Perlin& Perlin::operator/=(const Perlin& perlin) { return binop(perlin, [](float a, float b) { return a/b; }); }

float Perlin::get(size_t x, size_t y) const { return x < resolution && y < resolution ? image[y * resolution + x] / maxNoiseValue : 0.0f;}
float Perlin::operator()(size_t x, size_t y) const { return get(x, y); }

Perlin operator+(Perlin a, const Perlin& b) { return a += b; }
Perlin operator-(Perlin a, const Perlin& b) { return a -= b; }
Perlin operator*(Perlin a, const Perlin& b) { return a *= b; }
Perlin operator/(Perlin a, const Perlin& b) { return a /= b; }

} //End of namespace lowpoly3d