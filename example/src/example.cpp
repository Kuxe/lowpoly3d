#include "binary_path.hpp"
#include "lowpoly3d.hpp"

/** Example file illustrating how to use lowpoly3d **/
using namespace lowpoly3d;

// TODO: 
// When running from build-tree, use CMAKE_SOURCE_DIR (available from build.hpp) as RESOURCE_DIR
// When running from install-tree, use "../" relative to executable as RESOURCE_DIR
// Then load shaders from "RESOURCE_DIR/shaders/""

struct Game : public ILowpolyInput {
	using duration_s = std::chrono::duration<double, std::ratio<1, 1>>;
	KeyManager keymanager;
	FPSCamera camera;

	bool running = true;
	duration_s dt;

	Game() : camera(keymanager, dt) {}

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
		camera.look(xpos, ypos);
	}

	void run(Renderer& renderer) {
		using namespace std::chrono;
		const auto gametime = [start = high_resolution_clock::now()] {
			return duration_s(high_resolution_clock::now() - start);
		};
		
		/** Map key events to functions **/
		keymanager[GLFW_KEY_ESCAPE].setOnHoldFunction([this]() { running = false; });
		keymanager[GLFW_KEY_Z].setOnPressFunction([this]() { showWireframes = !showWireframes; });

		while(running) {
			// Game logic here - handle input and make a sphere go round and round
			const auto start = high_resolution_clock::now();
			keymanager.execute();
			const double sunRads = std::acos(-1.0) / 30.0 * gametime().count();

			SceneConstants const sceneConstants { camera.view(), sunRads, showWireframes };

			Scene scene { sceneConstants };
			scene.insert(begin(rds), end(rds));

			renderer.offer(scene); //Render a scene
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
	auto const bindir = get_current_binary_absolute_path().parent_path();
	lowpoly3d.initialize(&game, bindir.string() +  "/../shaders/") &&
	lowpoly3d.loadModels("sphere", sphere, "terrain", terrain, "tree", tree) &&
	lowpoly3d.run(); //Main-thread will remain in lowpoly3d.run() until lowpoly3d terminates
	game.running = false; //terminate game and join game thread with main thread
	thread.join();
	lowpoly3d.terminate();
	return 0;
}