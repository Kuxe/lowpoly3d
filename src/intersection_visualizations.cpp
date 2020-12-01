
#include "draw_geometric_primitives.hpp"

#include "generators/spheregenerator.hpp"

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

			lowpoly3d::draw(scene, lowpoly3d::Point(0.0f, 0.0f, 0.0f));

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

	lowpoly3d::Model model = lowpoly3d::SphereGenerator({255, 0, 255}, 3).generate();

	std::thread visualizerThread([&] { iv.run(renderer); });

	renderer.initialize(&iv, "../shaders/") &&
	renderer.loadModels("sphere", model) &&
	renderer.run();
	iv.stop();
	visualizerThread.join();
	renderer.terminate();

	return 0;
}