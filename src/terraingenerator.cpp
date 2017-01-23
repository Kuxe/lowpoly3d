#include "terraingenerator.hpp"
#include "model.hpp"
#include <math.h>
#include <random>
#include <glm/glm.hpp>

namespace lowpoly3d {

Model TerrainGenerator::generate() {
	std::vector<glm::vec3> vertices;
	std::vector<Color> colors;
	std::vector<Triangle> triangles;

	const uint16_t numVerticesPerSide = 200;
	const float tileWidth = 0.5f;

	std::random_device rd;
    std::knuth_b e2(rd());
    std::normal_distribution<> dist(0, 0.05);

	//1. Generate vertices
	for(int y = 0; y < numVerticesPerSide; y++) {
		for(int x = 0; x < numVerticesPerSide; x++) {
			vertices.push_back({x*tileWidth, dist(e2), y*tileWidth});
		}
	}
	//2. Color vertices
	for(int y = 0; y < numVerticesPerSide; y++) {
		for(int x = 0; x < numVerticesPerSide; x++) {
			colors.push_back({151, 176, 74});
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
