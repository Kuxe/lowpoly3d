#ifndef DUMMYSIMULATION_HPP
#define DUMMYSIMULATION_HPP

#include "camera.hpp"
#include "renderquerier.hpp"
#include "ilowpolyinput.hpp"
#include "renderdata.hpp"
#include "celestialbody.hpp"
#include <atomic>

class Gamedata;
class DummySimulation : public RenderQuerier, public ILowpolyInput {
private:
	Camera camera;
	std::vector<RenderData> renderDatas;
	bool forceDaytime;
	float gametimeSpeedFactor;
	std::atomic<bool> running;
public:
	DummySimulation(const Gamedata& gd);

	/** Required by RenderQuerier **/
	const std::vector<RenderData>& getRenderDatas() const;
	const glm::mat4 getView() const;
	const float getGametime() const;
	const float getSunRadians() const;

	/** Required by ILowpolyInput **/
	void onError();
	void onFramebufferResize();
	void onKeyPress();
	void onKeyRelease();
	void onMouseEnter();
	void onMouseExit();
	void onMouse();

	/** Regular methods **/
	void run();
	void terminate();
};

#endif //DUMMYSIMULATION_HPP