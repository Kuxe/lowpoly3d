#include "drawfeature.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <cmath>
#include "glfw_callbacks.hpp"
#include "renderer.hpp"
#include "model.hpp"
#include "renderdata.hpp"
#include "events.hpp"
#include "shaderprogram.hpp"
#include "framebuffer.hpp"
#include "depthframebuffer.hpp"
#include "uniformbuffer.hpp"
#include "worlduniformdata.hpp"
#include "modeluniformdata.hpp"
#include "mvpuniformdata.hpp"
#include "ilowpolyinput.hpp"
#include "celestialbody.hpp"
#include "scene.hpp"
#include "geometric_primitives/line.hpp"
#include "glframe.hpp"
#include "shaderprogrambank.hpp"

#include <filesystem>
#include <sstream>
#include <unordered_map>

using namespace gl;

namespace lowpoly3d {

/** FIXME LIST
	//Empty
**/

/** TODO LIST

	TODO:   Field of depth would be nice
	TODO:   Instanced indexed drawing could increase performance
	TODO:   Text


**/

Renderer::Renderer()
	: scenes(QUEUE_SIZE) {}

// Destructor must be defined when using std::unique_ptr with forward declarations
// I don't know why though.
Renderer::~Renderer() = default;

bool Renderer::initialize(ILowpolyInput* li, const std::filesystem::path& shaderDirectory) {
	assert(std::filesystem::exists(shaderDirectory));

	GLFWCallbacks::set_input_handler(li);
	this->shaderDirectory = shaderDirectory;
	glfwSetErrorCallback(GLFWCallbacks::error_callback);
	if(glfwInit() == GLFW_FALSE) {
		fprintf(stderr, "Could not load glfw (call to glfwInit returned GLFW_FALSE)\n");
		return false;
	}

	//Ensure that 3.2 context is used
	const GLubyte requiredMajorVersion('3'), requiredMinorVersion('2');
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, requiredMajorVersion-'0');
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, requiredMinorVersion-'0');

	window = glfwCreateWindow(480, 270, "Lowpoly3D", NULL, NULL);
	if(!window) {
		printf("ERROR: Could open window (call to glfwCreateWindow returned NULL)\n");
		glfwTerminate();
		return false;
	}

	glfwSetKeyCallback(window, GLFWCallbacks::key_callback);
	glfwSetCursorPosCallback(window, GLFWCallbacks::mouse_callback);
	glfwSetCursorEnterCallback(window, GLFWCallbacks::cursor_enter_callback);
	glfwSetFramebufferSizeCallback(window, GLFWCallbacks::framebuffer_size_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glbinding::Binding::initialize([](const char * name) {
		return glfwGetProcAddress(name);
	}, false);

	/** Check that required OpenGL version 3.0 is met.
		glGetIntegerv(GL_MAJOR_VERSION) not supported on OpenGL <=3.0 hence this approach **/
	const GLubyte* glVersion(glGetString(GL_VERSION));
	const GLubyte majorVersion(glVersion[0]), minorVersion(glVersion[2]);
	if(majorVersion < requiredMajorVersion || (majorVersion == requiredMajorVersion && minorVersion < requiredMinorVersion)) {
		printf("ERROR: Your have OpenGL %c.%c, but OpenGL >=%c.%c is required by Lowpoly3d.\n",
			majorVersion, minorVersion, requiredMajorVersion, requiredMinorVersion);
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(23.0f/255.0f, 126.0f/255.0f, 137.0f/255.0f, 255.0f/255.0f);

	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error encountered within renderer::initialize()\n");
		return false;
	}

	if(!debugRenderer.init()) {
		printf("ERROR: Renderer could not initialize debug renderer\n");
		return false;
	}

	worldAxes = std::make_unique<GLFrame>();
	originFrame = std::make_unique<GLFrame>();

	initialized = true;
	return true;
}

