#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "renderquerier.hpp"

class Model;
class GLFWwindow;
class ILowpolyInput;

/** Renderer can render RenderDatas provided by a RenderQuerier. Intended use is to first
	load a set of models via the loadModels-method. The loadModels-method return a set of integer
	handles. These integer handles correspond to the vertexArray-member of the RenderData struct.**/
class Renderer {
private:
	bool printFrameTime = false;
	GLFWwindow* window;

	//Keep track of triangles count per VA
	std::unordered_map<int, int> triangles;

	void handleHeldKeys() const;
public:

	//Initializes renderer (create window and get ready for rendering)
	//returns an error code if fail
	bool initialize(ILowpolyInput* const li);

	//Terminates renderer (destroy window, no longer ready for rendering)
	void terminate();

	//Render the renderDatas. Returns true on succesful termination, otherwise false
	bool render(RenderQuerier& rq) const;

	//Load a 3D-model to GPU memory, returns a handle to the model
	int loadModel(const Model& model);
	std::vector<int> loadModels(const std::vector<Model>& models);

	//Keep tracks of what keys are held down
	static std::unordered_set<int> keysHeld;

	void setPrintFrameTime(bool printFrameTime);
	void setMultisamples(int msaa);
};

#endif //RENDERER_HPP