#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <stddef.h>
#include <vector>
#include <functional>
#include <cmath>
#include <cassert>

namespace lowpoly3d {

struct Pixel {
	std::size_t value;
};

// Representation of an zero-indexed 2D image containing arbitrary values
template<typename value_type>
class Image {
private:
	std::vector<value_type> image;
	std::size_t resolution;

public:
	Image(std::size_t resolution) : image(resolution * resolution), resolution(resolution) { }

	const value_type& get(std::size_t i, std::size_t j) const { return image(resolution * i + j); }
	void set(std::size_t i, std::size_t j, const value_type& value) { image(resolution * i + j) = value; }
	std::size_t getResolution() const { return resolution; }

	// Stretches this image into the larger resolution, using bilinear interpolation
	Image stretch(std::size_t largerResolution) const {
		assert(largerResolution >= resolution);	
		Image stretchedImage(largerResolution);

		/* 	Loop over all pixels of the stretched image and find corresponding
			values in this image using bilinear interpolation.
			First off, find corresponding pixel in original image
			(think of it like taking the pixel of larger image into [0.0, 1.0]
			and then upscaling it back to [0.0, resolution]) */
		for(std::size_t i = 0; i < largerResolution; i++) {
			for(std::size_t j = 0; j < largerResolution; j++) {
				// First compute a floating-point pixel-value (this can be interpreted as an interpolated pixel between integer pixel)
				const double correspondingFloatingPointPixel = static_cast<double>(resolution) / static_cast<double>(largerResolution);
				// Find whatever t "was used" to get correspondingPixel, i.e
				// translate the correspondingFloatingPointPixel down to [0.0, 1.0].
				// This number in [0.0, 1.0] is obviously the interpolation parameter.
				const std::size_t correspondingPixel = static_cast<std::size_t>(std::floor(correspondingFloatingPointPixel));
				const double t = correspondingFloatingPointPixel - correspondingPixel;
				// This interpolation parameter is now used to perform bilinear interpolation
				const value_type
					topleft 	= image.get(correspondingPixel, correspondingPixel),
					topright 	= image.get(correspondingPixel+1, correspondingPixel),
					bottomleft 	= image.get(correspondingPixel, correspondingPixel+1),
					bottomright = image.get(correspondingPixel+1, correspondingPixel+1);
				const value_type bilerpedValue = bilerp(topleft, topright, bottomleft, bottomright, t, t);

				// We have finally computed a bilerpedImage for the i,j-pixel. Only remains to set it!
				stretchedImage.set(i, j, bilerpedValue);
			}
		}
		return stretchedImage;
	}
};

/** Generates a noise on construction in range [-1.0f, 1.0f] **/
class Perlin {
private:
	size_t resolution;
	std::vector<float> image;

	// Used to normalize noise into range [-1.0f, 1.0f]
	float maxNoiseValue = 0.0f;
	uint8_t seed = static_cast<uint8_t>(0xB1A5EDD1CE); //Default seed
	void addOctave(float amplitude, float frequecy);
public:
	explicit Perlin(size_t resolution, float amplitude, float frequecy, int seed);
	Perlin(const Perlin& perlin);
	explicit Perlin(size_t resolution, size_t startOctave, size_t endOctave, int seed);

	auto begin() { return image.begin(); }
	auto end() { return image.end(); }
	const auto begin() const { return image.begin(); }
	const auto end() const { return image.end(); }

	Perlin& binop(const Perlin& other, const std::function<float(float, float)>& binaryop);
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