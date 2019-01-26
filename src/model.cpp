#include <iostream>
#include "model.hpp"

namespace lowpoly3d {

Model::Model(
	const std::vector<vertex_type>& vertices,
	const std::vector<color_type>& colors,
	const std::vector<triangle_indices_type>& triangleIndices) : 
	vertices(vertices),
	colors(colors),
	triangleIndices(triangleIndices) {

	//Make sanity check, it should always be the case that max index in triangles is <= than number of vertices
	for(const auto& triangle : triangleIndices) {
		uint16_t maxIndex = 0;
		maxIndex = std::max(triangle.x, maxIndex);
		maxIndex = std::max(triangle.y, maxIndex);
		maxIndex = std::max(triangle.z, maxIndex);
		if(maxIndex >= vertices.size()) {
			printf("ERROR: An index refers to non-existent vertex, either model will be invisible or crash\n");
			break;
		}
	}

	//Make sanity check, it should always be the case that size of vertices does not exceed uint16_t
	if(vertices.size() >= uint16_t(0-1)) {
		printf("ERROR: To many vertices, can't have more vertices than %hu\n", uint16_t(0-1));
	}

	if(colors.size() < vertices.size()) {
		printf("ERROR: Not one color per vertex of model, defaulting to purple\n");
		for(size_t i = colors.size(); i < vertices.size(); i++) {
			this->colors.push_back({255, 0, 255});
		}
	}
}

Model::Model(const Model& model) : Model(model.vertices, model.colors, model.triangleIndices) { }

Model::Model() : Model({}, {}, {}) { }


void Model::subdivide(int i) {
	if(i <= 0) return;
	//Need to split triangle into 4 triangles
	//and then make sure no vertex is stored twice or thrice

	//1. Given three indices, get the three vertices
	size_t size = getNumTriangles();
	for(size_t i = 0; i < size; i++) {
		const glm::uvec3 triangle = triangleIndices[i];
		const vertex_type
			v1 = vertices[triangle.x],
			v2 = vertices[triangle.y],
			v3 = vertices[triangle.z],
			v4 = 0.5f*(v1+v3),
			v5 = 0.5f*(v1+v2),
			v6 = 0.5f*(v2+v3);
		uint16_t i4 = vertices.size();
		vertices.push_back(v4);
		uint16_t i5 = vertices.size();
		vertices.push_back(v5);
		uint16_t i6 = vertices.size();
		vertices.push_back(v6);

		triangleIndices.push_back({i5, triangle.y, i6});
		triangleIndices.push_back({triangle.z, i4, i6});
		triangleIndices.push_back({i4, i5, i6});
		triangleIndices[i] = {triangle.x, i5, i4};

		//Don't forget to add color aswell! (50% lerp)
		colors.push_back(colors[triangle.x]/uint8_t(2) + colors[triangle.z]/uint8_t(2));
		colors.push_back(colors[triangle.x]/uint8_t(2) + colors[triangle.y]/uint8_t(2));
		colors.push_back(colors[triangle.y]/uint8_t(2) + colors[triangle.z]/uint8_t(2));
	}
	subdivide(i-1);
}

bool Model::append(const Model& model) {
	if(triangleIndices.size() + model.getNumTriangles() >= std::numeric_limits<triangle_index_type>::max()) {
		return false;
	}
	const triangle_indices_type increment { static_cast<triangle_index_type>(vertices.size()) }; //Need to increment indices by the number of indices in original model
	vertices.insert(vertices.end(), model.vertices.begin(), model.vertices.end());
	colors.insert(colors.end(), model.colors.begin(), model.colors.end());

	for(const auto& triangle : model.triangleIndices) {
		triangleIndices.push_back(triangle + increment);
	} 

	return true;
}

glm::vec3 Model::triangle_midpoint(std::size_t i) const {
	return (1.0f / 3.0f) * (vertices[triangleIndices[i][0]] + vertices[triangleIndices[i][1]] + vertices[triangleIndices[i][2]]);
}

std::size_t Model::getNumVertices() const {
	return vertices.size();
};

std::size_t Model::getNumTriangles() const {
	return triangleIndices.size();
}

Model::triangle_indices_type Model::getTriangleIndices(std::size_t triangleIdx) const {
	return triangleIndices[triangleIdx];
}

void Model::translate(const glm::vec3& translation) {
	std::transform(std::begin(vertices), std::end(vertices), std::begin(vertices), [&translation](Vertex v) { return v + translation; });
}

void Model::scale(float factor) {
	std::transform(std::begin(vertices), std::end(vertices), std::begin(vertices), [&factor](Vertex v) { return factor * v; });
}

//Transforms all vertices of model by transform
Model& transform(Model& m, const glm::mat4& t) {
	std::transform(m.vertices.begin(), m.vertices.end(), m.vertices.begin(), [&t](const glm::vec3& v) {
		return glm::vec3(t * glm::vec4(v, 1.0f));
	});
	return m;
}

Model join(const std::vector<Model>& models, const std::vector<glm::mat4>& transforms) {
	if(models.size() != transforms.size()) {
		std::cout << "ERROR: The number of models " << models.size() << " is not the same as the number of transforms " << transforms.size() << ", can not join models" << std::endl;
	}
	Model output;
	for(size_t i = 0; i < models.size(); i++) {
		Model cpy = models[i];
		transform(cpy, transforms[i]);
		output.append(cpy);
	}
	return output;
}

// Returns a model consisting of a single triangle {(0,0,0), (1,0,0), (0,1,0)}
Model getSingleTriangleModel() {
	const std::vector<Vertex> vertices {
		Vertex
		{0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
	};

	std::vector<Model::color_type> colors(vertices.size());
	std::fill(std::begin(colors), std::end(colors), Color{0, 0, 0});

	const std::vector<Model::triangle_indices_type> triangleIndices {
		Model::triangle_indices_type
		{0, 1, 2}
	};

	return Model(vertices, colors, triangleIndices);
}

Model getTwoTriangleModel() {
	const std::vector<Vertex> vertices {
		Vertex
		{0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},

		{2.0, 0.0, 0.0},
		{3.0, 0.0, 0.0},
		{2.0, 1.0, 0.0},
	};

	std::vector<Model::color_type> colors(vertices.size());
	std::fill(std::begin(colors), std::end(colors), Model::color_type{0, 0, 0});

	const std::vector<Model::triangle_indices_type> triangleIndices {
		Model::triangle_indices_type
		{0, 1, 2},
		{3, 4, 5},
	};

	return Model(vertices, colors, triangleIndices);
}

}
