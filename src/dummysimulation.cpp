#include "dummysimulation.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include "gamedata.hpp"

DummySimulation::DummySimulation(const Gamedata& gd) : 
	renderDatas(gd.renderDatas), sunRd(gd.sunRd), suncb(gd.suncb), forceDaytime(gd.forceDaytime), gametimeSpeedFactor(gd.gametimeSpeedFactor), running(true)
 {

}

const std::vector<RenderData>& DummySimulation::getRenderDatas() const {
	return renderDatas;
}

const RenderData& DummySimulation::getSunRenderData() const {
	return sunRd;
}

const Camera& DummySimulation::getCamera() const {
	return camera;
}

const float DummySimulation::getGametime() const {
	using namespace std::chrono;
	return gametimeSpeedFactor*duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count()/36000.0f;
}

const float DummySimulation::getSunOmega() const {
	return !forceDaytime * getGametime();
}

void DummySimulation::run() {
	while(running) {
		sunRd.modelMatrix[3] = glm::vec4(suncb.getPos(getSunOmega()), 1.0);
		signalRenderer();
	}
};

void DummySimulation::terminate() {
	running = false;
}

