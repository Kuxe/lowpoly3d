#ifndef DUMMYSIMULATION_HPP
#define DUMMYSIMULATION_HPP

#include "camera.hpp"
#include "renderquerier.hpp"
#include "renderdata.hpp"
#include "celestialbody.hpp"

class Gamedata;
class DummySimulation : public RenderQuerier {
private:
	Camera camera;
	std::vector<RenderData> renderDatas;
	RenderData sunRd;
	CelestialBody suncb;
	bool forceDaytime;
	float gametimeSpeedFactor;
public:
	DummySimulation(const Gamedata& gd);
	const std::vector<RenderData>& getRenderDatas() const;
	const RenderData& getSunRenderData() const;
	const Camera& getCamera() const;
	const float getGametime() const;
	const float getSunOmega() const;
	void update();
};

#endif //DUMMYSIMULATION_HPP