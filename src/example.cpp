#include "lowpoly3d.hpp"

/** Example file illustrating how to use lowpoly3d.
	Error handling and encapsulation have been omitted for brevity.
	If it's pitch dark, wait until the morning appears! **/
using namespace glm;
using namespace std::chrono;
struct Game : public RenderQuerier, public ILowpolyInput {
	bool running = true;
	float dt = 0.0f;
	Camera camera;
	std::unordered_set<int> heldKeys;
	std::vector<RenderData> rds = {
		{translate(mat4(), vec3(-50.0f, 0.0, -50.0f)), "terrain", "default"},
		{scale(mat4(), vec3(-50.f)), "sphere", "skybox"},
		{mat4(), "sphere", "default"}
	};

	const std::vector<RenderData>& getRenderDatas() const { return rds; }
	const mat4 getView() const { return camera.get(); }
	const float getGametime() const { return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count()/36000.0f; }
	const float getSunRadians() const { return getGametime(); }

	void onKey(int key, int scancode, int action, int mods) {
		if(action == GLFW_PRESS) heldKeys.insert(key);
		else if(action == GLFW_RELEASE) heldKeys.erase(key);
	}

	void onMouse(double xpos, double ypos) { camera.look({xpos, ypos}, dt); }

	void run() {
		while(running) {
			//Game logic here - handle input and make a sphere go round and round
			float start = getGametime();
			for(const int key : heldKeys) {
				switch(key) {
					case GLFW_KEY_W: camera.dolly(-30.0f * dt); break;
					case GLFW_KEY_A: camera.truck(-30.0f * dt); break;
					case GLFW_KEY_S: camera.dolly(+30.0f * dt); break;
					case GLFW_KEY_D: camera.truck(+30.0f * dt); break;
					case GLFW_KEY_Q: camera.pedestal(-30.0f * dt); break;
					case GLFW_KEY_E: camera.pedestal(+30.0f * dt); break;
					case GLFW_KEY_ESCAPE: running = false; break;
				}
			}
			rds[2].modelMatrix[3] = vec4(5.0f*cosf(10.0f*getGametime()), 5.0f, 5.0f*sinf(10.0f*getGametime()), 1.0f);
			signalRenderer(); //Current thread blocked here until renderer is done rendering
			dt = getGametime() - start;
		}
	}
};

int main(int argc, char** argv) {
	Game game; //This is your game class which realizes the RenderQuerier interface
	Renderer renderer; //This is the lowpoly3d renderer
	renderer.initialize(&game); //Can pass game as argument since game inherits from ILowpolyInput
	SphereGenerator sphereGenerator({125.0f, 125.0f, 125.0f}, 3);
	TerrainGenerator terrainGenerator;
	renderer.loadModel("sphere", sphereGenerator.generate());
	renderer.loadModel("terrain", terrainGenerator.generate());
	std::thread thread(&Game::run, &game); //Run game in a thread
	renderer.render(game); //Main-thread will remain in here until renderer terminates
	game.running = false; //Renderer has quit, so terminate simulation and join simulation thread with main thread
	thread.join();
	renderer.terminate();
}