void Renderer::terminate() {
	glfwDestroyWindow(window);
	glfwTerminate();
	initialized = false;
}

bool Renderer::loadModel(const std::string& name, const Model& model) {
	if(!initialized) {
		printf("ERROR: Could not load model, renderer is not initialized (did you forget to call the initialize()-method?\n");
		return false;
	}
	//Send model to GPU memory and return handle
	//to the model stored in GPU memory such that
	//we later on easily can tell the GPU
	//"Mr GPU, please render the model on handle which I've stored in your memory!"
	//which is done with a glBindVertex(handle)-call

	GLuint vertexBuffer, colorBuffer, indexBuffer, vertexArray;

	glGenVertexArrays(1, &vertexArray);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not generate vertex array\n");
		return false;
	}

	glBindVertexArray(vertexArray);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not bind vertex array\n");
		return false;
	}

	glGenBuffers(1, &vertexBuffer);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not generate vertex buffer\n");
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not bind vertex buffer\n");
		return false;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * model.vertices.size(), &model.vertices[0], GL_STATIC_DRAW);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not send vertex buffer to GPU\n");
		return false;
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set vertex attribute pointer for vertex buffer\n");
		return false;
	}

	glEnableVertexAttribArray(0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not enable attribute pointer for vertex buffer\n");
		return false;
	}

	glGenBuffers(1, &colorBuffer);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not generate color buffer\n");
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not bind color buffer\n");
		return false;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * model.colors.size(), &model.colors[0], GL_STATIC_DRAW);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could send color buffer to GPU\n");
		return false;
	}

	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set vertex attribute pointer for color buffer\n");
		return false;
	}

	glEnableVertexAttribArray(1);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not enable vertex attribute for color buffer\n");
		return false;
	}

	glGenBuffers(1, &indexBuffer);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not generate index buffer\n");
		return false;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not bind index buffer\n");
		return false;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(model.triangleIndices)::value_type) * model.getNumTriangles(), &model.triangleIndices[0], GL_STATIC_DRAW);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not send index buffer to GPU\n");
		return false;
	}
	
	glVertexAttribPointer(3, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set set vertex attribute for index buffer\n");
		return false;
	}

	glEnableVertexAttribArray(3);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not enable vertex attribute for index buffer\n");
		return false;
	}

	triangles[name] = model.triangleIndices.size();
	models[name] = vertexArray;

	glBindVertexArray(0); //Dont let subsequent calls work on vertexArray
	return true;
}

