#include "terraingenerator.hpp"
#include "model.hpp"
#include <random>
#include "perlin.hpp"

namespace lowpoly3d {

TerrainGenerator::TerrainGenerator(const uint16_t numVerticesPerSide, const float tileWidth)
	: numVerticesPerSide(numVerticesPerSide), tileWidth(tileWidth) { }

Model TerrainGenerator::generate() {
	if(numVerticesPerSide*numVerticesPerSide >= uint16_t(0-1)) {
		printf("ERROR: Too many vertices %llu, max=%hu\n", static_cast<unsigned long long>(numVerticesPerSide*numVerticesPerSide), uint16_t(0-1));
		numVerticesPerSide = sqrt(numVerticesPerSide);
	}
	std::vector<Vertex> vertices(numVerticesPerSide*numVerticesPerSide);
	std::vector<Color> colors(numVerticesPerSide*numVerticesPerSide);
	std::vector<Triangle> triangles;

	const size_t START_OCTAVE = 0, END_OCTAVE = 6;
	const int seed = /** Und du bist mein **/0x50FA;
	Perlin perlin(numVerticesPerSide, START_OCTAVE, END_OCTAVE, seed);
	//1. Generate vertices
	for(uint16_t y = 0; y < numVerticesPerSide; y++) {
		for(uint16_t x = 0; x < numVerticesPerSide; x++) {
			vertices[y*numVerticesPerSide+x] = {x*tileWidth, perlin(x, y), y*tileWidth};
		}
	}
	//2. Color vertices
	for(uint16_t y = 0; y < numVerticesPerSide; y++) {
		for(uint16_t x = 0; x < numVerticesPerSide; x++) {
			colors[y*numVerticesPerSide+x] = {151, 176, 74};
		}
	}

	//4. Connect triangles (compute indices) by iterating over each triangle
	for(uint16_t i = 0; i < numVerticesPerSide*numVerticesPerSide - numVerticesPerSide; i++) {
		//Dont wrap triangles around the lattice
		if((i+1) % numVerticesPerSide != 0) {
			triangles.push_back({i, i+numVerticesPerSide, i+1});
			triangles.push_back({i+1, i+numVerticesPerSide, i+numVerticesPerSide+1});
		}
	}
	return {vertices, colors, triangles};
}

}
