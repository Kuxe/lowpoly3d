#include "draw_geometric_primitives.hpp"
#include "renderdata.hpp"
#include "drawfeature.hpp"
#include "scene.hpp"
#include <glm/ext/vector_relational.hpp>
#include <glm/geometric.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> //glm::translate

namespace lowpoly3d {

constexpr float getPointScalingFactor() { return 0.1f; }
constexpr float getLineSegmentScalingFactor() { return 0.05f; }

void draw(Scene& iScene, Point const& iPoint)
{
	RenderDataBuilder rdb;
	rdb.setTransformationInWorld(scale(translate(iPoint), glm::vec3(getPointScalingFactor())));
	// Assume that there is a model loaded in engine called "sphere"
	rdb.setModel("sphere");
	// Assume that there is a shader loaded in engine called "noshade"
	rdb.setShader("color");

	iScene.insert(rdb.build());
}

void draw(Scene& iScene, Line const& iLine)
{
	auto const p = iLine.parametrization();
	// TODO: Implement proper "infinite" rendering of iLine.
	// Idea:
	// 1. Project 'iLine' into screenspace
	// 2. Find the two points that coincide with the iLine and the screen "borders",
	//    which is the "visible" portion of the infinite line.
	// 3. Project these two points back into worldspace
	// 4. Draw a LineSegment via the two points in worldspace
	draw(iScene, LineSegment{p(-5.0f), p(5.0f)});
}

void draw(Scene& iScene, LineSegment const& iLineSegment)
{
	// Cylinder has base at (0, 0, 0) and tip (0, 1, 0)
	RenderDataBuilder rdb;
	rdb.setModel("cylinder");
	rdb.setShader("color");

	// Now we want to "align" the cylinder in its model-space
	// (which is just pointing upwards along +y) to the origLineSegment.
	// We can do this by finding a vector that is orthogonal to iLineSegment
	// Lets just cross direction of cylinder (0,1,0) with direction of iLineSegment (gives x-basis)
	// We can then just cross that vector with direction of iLineSegment (gives z-basis)
	// Normalize all vectors. Now we have an ON-basis expressed in model-space of cylinder,
	// if we take y-basis to be unit vector along iLineSegment.
	// BUT first check if we even need to align cylinder in model-space to iLineSegment to begin with
	// (we dont need to do this if iLineSegment x (0,1,0) == 0)
	rdb.setTransformationInWorld([&]() {
		auto const y = glm::normalize(iLineSegment.p2 - iLineSegment.p1);
		if(almostEqual(y, glm::vec3(0.0f, 1.0f, 0.0f))) {
			return glm::translate(iLineSegment.p1);
		} else {
			auto const x = glm::cross({0.0f, 1.0f, 0.0f}, y);
			auto const z = glm::cross(x, y);
			// We don't want uniform scaling.
			// We want to stretch along y so that we "reach" iLineSegment.p2.
			auto const mat = glm::mat3(
				x * getLineSegmentScalingFactor(),
				y * iLineSegment.length(),
				z * getLineSegmentScalingFactor()
			);
			return glm::translate(glm::mat4x4(mat), iLineSegment.p1);
		}
	}());

	iScene.insert(rdb.build());
}

void draw(Scene& iScene, Sphere const& iSphere)
{
	auto const rdb = RenderDataBuilder()
		.setTransformationInWorld(scale(translate(iSphere.p), glm::vec3(iSphere.r)))
		.setModel("sphere")
		.setShader("color");

	iScene.insert(rdb.build());
}

// FIXME: This does NOT respect the shape of the input 'iTriangle',
//        it will draw a triangle with unit-length catheti in the
//        first quadrant of the XY-plane.
void draw(Scene& iScene, Triangle const& iTriangle)
{
	auto dft = DrawFeatureTarget()
		.setNoFaceCull();

	auto const rdb = RenderDataBuilder()
		.setTransformationInWorld(translate(iTriangle.p1))
		.setModel("triangle_xy")
		.setShader("color")
		.setDrawFeatureTarget(std::move(dft));

	iScene.insert(rdb.build());	
}

} // End of namespace lowpoly3d