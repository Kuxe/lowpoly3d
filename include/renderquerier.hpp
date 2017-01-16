#ifndef RENDERQUERIER_HPP
#define RENDERQUERIER_HPP

#include <vector>
#include <mutex>
#include <condition_variable>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/** Renderer will through this class query the simulation for
	whatever data the renderer needs in order to render.
	The simulation (or game) should implement RenderQuerier.
	Whenever the renderer should render, pass the simulation to
	render(const RenderQuerier& rq) **/

class RenderData;
class Camera;
class Renderer;
class RenderQuerier {
private:
	std::condition_variable cv;
	std::mutex s;
	bool shouldRender = false, rendererActive = false;
	void signalSimulation() {
		shouldRender = false; 
		cv.notify_one();
	}
	friend Renderer;
public:
	virtual const std::vector<RenderData>& getRenderDatas() const = 0;
	virtual const glm::mat4 getView() const = 0;
	virtual const float getGametime() const = 0;
	virtual const float getSunRadians() const = 0;

	/**	signalRenderer tells the lowpoly3d renderer to a new frame.
		signalRenderer() blocks the current thread until
		the renderer is done rendering, since the renderer
		is using shared data provided by the RenderQuerier interface **/
	void signalRenderer() {
		shouldRender = true;
		std::unique_lock<std::mutex> lk(s);
		/** If shouldRender already set to false (renderer already did frame)
			or if renderer is terminated, dont even bother waiting.
			Otherwise wait (renderer will give signal if it is done rendering
			or about to terminate, so we should be good!) **/
		cv.wait(lk, [this] { return !shouldRender || !rendererActive; });
	}
};

#endif //RENDERQUERIER_HPP