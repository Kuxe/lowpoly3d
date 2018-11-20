#include "generators/planegenerator.hpp"
#include "model.hpp"
#include <glm/ext.hpp>

namespace lowpoly3d {

PlaneGenerator::PlaneGenerator(const Vertex& normal, const Color& color, const uint8_t subdivides) :
	normal(normal), color(color), subdivides(subdivides) { }

Model PlaneGenerator::generate() {

	//Create two axes perpendicular to normal
	const glm::vec3 axis1 = glm::cross(normal, {normal.z, normal.x, normal.y}),
					axis2 = glm::cross(normal, axis1);

	std::vector<Vertex> vertices = {axis1, axis2, -axis1, -axis2};
	std::vector<Color> colors(vertices.size(), color);
	std::vector<Triangle> triangles = {{0,1,2},{0,2,3}};
	Model m = {vertices, colors, triangles};
	m.subdivide(subdivides);
	return m;
}

}
