#include "draw_geometric_primitives.hpp"
#include "geometric_primitives/plane.hpp"
#include "glm/gtx/vector_query.hpp"
#include "renderdata.hpp"
#include "drawfeature.hpp"
#include "scene.hpp"
#include "utils/apt_assert.hpp"
#include <glm/ext/vector_relational.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include "utils/glm/glmprint.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> //glm::translate

namespace lowpoly3d {

constexpr float getPointScalingFactor() { return 0.1f; }
constexpr float getLineSegmentScalingFactor() { return 0.01f; }

void draw(Scene& iScene, Arrow const& iArrow)
{
	draw(iScene, iArrow.getCone());
	draw(iScene, iArrow.getCylinder());
}

void draw(Scene& iScene, Cone const& iCone)
{
	// Cone has base at (0, 0, 0) and tip (0, 1, 0)
	RenderDataBuilder rdb;
	rdb.setModel("cone");
	rdb.setShader("normal");

	auto const parametrization_centerline = iCone.getCenterline().parametrization();
	auto const p1 = parametrization_centerline(0.0f);
	auto const p2 = parametrization_centerline(1.0f);

	// See docs on drawing a LineSegment for more info (very much resembles the code below)

	rdb.setTransformationInWorld([&]() {
		auto const y = glm::normalize(p2 - p1);
		auto const x = !glm::areCollinear(y, glm::vec3(0.0f, 1.0f, 0.0f), 1E-4f) ? glm::cross({0.0f, 1.0f, 0.0f}, y) : glm::cross(y, {0.0f, 0.0f, 1.0f});
		auto const z = glm::cross(x, y);
		auto const mat = glm::mat3(
			x * iCone.getRadius(),
			y * iCone.getHeight(),
			z * iCone.getRadius()
		);
		return glm::translate(glm::identity<glm::mat4>(), p1) * glm::mat4x4(mat);
	}());

	iScene.insert(rdb.build());
}

void draw(Scene& iScene, Cylinder const& iCylinder)
{
	// Cylinder has base at (0, 0, 0) and tip (0, 1, 0)
	RenderDataBuilder rdb;
	rdb.setModel("cylinder");
	rdb.setShader("color");

	auto const parametrization_centerline = iCylinder.parametrization_centerline();
	auto const p1 = parametrization_centerline(0.0f);
	auto const p2 = parametrization_centerline(1.0f);

	// See docs on drawing a LineSegment for more info (very much resembles the code below)
	rdb.setTransformationInWorld([&]() {
		auto const y = glm::normalize(p2 - p1);
		auto const x = !glm::areCollinear(y, glm::vec3(0.0f, 1.0f, 0.0f), 1E-4f) ? glm::cross({0.0f, 1.0f, 0.0f}, y) : glm::cross(y, {0.0f, 0.0f, 1.0f});
		auto const z = glm::cross(x, y);
		// We don't want uniform scaling.
		// We want to stretch along y so that we "reach" iLineSegment.p2.
		auto const mat = glm::mat3(
			x * iCylinder.getRadius(),
			y * iCylinder.getHeight(),
			z * iCylinder.getRadius()
		);
		return glm::translate(glm::identity<glm::mat4>(), p1) * glm::mat4x4(mat);
	}());

	iScene.insert(rdb.build());
}

void draw(Scene& iScene, Line const& iLine)
{
	auto const p = iLine.parametrization();
	// TODO: Implement proper "infinite" rendering of iLine.
	// Idea:
	// 1. Project 3D 'iLine' into a 2D line in NDC-space (-1, +1)x(-1, +1)
	// 2. Find the two points that coincide with the iLine and the NDC "borders"
	//    by intersection projected 3D line with these lines:
	//      * bottom_left_to_bottom_right (p=(-1, -1), d=(+1, +0))
	//      * bottom_right_to_top_right   (p=(+1, -1), d=(+0, +1))
	//      * top_right_to_top_left       (p=(+1, +1), d=(-1, +0))
	//      * top_left_to_bottom_left     (p=(-1, +1), d=(+0, -1))
	//    if the intersection-result is anything but two points, don't draw anything.
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
		auto const x = !glm::areCollinear(y, glm::vec3(0.0f, 1.0f, 0.0f), 1E-4f) ? glm::cross({0.0f, 1.0f, 0.0f}, y) : glm::cross(y, {0.0f, 0.0f, 1.0f});
		auto const z = glm::cross(x, y);
		// We don't want uniform scaling.
		// We want to stretch along y so that we "reach" iLineSegment.p2.
		auto const mat = glm::mat3(
			x * getLineSegmentScalingFactor(),
			y * iLineSegment.length(),
			z * getLineSegmentScalingFactor()
		);
		return glm::translate(glm::identity<glm::mat4>(), iLineSegment.p1) * glm::mat4x4(mat);
	}());

	iScene.insert(rdb.build());
}

void draw(Scene& iScene, Parallelogram const& iParallelogram)
{
	draw(iScene, iParallelogram.getFirstTriangle());
	draw(iScene, iParallelogram.getSecondTriangle());
}

void draw(Scene& iScene, Plane const& iPlane)
{
	auto const planeParametrization = iPlane.parametrization();
	
	constexpr float uvside = 0.3f;
	constexpr float uvsidehalf = uvside/2.0f;

	auto const p1 = planeParametrization(-uvsidehalf, -uvsidehalf);
	auto const p2 = planeParametrization(-uvsidehalf, +uvsidehalf);
	auto const p3 = planeParametrization(+uvsidehalf, -uvsidehalf);
	auto const rectangle = Rectangle(p1, p2, p3);
	
	APT_ASSERT_EQ(iPlane.getPoint(), rectangle.midpoint());

	draw(iScene, rectangle);

	auto const arrow = Arrow(Cylinder({iPlane.getPoint(), iPlane.getPoint() + iPlane.getNormal()}, 0.01f));
	draw(iScene, arrow);
}

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

void draw(Scene& iScene, Rectangle const& iRectangle)
{
	draw(iScene, static_cast<Parallelogram const&>(iRectangle));
}

void draw(Scene& iScene, Sphere const& iSphere)
{
	auto const rdb = RenderDataBuilder()
		.setTransformationInWorld(scale(translate(iSphere.p), glm::vec3(iSphere.r)))
		.setModel("sphere")
		.setShader("color");

	iScene.insert(rdb.build());
}

void draw(Scene& iScene, Triangle const& iTriangle)
{
	auto dft = DrawFeatureTarget()
		.setNoFaceCull();

	auto const x = iTriangle.p2 - iTriangle.p1;
	auto const y = iTriangle.p3 - iTriangle.p1;
	auto const z = glm::cross(x, y);
	auto const mat = glm::mat3(x, y, z);

	auto const rdb = RenderDataBuilder()
		.setTransformationInWorld(glm::translate(glm::identity<glm::mat4>(), iTriangle.p1) * glm::mat4x4(mat))
		.setModel("triangle_xy")
		.setShader("normal")
		.setDrawFeatureTarget(std::move(dft));

	iScene.insert(rdb.build());	
}

} // End of namespace lowpoly3d