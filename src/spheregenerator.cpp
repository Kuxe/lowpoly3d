#include "spheregenerator.hpp"
#include "cubegenerator.hpp"
#include <glm/glm.hpp>
#include <algorithm>

namespace lowpoly3d {

SphereGenerator::SphereGenerator(const Color& color, uint8_t subdivides) : color(color), subdivides(subdivides) { }

Model SphereGenerator::generate() {
	CubeGenerator cg(color);
	Model sphere = cg.generate();
	sphere.subdivide(subdivides);
	auto& vertices = sphere.vertices;
	std::transform(vertices.begin(), vertices.end(), vertices.begin(),
		[](const glm::vec3& v) { return v / glm::length(v); }
	);
	return sphere;
}

}
