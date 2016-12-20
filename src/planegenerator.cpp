#include "planegenerator.hpp"
#include "model.hpp"
#include <math.h>

PlaneGenerator::PlaneGenerator(const Vertex& normal, const Color& color) : normal(normal), color(color) { }

Model PlaneGenerator::generate() {

	//Create two axes perpendicular to normal
	const glm::vec3 axis1 = glm::cross(normal, {normal.z, normal.x, normal.y}),
					axis2 = glm::cross(normal, axis1);

	std::vector<Vertex> vertices = {axis1, axis2, -axis1, -axis2};
	std::vector<Color> colors(vertices.size(), color);
	std::vector<Triangle> triangles = {{0,1,2},{0,2,3}};
	return {vertices, colors, triangles};
}