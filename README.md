[![Build Status](https://travis-ci.org/Kuxe/lowpoly3d.svg?branch=master)](https://travis-ci.org/Kuxe/lowpoly3d)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/kuxe/lowpoly3d?svg=true)](https://ci.appveyor.com/project/Kuxe/lowpoly3d)

![alt tag](lowpoly3d.png)
A 3D engine tailored towards procedurally generated low poly graphics, using OpenGL. Lowpoly3d builds under Windows, OSX and Linux.

Lowpoly3d is a hobby project :).

## Self-contained example
```c++
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
```

## Building (Linux and OSX)
Run `cmake .. && make` from within the build directory.

## Building (Windows)
Run `cmake -G "MinGW Makefiles" ..; make` from within the build directory.

## Installing (Windows and Linux)
After building, run `make install` from within the build directory.

## Installing (OSX)
Have not tried installing on OSX.

# How do I use lowpoly3d?

## The LowpolyInput interface
The LowpolyInput interface specifies what the renderer might send back to you (keypresses for example). The window created by lowpoly3d captures input from keyboard and mouse if the window is active. Therefore there must be some channel where data can move from the renderer to your game. The LowpolyInput interface serves as this channel. LowpolyInput is an interface which demands of you to implement some methods such as "onMouse" and "onKeyPress". These methods are called by lowpoly3d renderer whenver it receives whatever event you provoked.

## Three important structs
In addition there are three important structs which you should embrace to get going with lowpoly3d.

### Scene
A lowpoly3d scene is a struct which contains all data which is required by the class `Renderer` when rendering a frame, such as a view-matrix or where the sun is located. In order to trigger lowpoly3d into rendering anything you must call `setScene(const Scene& scene)`.

### RenderData
If your game has a hero, the `Renderer` cares only about the whereabouts of your hero (the `modelMatrix` member of RenderData) and what he looks like (the `model` member of RenderData).. and what shader he should be rendered with (the `shader` member of RenderData). These are the three members of RenderData. The `Scene`-class has a member `std::vector<RenderData> renderDatas` which you should populate before rendering the scene. So anything you want to show in your scene should be represented by an instance of `Renderdata` within the `renderDatas` vector.

### Model
Model represents a 3d model. It consists of a vector of N vertices, another vector of N vertex colors and a vector of M triangles, where a triangle in this context is three indices used for indexed drawing. A model is generated by a ModelGenerator.

## Model generators
A `ModelGenerator` is an interface that when realized spits out a `Model`. It is a very simple interface (`struct ModelGenerator { virtual Model generate() = 0; };`). Examples are `SphereGenerator` or `TerrainGenerator`.
