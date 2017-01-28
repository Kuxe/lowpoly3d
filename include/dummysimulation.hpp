#ifndef DUMMYSIMULATION_HPP
#define DUMMYSIMULATION_HPP

#include "lowpoly3d.hpp"
#include <atomic>

namespace lowpoly3d {

struct Gamedata;
class DummySimulation : public RenderQuerier, public ILowpolyInput {
private:
	Camera camera;
	std::vector<RenderData> renderDatas;
	bool forceDaytime;
	float gametimeSpeedFactor;
	float dt = 0.0f;
	std::atomic<bool> running;
	std::unordered_set<int> heldKeys;
public:
	DummySimulation(const Gamedata& gd);

	/** Required by RenderQuerier **/
	const std::vector<RenderData>& getRenderDatas() const;
	const glm::mat4 getView() const;
	const float getGametime() const;
	const float getSunRadians() const;

	/** Required by ILowpolyInput **/
	void onError();
	void onFramebufferResize(int width, int height);
	void onKey(int key, int scancode, int action, int mods);
	void onMouseEnter(double xpos, double ypos);
	void onMouseExit(double xpos, double ypos);
	void onMouse(double xpos, double ypos);

	/** Regular methods **/
	void run();
	void terminate();
};

}

#endif //DUMMYSIMULATION_HPP