#include "generators/cubegenerator.hpp"
#include <complex>
#include <vector>

typedef std::complex<float> fz;
using namespace std::literals::complex_literals;

namespace lowpoly3d {

CubeGenerator::CubeGenerator(const Color& color) : color(color) { }

Model CubeGenerator::generate() {
	std::vector<Vertex> vertices;

	using namespace std::complex_literals;
	const float sqrt1 = sqrtf(1.0f);
	fz z = {sqrt1, sqrt1};
	const uint16_t sides = 4;
	for(int i = 0; i < sides; i++) {
		vertices.push_back({std::real(z), std::imag(z), +1.0f});
		z *= (4.0/static_cast<double>(sides)) * 1i;
	}

	z = {sqrt1, sqrt1};
	for(int i = 0; i < sides; i++) {
		vertices.push_back({std::real(z), std::imag(z), -1.0f});
		z *= (4.0/static_cast<double>(sides)) * 1i;
	}
	
	std::vector<Triangle> triangles = {{0,1,2},{0,2,3}, {4,5,1},{4,1,0}, {5,4,7},{5,7,6}, {3,2,6},{3,6,7}, {4,0,3},{4,3,7}, {1,5,6},{1,6,2}};
	std::vector<Color> colors(vertices.size(), color);
	return {vertices, colors, triangles};
}

}
