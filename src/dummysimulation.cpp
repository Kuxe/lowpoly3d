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


void DummySimulation::onError() {

}
void DummySimulation::onFramebufferResize() {

}
void DummySimulation::onKeyPress() {

}
void DummySimulation::onKeyRelease() {

}
void DummySimulation::onMouseEnter() {

}
void DummySimulation::onMouseExit() {

}
void DummySimulation::onMouse() {

}

void DummySimulation::run() {
	while(running) {
		signalRenderer();
	}
};

void DummySimulation::terminate() {
	running = false;
}

