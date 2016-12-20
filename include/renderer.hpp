#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <glm/glm.hpp>
#include "subber.hpp"
#include "renderquerier.hpp"

class Scene;
class Model;
class RenderData;
class GLFWwindow;
class Camera;
struct rPress;
class DrawGridToggle;
class ShaderProgram;

/** Takes renderdatas and renderes them in 3D window **/
class Renderer : Subber<DrawGridToggle> {
private:
	bool drawGrid = false;
	GLFWwindow* window;

	//Keep track of triangles count per VA
	std::unordered_map<int, int> triangles;

	void handleHeldKeys() const;
public:

	//Initializes renderer (create window and get ready for rendering)
	//returns an error code if fail
	bool initialize();

	//Terminates renderer (destroy window, no longer ready for rendering)
	void terminate();

	//Render the renderDatas. Returns true on succesful termination, otherwise false
	bool render(RenderQuerier& rq) const;

	//Load a 3D-model to GPU memory, returns a handle to the model
	int loadModel(const Model& model);
	std::vector<int> loadModels(const std::vector<Model>& models);

	//Keep tracks of what keys are held down
	static std::unordered_set<int> keysHeld;

	void notify(const DrawGridToggle& event);
};

#endif //RENDERER_HPP