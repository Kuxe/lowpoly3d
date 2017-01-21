![alt tag](lowpoly3d.png)
A 3D engine tailored towards procedurally generated low poly graphics. It is a hobby project :).

## Self-contained example
```c++
#include "lowpoly3d.hpp"

/** Example file illustrating how to use lowpoly3d.
	Error handling and encapsulation have been omitted for brevity.
	If it's pitch dark, wait until the morning appears! **/
using namespace glm;
struct Game : public RenderQuerier, public ILowpolyInput {
	bool running = true;
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
		using namespace std::chrono;
		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count()/36000.0f;
	}

	const float getSunRadians() const {
		return getGametime();

	}

	void onKey(int key, int scancode, int action, int mods) {
		if(action == GLFW_PRESS) heldKeys.insert(key);
		else if(action == GLFW_RELEASE) heldKeys.erase(key);
	}

	void onMouse(double xpos, double ypos) {
		camera.look({xpos, ypos});
	}

	void run() {
		while(running) {
			//Game logic here - handle input and make a sphere go round and round 
			for(const int key : heldKeys) {
				switch(key) {
					case GLFW_KEY_W: camera.dolly(-3.0f); break;
					case GLFW_KEY_A: camera.truck(-3.0f); break;
					case GLFW_KEY_S: camera.dolly(+3.0f); break;
					case GLFW_KEY_D: camera.truck(+3.0f); break;
					case GLFW_KEY_Q: camera.pedestal(-3.0f); break;
					case GLFW_KEY_E: camera.pedestal(+3.0f); break;
					case GLFW_KEY_ESCAPE: running = false; break;
				}
			}
			rds[2].modelMatrix[3] = vec4(5.0f*cosf(10.0f*getGametime()), 5.0f, 5.0f*sinf(10.0f*getGametime()), 1.0f);
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
	renderer.loadModel("sphere", sphereGenerator.generate());
	renderer.loadModel("terrain", terrainGenerator.generate());
	std::thread thread(&Game::run, &game); //Run game in a thread
	renderer.render(game); //Main-thread will remain in here until renderer terminates
	game.running = false; //Renderer has quit, so terminate simulation and join simulation thread with main thread
	thread.join();
	renderer.terminate();
}
```

## Building (Linux)
Run `cmake .. && make` from within the build directory.

## Building (Windows)
Run `cmake -G "MinGW Makefiles" ..; make` from within the example directory.

## Installing (Windows and Linux)
After building, run `make install` from within the build directory.

# How do I use lowpoly3d?
## Two important interfaces
There are two interfaces which you must implement to use lowpoly3d. The RenderQuerier interface specifies what you must send to the renderer in order for it to render your game. The LowpolyInput interface specifies what the renderer might send back to you (keypresses for example).

### The RenderQuerier interface
RenderQuerier is the link between your game and lowpoly3d. RenderQuerier is a C++ interface which makes the renderer in lowpoly3d capable of rendering your scene. You must realize the RenderQuerier interface. The RenderQuerier interface should provide information about your scenery (=where are what objects) and other miscellaneous data such as position of the sun.

### The LowpolyInput interface
The window created by lowpoly3d captures input from keyboard and mouse if the window is active. Therefore there must be some channel where data can move from the renderer to your game. The LowpolyInput interface serves as this channel. LowpolyInput is an interface which demands of you to implement some methods such as "onMouse" and "onKeyPress". These methods are called by lowpoly3d renderer whenver it receives whatever event you provoked.

## Two important structs
In addition there are two important structs which you should embrace to get going with lowpoly3d.
### RenderData
A RenderData is a wrapper around data which should answer the "where are what objects" question (posed under The RenderQuerier interface!). If your game has a hero, the lowpoly renderer only cares about the whereabouts of your hero (the `modelMatrix` member of RenderData) and what he looks like (the `model` member of RenderData).. and what shader he should be rendered with (the `shader` member of RenderData).

### Model
Model represents a 3d model. It consists of a vector of N vertices, another vector of N vertex colors and a vector of M triangles, where a triangle in this context is three indices used for indexed drawing. A model is generated by a ModelGenerator.

## Model generators
A `ModelGenerator` is an interface that when realized spits out a `Model`. It is a very simple interface (`struct ModelGenerator { virtual Model generate() = 0; };`). Examples are `SphereGenerator` or `TerrainGenerator`.
