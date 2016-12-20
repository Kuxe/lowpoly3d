#include "circlegenerator.hpp"
#include "model.hpp"
#include <math.h>

CircleGenerator::CircleGenerator(const Vertex& normal, const Color& color) : normal(normal), color(color) { }

Model CircleGenerator::generate() {
	//TODO: IMPLEMENT
	std::vector<Vertex> vertices;
	std::vector<Color> colors(vertices.size(), color);
	std::vector<Triangle> triangles;
	return {vertices, colors, triangles};
}