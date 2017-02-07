#include "lowpoly3d.hpp"

/** Example file illustrating how to use lowpoly3d **/
using namespace glm;
using namespace std::chrono;
using namespace lowpoly3d;
struct Game : public RenderQuerier, public ILowpolyInput {
	bool running = true;
	decltype(high_resolution_clock::now()) startTime;
	float dt = 0.0f;
	Camera camera;
	std::unordered_set<int> heldKeys;
	std::vector<RenderData> rds = {
		{translate(mat4(), vec3(-50.0f, 0.0, -50.0f)), "terrain", "default"},
		{scale(mat4(), vec3(-50.f)), "sphere", "skybox"},
		{mat4(), "sphere", "default"}
	};

	const std::vector<RenderData>& getRenderDatas() const {
		return rds;
	}

	const mat4 getView() const {
		return camera.get();
	}

	const float getGametime() const {
		using ms = duration<float, std::milli>;
		return duration_cast<ms>(high_resolution_clock::now() - startTime).count() / 1000.0f;
	}

	const float getSunRadians() const {
		return .1f*getGametime();
	}

	void onKey(int key, int scancode, int action, int mods) {
		if(action == GLFW_PRESS) heldKeys.insert(key);
		else if(action == GLFW_RELEASE) heldKeys.erase(key);
	}

	void onMouse(double xpos, double ypos) {
		camera.look({xpos, ypos}, dt);
	}

	void run() {
		startTime = high_resolution_clock::now();
		while(running) {
			//Game logic here - handle input and make a sphere go round and round
			float gametime = getGametime();
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
			rds[2].modelMatrix[3] = 5.0f*vec4(cosf(gametime), 1.0f, sinf(gametime), .2f);
			signalRenderer(); //Current thread blocked here until renderer is done rendering
			dt = getGametime() - gametime;
		}
	}
};

int main(int argc, char** argv) {
	Game game; //This is your game class which realizes the RenderQuerier interface
	std::thread thread(&Game::run, &game); //Run game in a thread

	/** Initialize renderer and send a sphere and terrain to GPU, then start render the game **/
	SphereGenerator sg({125.0f, 125.0f, 125.0f}, 3);
	TerrainGenerator tg;
	Renderer renderer;
	renderer.initialize(&game, "../shaders/") &&
	renderer.loadModels("sphere", sg.generate(), "terrain", tg.generate()) &&
	renderer.render(game); //Main-thread will remain in here until renderer terminates
	game.running = false; //Renderer has quit, so terminate game and join game thread with main thread
	thread.join();
	renderer.terminate();
	return 0;
}