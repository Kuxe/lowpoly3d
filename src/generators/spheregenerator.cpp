#include "generators/spheregenerator.hpp"
#include "generators/cubegenerator.hpp"
#include <glm/ext.hpp>

namespace lowpoly3d {

SphereGenerator::SphereGenerator(const Color& color, uint8_t subdivides) : color(color), subdivides(subdivides) { }

Model SphereGenerator::generate() {
	CubeGenerator cg(color);
	Model sphere = cg.generate();
	sphere.subdivide(subdivides);
	for(auto& vertex : sphere.vertices) {
		vertex = vertex / glm::length(vertex);
	}
	return sphere;
}

Model SphereGenerator::generate(const Sphere& sphere) {
	Model ret = generate();
	ret.subdivide(subdivides);
	ret.scale(sphere.r);
	ret.translate(sphere.p);
	return ret;
}

}
