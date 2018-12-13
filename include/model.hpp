#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <algorithm> //std::transform
#include <numeric> //std::accumulate
#include <modeldefs.hpp>
#include <glm/glm.hpp>

namespace lowpoly3d {

/** Model contains vertices, color per vertex and triangles,
	where a triangle is a triplet of vertex indices.
**/

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

	virtual ~Model() {};

	void subdivide(int i = 1);

	// Returns true on succesful append
	bool append(const Model& other);
	
	glm::vec3 triangle_midpoint(std::size_t i) const {
		return (1.0f / 3.0f) * (vertices[triangles[i][0]] + vertices[triangles[i][1]] + vertices[triangles[i][2]]);
	}

	void translate(const glm::vec3& translation);
	void scale(float factor);
};

//Transforms all vertices of model by transform
Model& transform(Model& m, const glm::mat4& transform);

//Joins arbitrary amount models (ie concatenate them) but first apply transform to vertices
Model join(const std::vector<Model>& models, const std::vector<glm::mat4>& transforms);

//Creates a model consisting of a single triangle. Useful for debugging.
Model getSingleTriangleModel();

//Creates a model consisting of two disjoint triangle. Useful for debugging.
Model getTwoTriangleModel();

}

#endif //MODEL_HPP