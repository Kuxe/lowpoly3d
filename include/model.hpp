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
	using vertex_type = Vertex;
	using color_type = Color;
	using triangle_indices_type = TriangleIndices;
	using triangle_index_type = TriangleIndices::value_type;

	std::vector<vertex_type> vertices;
	std::vector<color_type> colors;
	std::vector<triangle_indices_type> triangleIndices;

	//Load model from "raw" mesh data
	Model(
		const std::vector<vertex_type>& vertices,
		const std::vector<color_type>& colors,
		const std::vector<triangle_indices_type>& triangleIndices
	);

	Model(const Model& model);

	Model();

	virtual ~Model() {};

	void subdivide(int i = 1);

	// Returns true on succesful append
	bool append(const Model& other);
	
	glm::vec3 triangle_midpoint(std::size_t i) const;

	std::size_t getNumVertices() const;
	std::size_t getNumTriangles() const;

	triangle_indices_type getTriangleIndices(std::size_t triangleIdx) const;

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