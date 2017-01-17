#include "dummysimulation.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include "gamedata.hpp"

DummySimulation::DummySimulation(const Gamedata& gd) : 
	renderDatas(gd.renderDatas), forceDaytime(gd.forceDaytime), gametimeSpeedFactor(gd.gametimeSpeedFactor), running(true)
 {

}

const std::vector<RenderData>& DummySimulation::getRenderDatas() const {
	return renderDatas;
}

const glm::mat4 DummySimulation::getView() const {
	return camera.get();
}

const float DummySimulation::getGametime() const {
	using namespace std::chrono;
	return gametimeSpeedFactor*duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count()/36000.0f;
}

const float DummySimulation::getSunRadians() const {
	return !forceDaytime * getGametime();
}

void DummySimulation::onError() { running = false; }
void DummySimulation::onFramebufferResize(int width, int height) { }

void DummySimulation::onKey(int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) heldKeys.insert(key);
	else if(action == GLFW_RELEASE) heldKeys.erase(key);
}

void DummySimulation::onMouseEnter(double xpos, double ypos) { }
void DummySimulation::onMouseExit(double xpos, double ypos) { }
void DummySimulation::onMouse(double xpos, double ypos) { camera.look({xpos, ypos}); }

void DummySimulation::run() {
	while(running) {
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
		signalRenderer();
	}
};

void DummySimulation::terminate() {
	running = false;
}

