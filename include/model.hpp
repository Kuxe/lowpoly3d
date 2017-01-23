#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/vec3.hpp>
#include <vector>
#include <string>
#include "modeldefs.hpp"

namespace lowpoly3d {

struct Model {
	std::vector<Vertex> vertices;
	std::vector<Color> colors;
	std::vector<Triangle> triangles;

	//Load model from file
	Model(const std::string& file);

	//Load model from "raw" mesh data
	Model(
		const std::vector<Vertex>& vertices,
		const std::vector<Color>& colors,
		const std::vector<Triangle>& triangles
	);

	void subdivide(int i = 1);
};

}

#endif //MODEL_HPP