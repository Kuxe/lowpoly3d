#include "generators/conegenerator.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"
#include "modeldefs.hpp"
#include "utils/apt_assert.hpp"

#include <glm/ext.hpp> //glm::quarter_pi

namespace lowpoly3d
{
	ConeGenerator::ConeGenerator(Cone const& cone, std::uint32_t resolution, Color const& color)
			: cone(cone)
			, resolution(resolution)
			, color(color)
	{
		APT_ASSERT_GEQ(resolution, std::uint32_t(3)); // Disallow "flat" cones aka triangles
	}

	Model ConeGenerator::generate()
	{
		auto const height = cone.getHeight();
		auto const parametrization = cone.parametrization();

		// Create vertices
		std::vector<Vertex> vertices;
		// Need to reserve, not just for performance, but for references to be valid
		vertices.reserve(2 + resolution);

		vertices.emplace_back(0.0f, 0.0f, 0.0f); // Base-center-vertex
		vertices.emplace_back(0.0f, height, 0.0f); // Tip-vertex

		// The special-case resolution=4 should give a pyramid that is aligned with
		// with it's local frame. However, the cone parametrization by default gives
		// a vertex on the x-axis for rad=0 since cos(0) = 1. This means that a
		// generated pyramid will be "oriented" 45 degrees.
		// Compensate for that by rotating back the pyramid 45 degrees,
		// which is to say, initialize rad as -pi/4.
		float rad = -glm::quarter_pi<float>();
		float const radianIncrement = 2.0f*glm::pi<float>()/float(resolution);
		for(std::uint32_t i = 0; i < resolution; ++i)
		{
			rad += radianIncrement;
			vertices.emplace_back(parametrization(rad, 0.0f));
		}
		APT_ASSERT_EQ(vertices.size(), 2 + resolution);

		// Connect vertices into triangles
		std::vector<TriangleIndices> indices;
		indices.reserve(2*resolution);

		for(std::uint32_t i = 0; i < resolution-1; ++i)
		{
			indices.emplace_back(2+i, 3+i, 0); // Base-triangle
			indices.emplace_back(3+i, 2+i, 1); // Surface-triangle
		}
		// Tie together the seam
		indices.emplace_back(resolution+1, 2, 0); // Base-triangle
		indices.emplace_back(2, resolution+1, 1); // Surface-triangle

		std::vector<Color> colors(vertices.size(), color);

		// The expected number of vertices is base-center-vertex, tip-vertex and
		// #numFaces vertices at the base circumference.
		APT_ASSERT_EQ(vertices.size(), colors.size());
		return {vertices, colors, indices};
	}
}