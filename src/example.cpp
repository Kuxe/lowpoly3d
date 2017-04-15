#include "lowpoly3d.hpp"

/** Example file illustrating how to use lowpoly3d **/
using namespace std::chrono;
using namespace lowpoly3d;
struct Game : public ILowpolyInput {
	bool running = true;
	float dt = 0.0f;
	Camera camera;
	std::unordered_set<int> heldKeys;
	std::vector<RenderData> rds = {
		{translate(glm::mat4(), -100.0f*glm::vec3(1.0f, 0.0, 1.0f)), "terrain", "default"},
		{scale(glm::mat4(), glm::vec3(-150.f)), "sphere", "skybox"},
		{glm::mat4(), "sphere", "default"}
	};

	void onKey(int key, int scancode, int action, int mods) {
		if(action == GLFW_PRESS) heldKeys.insert(key);
		else if(action == GLFW_RELEASE) heldKeys.erase(key);
	}

	void onMouse(double xpos, double ypos) {
		camera.look({xpos, ypos}, dt);
	}

	void run(Renderer& renderer) {
		using ms = duration<float, std::milli>;
		const auto start = high_resolution_clock::now();
		while(running) {
			//Game logic here - handle input and make a sphere go round and round
			const auto gametime = [start] {
				return duration_cast<ms>(high_resolution_clock::now() - start).count() / 1000.0f;
			};
			const float gt = gametime();
			for(const int key : heldKeys) {
				switch(key) {
					case GLFW_KEY_W: camera.dolly(-3.0f * dt); break;
					case GLFW_KEY_A: camera.truck(-3.0f * dt); break;
					case GLFW_KEY_S: camera.dolly(+3.0f * dt); break;
					case GLFW_KEY_D: camera.truck(+3.0f * dt); break;
					case GLFW_KEY_Q: camera.pedestal(-3.0f * dt); break;
					case GLFW_KEY_E: camera.pedestal(+3.0f * dt); break;
					case GLFW_KEY_ESCAPE: running = false; break;
				}
			}
			rds[2].modelMatrix[3] = 5.0f*glm::vec4(cosf(gt), 1.0f, sinf(gt), .2f);
			rds[1].modelMatrix[3] = camera.eye;
			renderer.setScene({rds, camera.get(), gt, .1f*gt}); //Construct and render a scene
			std::this_thread::sleep_for(1ms); //Workaround to prevent dt=0.0f
			dt = gametime() - gt;
		}
	}
};

int main(int argc, char** argv) {
	/** Create game and renderer objects and start running the game on new thread,
		using lowpoly3d as the game renderer. Notice that we haven't initialized
		renderer yet, so nothing will be rendered just yet **/
	Game game;
	Renderer lowpoly3d;
	std::thread thread([&] { game.run(lowpoly3d); });

	/** Create some 3d-model generators and initialize lowpoly3d with generated model **/
	SphereGenerator sg({125.0f, 125.0f, 125.0f}, 3);
	TerrainGenerator tg;
	lowpoly3d.initialize(&game, "../shaders/") &&
	lowpoly3d.loadModels("sphere", sg.generate(), "terrain", tg.generate()) &&
	lowpoly3d.run(); //Main-thread will remain in lowpoly3d.run() until lowpoly3d terminates
	game.running = false; //lowpoly3d has quit, so terminate game and join game thread with main thread
	thread.join();
	lowpoly3d.terminate();
	return 0;
}