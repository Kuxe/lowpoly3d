
#include "draw_geometric_primitives.hpp"

#include "generators/conegenerator.hpp"
#include "generators/cylindergenerator.hpp"
#include "generators/spheregenerator.hpp"

#include "geometric_primitives/arrow.hpp"
#include "geometric_primitives/cylinder.hpp"
#include "geometric_primitives/intersections.hpp"
#include "geometric_primitives/intersects.hpp"
#include "geometric_primitives/line.hpp"
#include "geometric_primitives/linesegment.hpp"
#include "geometric_primitives/plane.hpp"
#include "geometric_primitives/point.hpp"
#include "geometric_primitives/rectangle.hpp"
#include "geometric_primitives/sphere.hpp"
#include "geometric_primitives/triangle.hpp"

#include "minimum_bounding_sphere.hpp"

#include "lowpoly3d.hpp"
#include "model.hpp"

class IntersectionVisualizer : public lowpoly3d::ILowpolyInput
{
public:
	using duration_s = std::chrono::duration<double, std::ratio<1, 1>>;

	void onKey(int key, int scancode, int action, int mods) override
	{
		if(action == GLFW_PRESS) mKeyManager.pressed(key);
		else if(action == GLFW_RELEASE)	mKeyManager.released(key);
	}

	void onMouse(double xpos, double ypos) override {
		const double radsPerPixel = std::acos(-1) / 2000.0;
		mCamera.look(xpos, ypos);
	}

