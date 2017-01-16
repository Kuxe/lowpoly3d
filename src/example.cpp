#include <thread>
#include <chrono>
#include "lowpoly3d.hpp"

/** Example file illustrating how to use lowpoly3d.
	Error handling and encapsulation have been omitted for brevity **/

using namespace std::chrono;
struct Game : public RenderQuerier, public ILowpolyInput {
	bool running = true, focused = false;
	float mousex, mousey;
	Camera camera;

	std::vector<RenderData> rds = {
		{glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)), glm::vec3(-50.0f, 0.0, -50.0f)), "terrain", "default"},
		RenderData(glm::scale(glm::mat4(1.0f), glm::vec3(-100.f)), "sphere", "skybox"),
		RenderData(glm::mat4(1.0), "sphere", "default")
	};

	//Methods inherited from RenderQuerier which must be realized
	const std::vector<RenderData>& getRenderDatas() const { return rds; }
	const glm::mat4 getView() const { return camera.get(); }
	const float getGametime() const { return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count()/36000.0f; }
	const float getSunRadians() const { return getGametime(); }

	//Methods inherited from ILowpolyInpit which must be realized
	void onError() { running = false; }
	void onFramebufferResize(int width, int height) { /* Do nothing */ }

	void onKey(int key, int scancode, int action, int mods) {
		//TODO: Create switch and call appropiate camera methods
	}

	void onMouseEnter(double xpos, double ypos) {
		mousex = xpos;
		mousey = ypos;
		focused = true;
	}

	void onMouseExit(double xpos, double ypos) {
		focused = false;
	}
	void onMouse(double xpos, double ypos) {
		camera.pan((xpos - mousex) * DeltaTime::dt * 10.0f);
		camera.tilt((ypos - mousey) * DeltaTime::dt * 6.0f);
	}

	//Your own methods
	void run() {
		while(running) {
			rds[2].modelMatrix[3] = glm::vec4(5.0f*cosf(10.0f*getGametime()), 5.0f, 5.0f*sinf(10.0f*getGametime()), 1.0f); //Example game logic..
			signalRenderer(); //Current thread blocked here until renderer is done rendering
		}
	}
};

int main(int argc, char** argv) {
	Game game; //This is your game class which realizes the RenderQuerier interface
	Renderer renderer; //This is the lowpoly3d renderer
	renderer.initialize(&game); //Can pass game as argument since game inherits from ILowpolyInput
	SphereGenerator sphereGenerator({125.0f, 125.0f, 125.0f}, 3);
	TerrainGenerator terrainGenerator;
	Model sphereModel = sphereGenerator.generate();
	Model terrainModel = terrainGenerator.generate();
	renderer.loadModel("terrain", terrainModel);
	renderer.loadModel("sphere", sphereModel);
	std::thread thread(&Game::run, &game); //Run game in a thread
	renderer.render(game); //Main-thread will remain in here until renderer terminates
	game.running = false; //Renderer has quit, so terminate simulation and join simulation thread with main thread
	thread.join();
	renderer.terminate();
}