#include "lowpoly3d.hpp"

/** Example file illustrating how to use lowpoly3d **/
using namespace lowpoly3d;

struct Game : public ILowpolyInput {
	using duration_s = std::chrono::duration<double, std::ratio<1, 1>>;
	KeyManager keymanager;
	Camera camera;

	bool running = true;
	duration_s dt;
	double cameraSpeed = 3.0;

	std::vector<RenderData> rds = {
		{translate(glm::mat4(1.0f), -100.0f*glm::vec3(1.0f, 0.0, 1.0f)), "terrain", "default"},
		{scale(glm::mat4(1.0f), glm::vec3(-150.f)), "sphere", "skybox"}
	};

	bool showWireframes = false;

	void onKey(int key, int scancode, int action, int mods) {
		if(action == GLFW_PRESS) keymanager.pressed(key);
		else if(action == GLFW_RELEASE)	keymanager.released(key);
	}

	void onMouse(double xpos, double ypos) {
		const double radsPerPixel = std::acos(-1) / 2000.0;
		camera.look({xpos, ypos}, cameraSpeed * radsPerPixel);
	}

	void run(Renderer& renderer) {
		using namespace std::chrono;
		const auto gametime = [start = high_resolution_clock::now()] {
			return duration_s(high_resolution_clock::now() - start);
		};

		// Setup keybindings
		KeyAction keyw, keya, keys, keyd, keyq, keye, keyescape, keyz, keyleftshift;
		keyw.setOnHoldFunction([this]() { camera.dolly(+cameraSpeed * dt.count()); });
		keys.setOnHoldFunction([this]() { camera.dolly(-cameraSpeed * dt.count()); });
		keya.setOnHoldFunction([this]() { camera.truck(-cameraSpeed * dt.count()); });
		keyd.setOnHoldFunction([this]() { camera.truck(+cameraSpeed * dt.count()); });
		keyq.setOnHoldFunction([this]() { camera.pedestal(-cameraSpeed * dt.count()); });
		keye.setOnHoldFunction([this]() { camera.pedestal(+cameraSpeed * dt.count()); });
		keyescape.setOnHoldFunction([this]() { running = false; });
		keyz.setOnPressFunction([this]() { showWireframes = !showWireframes; });
		keyleftshift
		.setOnPressFunction([this]() { cameraSpeed = 10.0f; })
		.setOnReleaseFunction([this]() { cameraSpeed = 3.0f; });

		/** Bind key actions to GLFW key identifiers **/
		keymanager
		.bind(GLFW_KEY_W, keyw).bind(GLFW_KEY_S, keys).bind(GLFW_KEY_A, keya)
		.bind(GLFW_KEY_D, keyd).bind(GLFW_KEY_Q, keyq).bind(GLFW_KEY_E, keye)
		.bind(GLFW_KEY_Z, keyz).bind(GLFW_KEY_LEFT_SHIFT, keyleftshift);

		while(running) {
			// Game logic here - handle input and make a sphere go round and round
			const auto start = high_resolution_clock::now();
			keymanager.execute();
			const double sunRads = std::acos(-1.0) / 30.0 * gametime().count();
			renderer.offer({rds, camera.view(), sunRads, showWireframes}); //Render a scene
			dt = high_resolution_clock::now() - start;
		}
	}
};

int main(int argc, char** argv) {
	/** Create some 3d-model generators and generate geometries **/
	SphereGenerator sg({125.0f, 125.0f, 125.0f}, 0);
	TerrainGenerator tg;
	TreeGenerator treeg;
	Model sphere = sg.generate(), terrain = tg.generate(), tree = treeg.generate();

	/** Start your game using the lowpoly3d renderer **/
	Game game;
	Renderer lowpoly3d;
	std::thread thread([&] { game.run(lowpoly3d); });

	/** Tell the renderer to render the game, using shaders within the ../shaders/ directory.
		Proceed to load 3D-meshes into GPU-memory if initialization went well
		Finally, run the lowpoly3d-renderer if everything went well. **/
	lowpoly3d.initialize(&game, "../shaders/") &&
	lowpoly3d.loadModels("sphere", sphere, "terrain", terrain, "tree", tree) &&
	lowpoly3d.run(); //Main-thread will remain in lowpoly3d.run() until lowpoly3d terminates
	game.running = false; //terminate game and join game thread with main thread
	thread.join();
	lowpoly3d.terminate();
	return 0;
}