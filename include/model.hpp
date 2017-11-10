#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <algorithm> //std::transform
#include "modeldefs.hpp"
#include <glm/glm.hpp>

namespace lowpoly3d {

struct Model {
	std::vector<Vertex> vertices;
	std::vector<Color> colors;
	std::vector<Triangle> triangles;

	//Load model from "raw" mesh data
	Model(
		const std::vector<Vertex>& vertices,
		const std::vector<Color>& colors,
		const std::vector<Triangle>& triangles
	);

	Model(const Model& model);

	Model();

	void subdivide(int i = 1);
	void append(const Model& other);
};

//Transforms all vertices of model by transform
Model& transform(Model& m, const glm::mat4& transform);

//Joins arbitrary amount models (ie concatenate them) but first apply transform to vertices
Model join(const std::vector<Model>& models, const std::vector<glm::mat4>& transforms);

}

#endif //MODEL_HPP