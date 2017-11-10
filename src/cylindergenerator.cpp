#include "cylindergenerator.hpp"
#include <vector>
#include <glm/ext.hpp> //glm::quarter_pi

namespace lowpoly3d {

CylinderGenerator::CylinderGenerator(const Color& color, const size_t pies) : color(color), pies(pies) { }

Model CylinderGenerator::generate() {
	std::vector<Vertex> vertices;
	const float dtheta = 2.0f*glm::pi<float>() / float(pies);
	float theta = -glm::quarter_pi<float>();

	//Add vertices. The base-case corresponds to i = 0 so start from i = 1.
	for(std::remove_const<decltype(pies)>::type i = 0; i < pies; i++) {
		vertices.push_back({cosf(theta), 0.0f, -sinf(theta)});
		vertices.push_back({cosf(theta), 1.0f, -sinf(theta)});
		theta += dtheta;
	}

	//Now we have the vertices, time to connect them into a cylinder!
	std::vector<Triangle> triangles;

	//Connect cylinder shell
	for(std::remove_const<decltype(pies)>::type i = 0; i < pies-1; i++) {
		triangles.push_back({2*i, 2*i+2, 2*i+1});
		triangles.push_back({2*i+1, 2*i+2, 2*i+3});
	}
	triangles.push_back({2*pies-2, 0, 2*pies-1});
	triangles.push_back({2*pies-1, 0, 1});

	//Connect cylinder bottom
	vertices.push_back({0.0f, 0.0f, 0.0f}); //Center of bottom fan
	const size_t bottomIndex = vertices.size()-1;
	for(std::remove_const<decltype(pies)>::type i = 0; i < pies-1; i++) {
		triangles.push_back({bottomIndex, 2*i+2, 2*i});
	}
	triangles.push_back({0, bottomIndex-2, bottomIndex});

	//Connect cylinder lid
	vertices.push_back({0.0f, 1.0f, 0.0f}); //Center of lid fan
	const size_t lidIndex = vertices.size()-1;
	for(std::remove_const<decltype(pies)>::type i = 0; i < pies-1; i++) {
		triangles.push_back({lidIndex, 2*i+1, 2*i+3});
	}
	triangles.push_back({lidIndex, lidIndex-2, 1});
	std::vector<Color> colors(vertices.size(), color);
	return {vertices, colors, triangles};
}

}
