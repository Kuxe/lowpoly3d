#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <stddef.h>
#include <vector>
#include <random>

namespace lowpoly3d {

/** Generates a noise on construction in range [-1.0f, 1.0f] **/
class Perlin {
private:
	size_t imageSize;
	std::vector<float> image;
	float max = 0.0f;
	void addOctave(float amplitude, float frequecy, std::mt19937& gen, std::uniform_real_distribution<>& dis);
public:
	explicit Perlin(size_t imageSize, float amplitude, float frequecy, int seed);
	Perlin(const Perlin& perlin);
	explicit Perlin(size_t imageSize, size_t startOctave, size_t endOctave, int seed);

	auto begin() { return image.begin(); }
	auto end() { return image.end(); }
	const auto begin() const { return image.begin(); }
	const auto end() const { return image.end(); }

	Perlin& binop(const Perlin& other, const auto& binaryop);
	Perlin& operator=(Perlin perlin);
	Perlin& operator+=(const Perlin& perlin);
	Perlin& operator-=(const Perlin& perlin);
	Perlin& operator*=(const Perlin& perlin);
	Perlin& operator/=(const Perlin& perlin);

	/** Return value of perlin-noise at coordinate **/
	float get(size_t x, size_t y) const;
	float operator()(size_t x, size_t y) const;

	friend Perlin operator+(Perlin a, const Perlin& b);
	friend Perlin operator-(Perlin a, const Perlin& b);
	friend Perlin operator*(Perlin a, const Perlin& b);
	friend Perlin operator/(Perlin a, const Perlin& b);

};

}

#endif //PERLIN_HPP