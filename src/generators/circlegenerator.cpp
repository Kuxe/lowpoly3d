#include "generators/circlegenerator.hpp"
#include "model.hpp"

namespace lowpoly3d {

CircleGenerator::CircleGenerator(const Vertex& normal, const Color& color) : normal(normal), color(color) { }

Model CircleGenerator::generate() {
	//TODO: IMPLEMENT
	std::vector<Vertex> vertices;
	std::vector<Color> colors(vertices.size(), color);
	std::vector<Triangle> triangles;
	return {vertices, colors, triangles};
}

}
