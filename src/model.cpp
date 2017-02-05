#include <stdio.h>
#include "model.hpp"

namespace lowpoly3d {

Model::Model(
	const std::vector<Vertex>& vertices,
	const std::vector<Color>& colors,
	const std::vector<Triangle>& triangles) : 
	vertices(vertices),
	colors(colors),
	triangles(triangles) {

	//Make sanity check, it should always be the case that max index in triangles is <= than number of vertices
	for(const auto& triangle : triangles) {
		uint16_t maxIndex = 0;
		maxIndex = std::max(triangle.x, maxIndex);
		maxIndex = std::max(triangle.y, maxIndex);
		maxIndex = std::max(triangle.z, maxIndex);
		if(maxIndex >= vertices.size()) {
			printf("ERROR: An index refers to non-existent vertex, either model will be invisible or crash\n");
			break;
		}
	}

	if(colors.size() < vertices.size()) {
		printf("ERROR: Not one color per vertex of model, defaulting to purple\n");
		for(size_t i = colors.size(); i < vertices.size(); i++) {
			this->colors.push_back({255, 0, 255});
		}
	}
}


void Model::subdivide(int i) {
	if(i <= 0) return;
	//Need to split triangle into 4 triangles
	//and then make sure no vertex is stored twice or thrice

	//1. Given three indices, get the three vertices
	size_t size = triangles.size();
	for(size_t i = 0; i < size; i++) {
		const glm::uvec3 triangle = triangles[i];
		const Vertex v1 = vertices[triangle.x];
		const Vertex v2 = vertices[triangle.y];
		const Vertex v3 = vertices[triangle.z];
		const Vertex v4 = 0.5f*(v1+v3);
		const Vertex v5 = 0.5f*(v1+v2);
		const Vertex v6 = 0.5f*(v2+v3);
		uint16_t i4 = vertices.size();
		vertices.push_back(v4);
		uint16_t i5 = vertices.size();
		vertices.push_back(v5);
		uint16_t i6 = vertices.size();
		vertices.push_back(v6);

		triangles.push_back({i5, triangle.y, i6});
		triangles.push_back({triangle.z, i4, i6});
		triangles.push_back({i4, i5, i6});
		triangles[i] = {triangle.x, i5, i4};

		//Don't forget to add color aswell! (50% lerp)
		colors.push_back(colors[triangle.x]/uint8_t(2) + colors[triangle.z]/uint8_t(2));
		colors.push_back(colors[triangle.x]/uint8_t(2) + colors[triangle.y]/uint8_t(2));
		colors.push_back(colors[triangle.y]/uint8_t(2) + colors[triangle.z]/uint8_t(2));
	}
	subdivide(i-1);
}

}
