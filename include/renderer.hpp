#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <unordered_map>
#include <string>
#include "scene.hpp"
#include "queue.hpp" //For buffering scenes during setScene()
#include "debugrenderer.hpp"
#include <memory> //std::unique_ptr
#include <filesystem> //std::filesystem::path

#include "geometric_primitives/line.hpp"

struct GLFWwindow;

namespace lowpoly3d {

/** Forward declarations **/
struct Model;
struct ILowpolyInput;
struct GLFrame;

template<typename floating_point_type, std::size_t dimension>
struct TLine;
using Line = TLine<float, 3>;

/** Renderer can render a scene, set via the setScene()-method. Intended use is to first
	load a set of models via the loadModels-method. The loadModels-method return a set of integer
	handles. These integer handles correspond to the vertexArray-member of the RenderData struct.**/
class Renderer {
private:
	bool initialized = false, printFrameTime = false;
	GLFWwindow* window;
	std::filesystem::path shaderDirectory;

	//Keep track of triangles count per model and what vertex array is associated with what name
	std::unordered_map<std::string, int> triangles, models;

	/** QUEUE_SIZE determines how many scenes may be buffered, that is, how many scenes
		the producer may produce before scenes will be discarded if queue is full.
		In practice a large buffer result in more memory usage and possible input-delay
		(since the rendered scene is a function of input received several milliseconds earlier)
		but a large buffer may also provide a smoother experience since if the producer
		sporadically lag behind and stop producing, the renderer will still have scenes to
		render so thus the visible lag observed by a sporadically slow producer will be mitigated.
		An opportunity with using a large scene buffer is that scenes whom are not at the front of
		the queue may be subject to culling algorithms in parallell, so a large buffer will
		allow for more parallell culling (still at expense of input lag, so it's really a
		trade-off between input lag and render lag) **/
	static constexpr size_t QUEUE_SIZE = 2;
	Queue<Scene> scenes;

	//Debug renderer draws various debug primitives useful for debugging
	DebugRenderer debugRenderer;

	std::unique_ptr<GLFrame> worldAxes;

	bool showWireframes = false;

public:

	Renderer();
	~Renderer();

	/** Initializes renderer (create window and get ready for rendering)
		First argument "li" is the class to which the renderer will forward
		callbacks such as key presses or window resize.
		shaderDirectory is a relative or absolute path to the directory containing
		shaders used by lowpoly3d. These shaders are provided by lowpoly3d.
		returns an error code if fail **/
	bool initialize(ILowpolyInput* li, const std::filesystem::path& shaderDirectory);

	//Terminates renderer (destroy window, no longer ready for rendering)
	void terminate();

	//Run the renderer. Remember to set the scene, otherwise nothing is rendered.
	bool run();

	//Load a 3D-model to GPU memory, returns a handle to the model
	bool loadModel(const std::string& name, const Model& model);
	bool loadModels() const { return true; }
	template<typename T, typename S, typename... Pack>
	bool loadModels(const T& t, const S& s, const Pack&... pack) {
		return loadModel(t, s) && loadModels(pack...);
	}

	/** Offer a scene which may be rendered. A call to offer places the scene
		in a queue if the scene fits, otherwise the scene is discarded.
		The offer method can therefore (sloppily) be thought of as
		the "render" method which renders a scene **/
	void offer(const Scene& scene);

	// Enable to show wireframes for all triangle models
	void wireframes(bool enable);
	// Returns true if wireframes are shown, otherwise false
	bool wireframes() const;

	void setPrintFrameTime(bool printFrameTime);
	void setMultisamples(int msaa);
};

}

#endif //RENDERER_HPP