#ifndef RENDERQUERIER_HPP
#define RENDERQUERIER_HPP

#include <vector>

/** Renderer will through this class query the simulation for
	whatever data the renderer needs in order to render.
	The simulation (or game) should implement RenderQuerier.
	Whenever the renderer should render, pass the simulation to
	render(const RenderQuerier& rq) **/

class RenderData;
class Camera;
struct RenderQuerier {
	virtual const std::vector<RenderData>& getRenderDatas() const = 0;
	virtual const RenderData& getSunRenderData() const = 0;
	virtual const Camera& getCamera() const = 0;
	virtual const float getGametime() const = 0;
	virtual const float getSunOmega() const = 0;
	virtual void update() = 0;
};


#endif //RENDERQUERIER_HPP