bool Renderer::run() {
	if(!initialized) {
		printf("ERROR: Renderer is not initialized (did you forget to call the initialize()-method?\n");
		return false;
	}

	using namespace std::chrono;
	using RenderDatas = std::vector<RenderData>;

	/** Create quad vertices for use in post-processing **/
	gl::GLuint quadVA, quadVBO;
	glGenVertexArrays(1, &quadVA);
	glBindVertexArray(quadVA);
	static const GLfloat quadVertices[] {
		-1.0f, -1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f,
		-1.0f, +1.0f, 0.0f,
		-1.0f, +1.0f, 0.0f,
		+1.0f, -1.0f, 0.0f,
		+1.0f, +1.0f, 0.0f};
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not load quad for postprocessing\n");
		return false;
	}

	ShaderProgramBank shaderProgramBank(shaderDirectory);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error during shaderprogrambank creation.\n");
		return false;
	}

	if(!shaderProgramBank.link()) {
		return false;
	}
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Unknown error during shaderprogrambank linking.\n");
		return false;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	/** mainFBO is the fbo where the actual full-scene is rendered.
		The FBO pipeline is as follows:
						  draw mainFBO w MSAA          Render mainFBO to postProcessFBO w postfx shaders (no MSAA)           Blit postfxFBO to defaultFBO
			depthFBO --------------------> mainFBO ----------------------------------------------------------> postfxFBO ----------------------------> defaultFBO
	**/
	constexpr GLsizei numMultisamples = 0; //FIXME: Causes crash on Linux (and possibly OSX too on >0)
	constexpr uint16_t SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	Framebuffer mainFBO("Main FBO", width, height, numMultisamples);
	Framebuffer postfxFBO("PostFX FBO", width, height);
	Framebuffer debugFBO("Debug FBO", width, height);
	DepthFramebuffer depthFBO(SHADOW_WIDTH, SHADOW_HEIGHT);

	/** Create two UBOs. World UBO is an Uniform Buffer Object which contains data that is constant
		over any RenderData (such as position of sun, viewprojection matrix, time). All shaders have access
		to World UBO. World UBO is updated once per frame. Model UBO is an Uniform Buffer Object which
		is updated once per RenderData. It contains data provided by RenderData such as the model matrix **/
	UniformBuffer worldUBO("World UBO", 1);
	UniformBuffer modelUBO("Model UBO", 2);
	UniformBuffer mvpUBO("MVP UBO", 3);
	if(!shaderProgramBank["default"_sph].setUBO("WorldUniformData", worldUBO)) return false;
	if(!shaderProgramBank["default"_sph].setUBO("ModelUniformData", modelUBO)) return false;
	if(!shaderProgramBank["skybox"_sph].setUBO("WorldUniformData", worldUBO)) return false;
	if(!shaderProgramBank["skybox"_sph].setUBO("ModelUniformData", modelUBO)) return false;
	if(!shaderProgramBank["depth"_sph].setUBO("ModelUniformData", modelUBO)) return false;
	if(!shaderProgramBank["simple"_sph].setUBO("MVPUniformData", mvpUBO)) return false;
	if(!shaderProgramBank["color"_sph].setUBO("WorldUniformData", worldUBO)) return false;
	if(!shaderProgramBank["color"_sph].setUBO("ModelUniformData", modelUBO)) return false;

	//Sun should rotate around the x-axis through origo
	CelestialBody suncb({0.0, 0.0, 0.0}, {95.0, 0.0, 0.0}, 1.57079632679);

	auto startTime = std::chrono::high_resolution_clock::now();

	while(!glfwWindowShouldClose(window)) {
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		const glm::vec2 windowResolution(width, height);
		const glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f/1080.0f, 0.1f, 1000.0f);

		const auto currentTime = std::chrono::high_resolution_clock::now();
		const uint32_t time32 = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count(); //Used for creating effects as function of time in shaders

		//Linear interpolation
		const auto lerp = [](const auto& a, const auto& b, float t) { return a*(1.0f-t) + b*t; };

		//Function that clamps values in range [lower, upper]
		const auto clamp = [](float n, float lower, float upper) { return std::max(lower, std::min(n, upper)); };

		//Clamps cosine in range [0.0f, 1.0f], steepness determines how steep the slope is
		const auto clippedcos = [&](float t, float steepness) { return clamp(steepness*cosf(t), 0.0f, 1.0f); };

		//Compute clear color as function of time to match the sunset and sunrise
		const auto timeOfDayColorLambda = [&](const float t) {
			const glm::vec3 noonColor(80, 219, 255);
			const glm::vec3 midnightColor(5,0,27);
			return lerp(midnightColor, noonColor, t) / 255.0f;
		};

		/** Draws a set of renderdatas.
			This lambda does NOT bind anything but the vertex array of each render data,
			so you need to specify what fbo or what shaders to use beforehand **/
		const auto drawRenderData = [&](const RenderData& rd) {
			try {
				
				auto drawcall = prepareDrawCall([&](){
					glBindVertexArray(models.at(rd.model)); 
					glDrawElements(GL_TRIANGLES, triangles.at(rd.model)*3, GL_UNSIGNED_SHORT, nullptr);
				});

				rd.drawFeatureTarget.draw(*drawcall, *this);

			} catch(const std::out_of_range& oor) {
				std::stringstream ss;
				ss << "ERROR: Could not lookup vertex array given by render data \"" << rd.model << "\"\n";

				std::vector<std::string> names;
				names.reserve(models.size());

				ss << "Available models are: {";

				if(models.empty()) ss << "}\n";
				else
				{
					auto lastIt = prev(end(models));
					for(auto it = begin(models); it != lastIt; ++it)
					{
						ss << it->first << ", ";
					}
					ss << lastIt->first << "}\n";
				}
				printf("%s!", ss.str().c_str());

				return false;
			} catch(const std::exception& e) {
				printf("Error: Could not draw render data %s", e.what());
			}

			if(glGetError() != GL_NO_ERROR) {
				printf("ERROR: Failed to draw RenderData with model \"%s\"\n", rd.model.c_str());
				return false;
			}

			return true;
		};

		/** Draws a set of renderdatas, the sun from POV of view-matrix to an framebuffer object
			This is the place where the whole scene, with per-model shaders, is drawn **/
		const auto render2fbo = [&](
			const RenderDatas& rds,
			const float sunRadians,
			const glm::mat4& sunvp,
			const glm::mat4& view,
			const glm::mat4& projection,
			const Framebuffer& fbo) {
			fbo.use();
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/** Compute clear color as function of time to match the sunset and sunrise **/
			const float t = clippedcos(sunRadians, 5.0f);
			const glm::vec3 timeOfDayColor = timeOfDayColorLambda(t);

			worldUBO.use<WorldUniformData>(view, projection, glm::vec4(suncb.getPos(sunRadians), 1.0), glm::vec4(timeOfDayColor, 0.0), windowResolution);

			if(!shaderProgramBank["default"_sph].use()) {
				printf("ERROR: Could not use shader program\n");
				return false;
			}
			if(!shaderProgramBank["default"_sph].setTexture("shadowmap", depthFBO.getTexture())) {
				printf("ERROR: Could not set shadowmap\n");
				return false;
			}

			wireframes(showWireframes);

			/** vp is the view-projection matrix. Model matrix is provided per renderdata,
				so later on vp will be multiplied with model matrix once per renderdata **/
			const glm::mat4 vp = projection * view;
			for(const auto& rd : rds) {
				try {
					shaderProgramBank[rd.shader].use();
				} catch (const std::out_of_range& e) {
					printf("ERROR: Could not draw RenderData, there is no shader \"%s\" (there are 0...%zu shaders)\n", rd.shader.c_str(), shaderProgramBank.size());
					return false;
				} catch (const std::exception& e) {
					printf("ERROR: Could not draw RenderData %s\n", e.what());
				}
				modelUBO.use<ModelUniformData>(rd.modelMatrix, vp * rd.modelMatrix, sunvp * rd.modelMatrix);
				if(!drawRenderData(rd)) {
					printf("ERROR: Could not draw a renderdata\n");
					return false;
				};
			}

			wireframes(false);

			// Lets trick the MVP-ubo into rendering into
			// screen-space by not passing a MVP matrix
			worldAxes->draw([&view]() {
				const float worldAxesSize = 0.1f;
				glm::mat4 frame = glm::scale(view, glm::vec3(worldAxesSize));
				frame[3] = glm::vec4(-1.0f + worldAxesSize, -1.0f + worldAxesSize, 0.0f, 1.0f);
				return frame;
			}(), shaderProgramBank["simple"_sph], mvpUBO, 1.0f);

			originFrame->draw(vp * glm::identity<glm::mat4>(), shaderProgramBank["simple"_sph], mvpUBO, 3.0f);

			return true;
		}; //End of render2fbo

		/** Blit content of mainFBO to postfxFBO and finally draw postfxFBO to default FBO **/
		auto render2screen = [&] {
			/** Blit mainFBO onto postfxFBO **/
			if(!mainFBO.use(GL_READ_FRAMEBUFFER)) {
				printf("ERROR: Could not use mainFBO with GL_READ_FRAMEBUFFER target!\n");
				return false;
			}
			postfxFBO.use(GL_DRAW_FRAMEBUFFER);
			glViewport(0, 0, width, height);
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			if(glGetError() != GL_NO_ERROR) {
				printf("ERROR: Could not blit mainFBO to postfxFBO\n");
				return false;
			}

			/** Render quad filling the screen using postfx shaders, where postfx shader got multisampled texture from mainFBO **/
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			shaderProgramBank["post-process"_sph].use();
			if(!shaderProgramBank["post-process"_sph].setUniform("resolution", windowResolution)) return false;
			if(!shaderProgramBank["post-process"_sph].setUniform("time", time32)) return false; 
			if(!shaderProgramBank["post-process"_sph].setTexture("renderedTexture", postfxFBO.getTexture())) return false;
			glBindVertexArray(quadVA);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			if(glGetError() != GL_NO_ERROR) {
				printf("ERROR: Could not draw postfxFBO to defaultFBO\n");
				return false;
			}
			return true;
		};

		/** Renders renderDatas to depthFBO such that render2screen can provide the shadowmap uniform **/
		auto render2depth = [&](const RenderDatas& rds, const glm::mat4& viewproj) {
			//Render each renderdata (except sun?) to depthfbo using depth shaders
			depthFBO.use();
			shaderProgramBank["depth"_sph].use();
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glClear(GL_DEPTH_BUFFER_BIT);
			for(const auto& rd : rds) {
				modelUBO.use<ModelUniformData>(rd.modelMatrix, viewproj * rd.modelMatrix, viewproj * rd.modelMatrix);
				if(!drawRenderData(rd)) {
					printf("ERROR: Could not draw a renderdata to depthFBO\n");
					return false;
				};
			}
			return true;
		};

		if(!scenes.empty()) {
			/** In order to do shadow-mapping, I need to render to FBO. In order to render to FBO
				I need to provide a view matrix for the sun. For now the sun will look at 0.0
				This will only give shadows in the 0.0 region. Later on this can probably be improved
				by letting the sun look at the point which is the foci of the camera, or something along
				those lines (this corresponds to computing shadowmap only where player is looking). **/
			const Scene& scene = scenes.front(); //This assignment to Scene& scene assumes that scenes.front() is only modified by this thread
			auto const& constants = scene.getSceneConstants();
			const glm::mat4 sunView = glm::lookAt(suncb.getPos(constants.getSunRadians()), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			const glm::mat4 sunPerspective = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
			const glm::mat4 sunvp = sunPerspective * sunView;
			debugRenderer.render(constants.getView() * projection, debugFBO, shaderProgramBank["debug"_sph]);

			showWireframes = constants.getShowWireframes();

			auto const& renderDatas = scene.getRenderDatas();

			if(!(
				render2depth(renderDatas, sunvp) &&
				render2fbo(renderDatas, constants.getSunRadians(), sunvp, constants.getView(), projection, mainFBO) &&
				render2screen())) {
					scenes.pop();
					return false;
			}
			scenes.pop();
		}

		glfwPollEvents();
		glfwSwapBuffers(window);

		if(glGetError() != GL_NO_ERROR) {
			printf("ERROR: Unknown error in renderer!\n");
			return false;
		}
	}
	return true;
}

void Renderer::offer(const Scene& scene) {
	//If a scene fits into the scene-queue, then insert it and tell renderer that it should render a scene in the queue
	scenes.offer(scene);
}

void Renderer::wireframes(bool enable) {
	showWireframes = enable;
	glPolygonMode(GL_FRONT_AND_BACK, showWireframes ? GL_LINE : GL_FILL);
}

bool Renderer::wireframes() const {
	return showWireframes;
}

void Renderer::setPrintFrameTime(bool printFrameTime) {
	this->printFrameTime = printFrameTime;
}

}
