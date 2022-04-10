
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
#include "geometric_primitives/oriented_plane.hpp"
#include "geometric_primitives/parallelogram.hpp"
#include "geometric_primitives/plane.hpp"
#include "geometric_primitives/point.hpp"
#include "geometric_primitives/rectangle.hpp"
#include "geometric_primitives/sphere.hpp"
#include "geometric_primitives/triangle.hpp"

#include "minimum_bounding_sphere.hpp"

#include "utils/glm/vector_projection.hpp"

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

			// 3D Circumcenter visualizations
			[this, &scene](){

				auto const offset = glm::vec3{0.0f, -6.0f, 0.0f};
				auto const p1 = lowpoly3d::Point{0.0f, 0.0f, 0.0f} + offset;
				auto const p2 = lowpoly3d::Point{4.0f, 0.0f, 0.0f} + offset;
				auto const p3 = lowpoly3d::Point{0.0f, 0.0f, -3.0f} + offset;
				auto const t = lowpoly3d::Triangle(
					0.1f*std::sin(1.07f * mElapsed.count()) * p1 + p1,
					0.1f*std::sin(1.13f * mElapsed.count()) * p2 + p2,
					0.1f*std::sin(1.17f * mElapsed.count()) * p3 + p3
				);
				auto const circumcenter = lowpoly3d::circumcenter(t);

				lowpoly3d::draw(scene, t);
				lowpoly3d::draw(scene, lowpoly3d::Arrow({0.0f, -3.0f, 0.0f}, lowpoly3d::Point(circumcenter), 0.1f));
				lowpoly3d::draw(scene, circumcenter);

				// Draw bisectors of animated triangle
				[&scene](lowpoly3d::Triangle const& t){
					lowpoly3d::draw(scene, lowpoly3d::Arrow(0.5f*(t.p1 + t.p2), lowpoly3d::Direction(lowpoly3d::edge_normal_12(t)), 0.03f));
					lowpoly3d::draw(scene, lowpoly3d::Arrow(0.5f*(t.p2 + t.p3), lowpoly3d::Direction(lowpoly3d::edge_normal_23(t)), 0.03f));
					lowpoly3d::draw(scene, lowpoly3d::Arrow(0.5f*(t.p3 + t.p1), lowpoly3d::Direction(lowpoly3d::edge_normal_31(t)), 0.03f));
				}(t);

				// Draw oriented plane of animated triangle
				[&scene](lowpoly3d::Triangle const& t){
					lowpoly3d::draw(scene, lowpoly3d::orientedParallel(t));
				}(t);
			}();

			// 2D Circumcenter visualizations
			[this, &scene]()
			{
				// Animate a triangle that never degenerates into a line or point.
				// This ensures that circumcenter visualization never blows off into
				// infinity, which may confuse me in the future.
				auto const triangle2d = lowpoly3d::Triangle2(
					{0.0f, 0.0f},
					{1.5f + 0.25f*std::cos(0.13f*mElapsed.count()), 0.0f + 0.25f*std::sin(0.17f*mElapsed.count())},
					{0.0f + 0.25f*std::sin(0.23f*mElapsed.count()), 1.5f + 0.25f*std::cos(0.29f*mElapsed.count())}
				);
				auto const circumcenter2d = lowpoly3d::circumcenter(triangle2d);

				auto const visualizationOffset = lowpoly3d::Point({-2.0f, -4.0f, 0.0f});

				// Lift point in 2d to point in 3d with supplementary z
				auto const pointlift = [](lowpoly3d::Point2 const& p, float z)
				{
					return lowpoly3d::Point{p.x, p.y, z};
				};

				// Lift triangle in 2d to triangle in 3d with supplementary z
				auto const trianglelift = [&pointlift](lowpoly3d::Triangle2 const& p, float z)
				{
					return lowpoly3d::Triangle(pointlift(p.p1, z), pointlift(p.p2, z), pointlift(p.p3, z));
				};

				auto const linelift = [&pointlift](lowpoly3d::Line2 const& l, float pz, float lz)
				{
					return lowpoly3d::Line(pointlift(l.getPoint(), pz), pointlift(l.getDirection(), lz));
				};

				auto const lineoffset = [](lowpoly3d::Line const& line, lowpoly3d::Point const& offset)
				{
					return lowpoly3d::Line(line.getPoint() + offset, line.getDirection());
				};

				auto const triangle3d = [&triangle2d, &visualizationOffset, &trianglelift](){
					lowpoly3d::Triangle t = trianglelift(triangle2d, 0.0f);
					t.p1 += visualizationOffset;
					t.p2 += visualizationOffset;
					t.p3 += visualizationOffset;
					return t;
				}();
					
				auto const circumcenter3d = lowpoly3d::Point({
					circumcenter2d.x + visualizationOffset.x,
					circumcenter2d.y + visualizationOffset.y,
					0.0f + visualizationOffset.z}
				);

				// These visualizations show that circumcenter calculations in 2D are OK!
				lowpoly3d::draw(scene, triangle3d);
				lowpoly3d::draw(scene, circumcenter3d);
				lowpoly3d::draw(scene, lineoffset(linelift(lowpoly3d::bisector_01(triangle2d), 0.0f, 0.0f), visualizationOffset));
				lowpoly3d::draw(scene, lineoffset(linelift(lowpoly3d::bisector_12(triangle2d), 0.0f, 0.0f), visualizationOffset));
				lowpoly3d::draw(scene, lineoffset(linelift(lowpoly3d::bisector_20(triangle2d), 0.0f, 0.0f), visualizationOffset));
			}();

			// Vector projections
			[this, &scene](){
				auto const offset = lowpoly3d::Point(2.0f, -5.0f, -3.0f);
				auto const projectee1 = lowpoly3d::Point(std::cos(mElapsed.count()) * 4.0f, std::sin(mElapsed.count()) * 3.0f, 0.0f);
				auto const v1 = lowpoly3d::Point(std::cos(mElapsed.count()), 0.0f, std::sin(mElapsed.count()));
				auto const projection1 = projectOnto(projectee1, v1);

				lowpoly3d::draw(scene, lowpoly3d::LineSegment(offset, projectee1+offset));
				lowpoly3d::draw(scene, lowpoly3d::LineSegment(offset, v1+offset));
				lowpoly3d::draw(scene, projection1+offset);
			}();

			[&scene](){
				auto const offset = lowpoly3d::Point(0.0f, -5.0f, -7.0f);
				auto const ls1 = lowpoly3d::LineSegment(
					offset + lowpoly3d::Point{0.0f, 0.0f, 0.0f},
					offset + lowpoly3d::Point{1.0f, 1.0f, 1.0f}
				);
				
				lowpoly3d::draw(scene, ls1);
				for(auto const& vertex : vertices(ls1))
				{
					lowpoly3d::draw(scene, vertex);
				}
			}();

			[&scene](){
				auto const offset = lowpoly3d::Point(2.0f, -5.0f, -7.0f);
				auto const parallelogram = lowpoly3d::Parallelogram(
					offset + lowpoly3d::Point{0.0f, 0.0f, 0.0f},
					offset + lowpoly3d::Point{0.0f, 0.0f, 1.0f},
					offset + lowpoly3d::Point{0.0f, 1.0f, 2.0f}
				);
				
				lowpoly3d::draw(scene, parallelogram);
				for(auto const& vertex : vertices(parallelogram))
				{
					lowpoly3d::draw(scene, vertex);
				}
			}();

			// Visualize line2d-line2d intersection
			[&scene, this](){
				auto const offset = lowpoly3d::Point(2.0f, -5.0f, -9.0f);

				auto const l1 = lowpoly3d::Line2(
					lowpoly3d::Point2f(std::cos(mElapsed.count()) + 1.0f, std::sin(mElapsed.count()) + 1.0f),
					{1.13f * std::cos(mElapsed.count()), 1.17f * std::sin(mElapsed.count())}
				);

				auto const l2 = lowpoly3d::Line2(
					lowpoly3d::Point2f(std::sin(mElapsed.count()) - 1.0f, std::cos(mElapsed.count()) - 1.0f),
					{1.13f * std::sin(mElapsed.count()), 1.17f * std::cos(mElapsed.count())}
				);

				auto line2d_to_line3d = [](lowpoly3d::Line2 const& l)
				{
					return lowpoly3d::Line(lowpoly3d::Point{l.getPoint().x, l.getPoint().y, -9.0f}, lowpoly3d::Point{l.getDirection().x, l.getDirection().y, 0.0f});
				};

				auto const l1_3d = line2d_to_line3d(l1);
				auto const l2_3d = line2d_to_line3d(l2);
				auto const intersection2d = intersection(l1, l2);
				auto const intersection3d = lowpoly3d::Point{intersection2d.x, intersection2d.y, -9.0f};

				lowpoly3d::draw(scene, l1_3d);
				lowpoly3d::draw(scene, l2_3d);
				lowpoly3d::draw(scene, intersection3d);
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