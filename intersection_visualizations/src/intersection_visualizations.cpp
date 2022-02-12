
#include "draw_geometric_primitives.hpp"

#include "generators/cylindergenerator.hpp"
#include "generators/spheregenerator.hpp"

#include "geometric_primitives/linesegment.hpp"
#include "geometric_primitives/sphere.hpp"

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

			lowpoly3d::draw(scene, p0);
			lowpoly3d::draw(scene, p1);
			lowpoly3d::draw(scene, lowpoly3d::LineSegment(p0, p1));
			lowpoly3d::draw(scene, s0);
			lowpoly3d::draw(scene, t0);

			iRenderer.offer(scene); //Render a scene
			mDt = std::chrono::high_resolution_clock::now() - start;
		}
	}

	void stop()
	{
		mRunning = false;
	}

private:
	duration_s mDt;

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
	lowpoly3d::Model const triangle_xy(
		{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{255, 255, 255}, {255, 255, 255}, {255, 255, 255}},
		{{0, 1, 2}}
	);

	std::thread visualizerThread([&] { iv.run(renderer); });

	renderer.initialize(&iv, "../shaders/") &&
	renderer.loadModels("sphere", sphere, "cylinder", cylinder, "triangle_xy", triangle_xy) &&
	renderer.run();
	iv.stop();
	visualizerThread.join();
	renderer.terminate();

	return 0;
}