	void run(lowpoly3d::Renderer& iRenderer)
	{
		mRunning = true;
		auto const runstart = std::chrono::high_resolution_clock::now();
		while(mRunning)
		{
			auto const start = std::chrono::high_resolution_clock::now();
			mKeyManager.execute();

			lowpoly3d::SceneConstants sceneConstants { mCamera.view(), std::acos(-1.0) / 2.0, false };
			lowpoly3d::Scene scene { sceneConstants };

			auto const p0 = lowpoly3d::Point(0.0f, 0.0f, 0.0f);
			auto const p1 = lowpoly3d::Point(-1.0f, 1.0f, -1.0f);
			auto const s0 = lowpoly3d::Sphere({10.0f, 0.0f, 0.0f}, 2.0f);
			auto const t0 = lowpoly3d::Triangle({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
			auto const t0mbs = lowpoly3d::mbs(t0);
			auto const l0 = lowpoly3d::Line({-5.0f, -5.0f, 0.0f}, {0.0f, 0.0f, 1.0f});

			lowpoly3d::draw(scene, p0);
			lowpoly3d::draw(scene, p1);
			lowpoly3d::draw(scene, lowpoly3d::LineSegment(p0, p1));
			lowpoly3d::draw(scene, s0);
			lowpoly3d::draw(scene, t0);
			lowpoly3d::draw(scene, t0.p1);
			lowpoly3d::draw(scene, t0.p2);
			lowpoly3d::draw(scene, t0.p3);
			lowpoly3d::draw(scene, t0mbs);

			// Draw a triangle that is later intersected with several lines arranged in a grid
			auto const t1 = lowpoly3d::Triangle(
				0.1f*std::sin(1.07f * mElapsed.count()) * lowpoly3d::Point{20.0f, 3.0f, -2.0f} + lowpoly3d::Point{20.0f, 3.0f, -2.0f},
				0.1f*std::sin(1.13f * mElapsed.count()) * lowpoly3d::Point{15.0f, 2.0f, 10.0f} + lowpoly3d::Point{15.0f, 2.0f, 10.0f},
				0.1f*std::sin(1.17f * mElapsed.count()) * lowpoly3d::Point{22.0f, -4.0f, 5.0f} + lowpoly3d::Point{22.0f, -4.0f, 5.0f}
			);

			lowpoly3d::draw(scene, t1);

			// Draw lines in a grid and intersect each lines against triangle
			for(float fx = 0.0f; fx < 10.0f; fx += 1.0f)
			{
				for(float fz = 0.0f; fz < 10.0f; fz += 1.0f)
				{
					auto const lineorigin = lowpoly3d::Point{15.0f + fx, 0.0f, fz};
					// Draw line parallel with world's x-axis and optionally draw collision-point
					auto const lx = lowpoly3d::Line(lineorigin, {1.0f, 0.0f, 0.0f});
					lowpoly3d::draw(scene, lx);
					if(auto const p = lowpoly3d::intersection(t1, lx); p.x != NAN && p.y != NAN && p.z != NAN)
					{
						lowpoly3d::draw(scene, p);
					}

					// Draw line parallel with world's z-axis and optionally draw collision-point
					auto const lz = lowpoly3d::Line(lineorigin, {0.0f, 0.0f, 1.0f});
					lowpoly3d::draw(scene, lz);
					if(auto const p = lowpoly3d::intersection(t1, lz); p.x != NAN && p.y != NAN && p.z != NAN)
					{
						lowpoly3d::draw(scene, p);
					}
				}
			}

			auto const t2  = lowpoly3d::Triangle({10.0f, 10.0f, 10.0f}, {0.0f, 20.0f, 10.0f}, {-10.0f, 10.0f, 10.0f});
			lowpoly3d::draw(scene, t2);

			auto const r1 = lowpoly3d::Rectangle({15.0f, 0.0f, 0.0f}, {24.0f, 0.0f, 0.0f}, {15.0f, 5.0f, 0.0f});
			lowpoly3d::draw(scene, r1);

			auto const c1 = lowpoly3d::Cylinder(lowpoly3d::LineSegment({15.0f, 7.0f, 0.0f}, {24.0f, 7.0f, 0.0f}), 1.0f);
			lowpoly3d::draw(scene, c1);

			auto const c2 = lowpoly3d::Cylinder(lowpoly3d::LineSegment({15.0f, 7.0f, 0.0f}, {10.0f, 10.0f, 10.0f}), 0.5f);
			lowpoly3d::draw(scene, c2);

			auto const cone1 = lowpoly3d::Cone(lowpoly3d::Cone(lowpoly3d::LineSegment({0.0f, 0.0f, -5.0f}, {0.0f, 1.0f, -5.0f}), 0.5f));
			auto const cone2 = lowpoly3d::Cone(lowpoly3d::Cone(lowpoly3d::LineSegment({0.0f, 3.0f, -5.0f}, {0.0f, 1.0f, -5.0f}), 1.0f));
			lowpoly3d::draw(scene, cone1);
			lowpoly3d::draw(scene, cone2);

			auto const arrow1 = lowpoly3d::Arrow(lowpoly3d::Cylinder(lowpoly3d::LineSegment({0.0f, 0.0f, -7.0f}, {0.0f, 1.0f, -7.0f}), 0.5f)); // Bulky
			auto const arrow2 = lowpoly3d::Arrow(lowpoly3d::Cylinder(lowpoly3d::LineSegment({2.0f, 0.0f, -7.0f}, {4.0f, 1.0f, -7.0f}), 1.0f)); // Bulky & tilted
			auto const arrow3 = lowpoly3d::Arrow(lowpoly3d::Cylinder(lowpoly3d::LineSegment({4.0f, 3.5f, -7.0f}, {4.0f, 2.0f, -7.0f}), 0.25f)); // Pretty & upside-down
			auto const arrowtarget = lowpoly3d::Sphere({4.0f, 1.0f, -7.0f}, 0.05f); // TODO: Create this as intersection of lines extended from line-segments of arrows
			lowpoly3d::draw(scene, arrow1);
			lowpoly3d::draw(scene, arrow2);
			lowpoly3d::draw(scene, arrow3);
			lowpoly3d::draw(scene, arrowtarget);

			auto const planePoint = lowpoly3d::Point(-5.0f, 0.0f, +5.0f);
			auto const yzplane = lowpoly3d::Plane(planePoint, {1.0f, 0.0f, 0.0f});
			auto const xzplane = lowpoly3d::Plane(planePoint, {0.0f, 1.0f, 0.0f});
			auto const xyplane = lowpoly3d::Plane(planePoint, {0.0f, 0.0f, 1.0f});

			auto const animatedPlaneX = lowpoly3d::Plane(
				planePoint + glm::vec3{-1.0f, 0.0f, 0.0f},
				glm::vec3(
					0.0f,
					std::cos(0.5f*mElapsed.count()),
					std::sin(0.5f*mElapsed.count())
				)
			);

			auto const animatedPlaneY = lowpoly3d::Plane(
				planePoint + glm::vec3{-2.0f, 1.0f, 0.0f},
				glm::vec3(
					std::sin(0.5f*mElapsed.count()),
					0.0f,
					std::cos(0.5f*mElapsed.count())
				)
			);

			auto const animatedPlaneZ = lowpoly3d::Plane(
				planePoint + glm::vec3{-2.0f, 0.0f, 1.0f},
				glm::vec3(
					std::cos(0.5f*mElapsed.count()),
					std::sin(0.5f*mElapsed.count()),
					0.0f
				)
			);

			lowpoly3d::draw(scene, yzplane);
			lowpoly3d::draw(scene, xzplane); // FIXME: This is not visible?
			lowpoly3d::draw(scene, xyplane);
			lowpoly3d::draw(scene, animatedPlaneX);
			lowpoly3d::draw(scene, animatedPlaneY);
			lowpoly3d::draw(scene, animatedPlaneZ);

			// Circumcenter visualizations
			[&scene](){
				glm::vec3 offset = {0.0f, -5.0f, 0.0f};
				auto const t1 = lowpoly3d::Triangle({3.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f},{-3.0f, 0.0f, 0.0f});
				lowpoly3d::draw(scene, lowpoly3d::Triangle(t1.p1 + offset, t1.p2 + offset, t1.p3 + offset));
				lowpoly3d::draw(scene, lowpoly3d::circumcenter(t1) + offset);
			}();

			//auto const intersection_line = intersection(t1, r1.getFirstTriangle());

			iRenderer.offer(scene); //Render a scene
			mDt = std::chrono::high_resolution_clock::now() - start;
			mElapsed = std::chrono::high_resolution_clock::now() - runstart;
			
		}
	}

	void stop()
	{
		mRunning = false;
	}

private:
	duration_s mDt;
	duration_s mElapsed;

	lowpoly3d::KeyManager mKeyManager;
	lowpoly3d::FPSCamera mCamera {mKeyManager, mDt};

	double mCameraSpeed = 3.0;

	bool mRunning = true;
};

int main()
{
	lowpoly3d::Renderer renderer;
	IntersectionVisualizer iv;

	lowpoly3d::Model sphere = lowpoly3d::SphereGenerator({200, 0, 200}, 3).generate();
	lowpoly3d::Model cylinder = lowpoly3d::CylinderGenerator({255, 50, 255}, 16).generate();
	lowpoly3d::Model cone = lowpoly3d::ConeGenerator(lowpoly3d::Cone(lowpoly3d::LineSegment({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}), 1.0f), 24).generate();
	lowpoly3d::Model const triangle_xy(
		{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
		{{0, 1, 2}}
	);

	std::thread visualizerThread([&] { iv.run(renderer); });

	renderer.initialize(&iv, "../shaders/") &&
	renderer.loadModels("sphere", sphere, "cylinder", cylinder, "triangle_xy", triangle_xy, "cone", cone) &&
	renderer.run();
	iv.stop();
	visualizerThread.join();
	renderer.terminate();

	return 0;
}