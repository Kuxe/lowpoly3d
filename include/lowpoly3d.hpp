#ifndef LOWPOLY3D_HPP
#define LOWPOLY3D_HPP

/** Lowpoly3d is an interface which leverages functionality from the lowpoly3d library

	//TODO: CONTINUE WRITING HERE.
	//TODO: WORK WITH CMAKE TO CREATE LIBRARY + HEADER INTERFACING TOWARDS LIBRARY which can be "installed"
	//		ie create directory with only visible to 3rd party headers and shared library
**/
struct Lowpoly3d {

	//Initializes renderer (create window and get ready for rendering)
	//returns an error code if fail
	bool initialize();

	//Terminates renderer (destroy window, no longer ready for rendering)
	void terminate();

	//Render renderDatas provided by RenderQuerier. Thread should remain in this method until 
	bool render(RenderQuerier& rq) const;

	//Load a 3D-model to GPU memory, returns a handle to the model
	int loadModel(const Model& model);
	std::vector<int> loadModels(const std::vector<Model>& models);

	//Keep tracks of what keys are held down
	static std::unordered_set<int> keysHeld;

	void setPrintFrameTime(bool printFrameTime);
	void setMultisamples(int msaa);
};


#endif //LOWPOLY3D_HPP