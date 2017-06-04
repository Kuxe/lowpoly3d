#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <cmath>
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
#include "ilowpolyinput.hpp"
#include "celestialbody.hpp"
#include "scene.hpp"

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

/*************/
/** Statics **/
/*************/

ILowpolyInput* lowpolyInput;

static void error_callback(int error, const char* description) {
    lowpolyInput->onError();
}

static void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
    lowpolyInput->onFramebufferResize(w, h);
    publish<OnResize>({w, h});
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
    lowpolyInput->onKey(key, scancode, action, mods);
}

static void cursor_enter_callback(GLFWwindow* window, int focused) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if(focused) {
        lowpolyInput->onMouseEnter(x, y);
    } else {
        lowpolyInput->onMouseExit(x, y);
    }
}

static void mouse_callback(GLFWwindow* window, double x, double y) {
    lowpolyInput->onMouse(x, y);
}

/*********************/
/** Private methods **/
/*********************/

//Empty over here AS IT SHOULD BE USE LAMBDAS

/*********************/
/** Public methods **/
/*********************/

bool Renderer::initialize(ILowpolyInput* li, const std::string& shaderDirectory) {
    lowpolyInput = li;
    this->shaderDirectory = shaderDirectory;
    glfwSetErrorCallback(error_callback);
    if(!glfwInit()) {
        fprintf(stderr, "Could not load glfw (call to glfwInit returned 0)\n");
        return false;
    }

    //Ensure that 3.0 context is used
    const GLubyte requiredMajorVersion('3'), requiredMinorVersion('1');
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, requiredMajorVersion-'0');
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, requiredMinorVersion-'0');
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(480, 270, "Lowpoly3D", NULL, NULL);
    if(!window) {
        printf("ERROR: Could open window (call to glfwCreateWindow returned NULL)\n");
        glfwTerminate();
        return false;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glbinding::Binding::initialize();

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

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triangle) * model.triangles.size(), &model.triangles[0], GL_STATIC_DRAW);
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

    triangles[name] = model.triangles.size();
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
    const auto error = glGetError();
    if(error != GL_NO_ERROR) {
        printf("ERROR: Could not load quad for postprocessing\n");
        return false;
    }

    /** Ideally a ShaderProgram defines what uniforms it needs.
        It would be nice during initialization of a shader to simply
        pass a list of datatypes which the shaders sets as uniforms.
        No. This is not nice. I want uniforms to be defined in shader
        NO not nice either, dont wanna subclass. **/
    ShaderProgram program("default");
    if(!program.link(
        GL_VERTEX_SHADER, shaderDirectory + "shader.vert",
        GL_FRAGMENT_SHADER, shaderDirectory + "shader.frag",
        GL_GEOMETRY_SHADER, shaderDirectory + "shader.geom")) {
        return false;
    }

    ShaderProgram sunProgram("sun");
    if(!sunProgram.link(
        GL_VERTEX_SHADER, shaderDirectory + "sun.vert",
        GL_FRAGMENT_SHADER, shaderDirectory + "sun.frag")) {
        return false;
     }

     ShaderProgram skyboxProgram("skybox");
     if(!skyboxProgram.link(
        GL_VERTEX_SHADER, shaderDirectory + "skybox.vert",
        GL_FRAGMENT_SHADER, shaderDirectory + "skybox.frag")) {
        return false;
    }

    ShaderProgram waterProgram("water");
    if(!waterProgram.link(
        GL_VERTEX_SHADER, shaderDirectory + "water.vert",
        GL_FRAGMENT_SHADER, shaderDirectory + "water.frag")) {
        return false;
    }

    ShaderProgram postprocessProgram("post-process");
    if(!postprocessProgram.link(
        GL_VERTEX_SHADER, shaderDirectory + "passthrough.vert",
        GL_FRAGMENT_SHADER, shaderDirectory + "postprocess.frag")) {
        return false;
    }

    ShaderProgram depthProgram("depth");
    if(!depthProgram.link(
        GL_VERTEX_SHADER, shaderDirectory + "depth.vert",
        GL_FRAGMENT_SHADER, shaderDirectory + "depth.frag")) {
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
    DepthFramebuffer depthFBO(SHADOW_WIDTH, SHADOW_HEIGHT);

    /** Create two UBOs. World UBO is an Uniform Buffer Object which contains data that is constant
        over any RenderData (such as position of sun, viewprojection matrix, time). All shaders have access
        to World UBO. World UBO is updated once per frame. Model UBO is an Uniform Buffer Object which
        is updated once per RenderData. It contains data provided by RenderData such as the model matrix **/
    UniformBuffer worldUBO("World UBO", 1);
    UniformBuffer modelUBO("Model UBO", 2);
    if(!program.setUBO("WorldUniformData", worldUBO)) return false;
    if(!program.setUBO("ModelUniformData", modelUBO)) return false;
    if(!skyboxProgram.setUBO("WorldUniformData", worldUBO)) return false;
    if(!skyboxProgram.setUBO("ModelUniformData", modelUBO)) return false;
    if(!depthProgram.setUBO("ModelUniformData", modelUBO)) return false;

    std::unordered_map<std::string, const ShaderProgram&> programs;
    auto addProgram = [&](const ShaderProgram& p) { programs.insert({p.name, p}); };
    addProgram(program);
    addProgram(sunProgram);
    addProgram(skyboxProgram);
    addProgram(waterProgram);
    addProgram(postprocessProgram);
    addProgram(depthProgram);

    //Sun should rotate around the x-axis through origo
    CelestialBody suncb({0.0, 0.0, 0.0}, {95.0, 0.0, 0.0}, 1.57079632679);

    while(!glfwWindowShouldClose(window)) {

        /** Explanation of threading in renderer (why and how):
            First of all, the simulation part of game (logic, physics, AI etc)
            should be decoupled from the rendering engine. This is because
            the code would get messy otherwise. In the best of worlds, only a
            minimal set of data flows from the "game" into the renderer.
            This minimal set of data is, for most part, called "RenderData".
            Other information that the renderer need from the game is position of sun,
            view-matrix and some other things. So an interface which the game implements
            should provide these data if the game should be compatible with the renderer.
            Thus, the only thing the game needs to do is simply realize the interface.
            The game need not know anything about the renderer. This is nice.

            So we know what the game must do (again, simply realize the RenderQuerier interface).
            What must the renderer do? The renderer must have access to the RenderQuerier,
            so pass a RenderQuerier into the render-method
                Renderer::render(const RenderQuerier& rq)
            This is precisely what is done right now. Now the renderer can access
            whatever it needs to render a nice world. Very good.
            But who passes RenderQuerier into the render-method?
            This can be done in main, like so:

                DummySimulation sim;
                Renderer renderer;
                renderer.initialize();
                renderer.render(sim); //Main-thread locked in here!
                renderer.terminate();

            Not very difficult. But notice that renderer.render(sim) will not return
            until the renderer is close (crossing out the window, alt+f4 or whatever).
            Question: When should the simulation update?
            Try1: Let the renderer update the simulation

                DummySimulation sim;
                Renderer renderer;
                renderer.initialize();
                renderer.render(sim); ... {
                    sim.update();
                    //do whatever the renderer does
                }
                renderer.terminate();

            Since the renderer knows the RenderQuerier, the RenderQuerier can provide
            a "update"-method which the renderer calls. Thus the renderer is in
            control of updating the simulation. This is not nice, because then
            the renderer is highly coupled with the simulation
            (the renderer is a puppet-master and the simulation is the puppet).

            Try2: Update the simulation in another thread, like so:

                DummySimulation sim;
                Renderer renderer;
                renderer.initialize();
                updateInThread(sim);
                renderer.render(sim);
                renderer.terminate();

            Very nice. But problem arises. Since the renderer may access sim at anytime, even when
            sim is doing computation, a race condition is introduced. So we must make sure that
            the renderer does not use anything from simulation when simulation is running.
            So whenever the simulation wants to render anything, signal the renderer that it
            should render. When the renderer is done, wake the simulation. That is all.

            Disregard everything stated above. It is kept for historical reasons
            (also kept incase I get confused, again). RenderQuerier-interface is
            obsolete since a game copies a scene into memory owned by renderer.
            The scene is copied from the game-thread into and only after
            the copying took place the renderer is informed that there is a new
            scene which should be rendered. This way, there is no shared data
            so there is no need for any locking mechanisms. This change from
            RenderQuerier to copying a scene was made since blocking the game-thread
            whenever the renderer is not feasible since rendering might take 16ms,
            or more if computer is slow. A 16ms block on the game thread is not ok.
            A copy on the other hand is (or I think so, have not actually measured)
            faster than 16ms so it is less of a penalty to copy than to block.
        **/

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        const glm::vec2 windowResolution(width, height);
        const glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f/1080.0f, 0.1f, 1000.0f);

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
                glBindVertexArray(models.at(rd.model)); 
                glDrawElements(GL_TRIANGLES, triangles.at(rd.model)*3, GL_UNSIGNED_SHORT, nullptr);
            } catch(const std::out_of_range& oor) {
                printf("ERROR: Could not lookup vertex array given by render data \"%s\"!\n", rd.model.c_str());
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

            worldUBO.use<WorldUniformData>({view, projection, glm::vec4(suncb.getPos(sunRadians), 1.0), glm::vec4(timeOfDayColor, 0.0), windowResolution});

            if(!program.use()) {
                printf("ERROR: Could not use shader program\n");
                return false;
            }
            if(!program.setTexture("shadowmap", depthFBO.getTexture())) {
                printf("ERROR: Could not set shadowmap\n");
                return false;
            }

            /** vp is the view-projection matrix. Model matrix is provided per renderdata,
                so later on vp will be multiplied with model matrix once per renderdata **/
            const glm::mat4 vp = projection * view;
            for(const auto& rd : rds) {
                try {
                    programs.at(rd.shader).use();
                } catch (const std::out_of_range& e) {
                    printf("ERROR: Could not draw RenderData, there is no shader \"%s\" (there are 0...%zu shaders)\n", rd.shader.c_str(), programs.size());
                    return false;
                } catch (const std::exception& e) {
                    printf("ERROR: Could not draw RenderData %s\n", e.what());
                }

                modelUBO.use<ModelUniformData>({rd.modelMatrix, vp * rd.modelMatrix, sunvp * rd.modelMatrix});
                if(!drawRenderData(rd)) {
                    printf("ERROR: Could not draw a renderdata\n");
                    return false;
                };
            }

            return true;
        }; //End of render2fbo

        /** Blit content of mainFBO to postfxFBO and finally draw postfxFBO to default FBO **/
        auto render2screen = [&] {
            /** Blit mainFBO onto postfxFBO **/
            mainFBO.use(GL_READ_FRAMEBUFFER);
            postfxFBO.use(GL_DRAW_FRAMEBUFFER);
            glViewport(0, 0, width, height);
            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            if(glGetError() != GL_NO_ERROR) {
                printf("ERROR: Could not blit mainFBO to postfxFBO\n");
                return false;
            }

            /** Render quad filling the screen using postfx shaders, where postfx shader got multisampled texture from mainFBO **/
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            postprocessProgram.use();
            if(!postprocessProgram.setUniform("resolution", windowResolution)) return false; 
            if(!postprocessProgram.setTexture("renderedTexture", postfxFBO.getTexture())) return false;
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
            depthProgram.use();
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glClear(GL_DEPTH_BUFFER_BIT);
            for(const auto& rd : rds) {
                modelUBO.use<ModelUniformData>({rd.modelMatrix, viewproj * rd.modelMatrix, viewproj * rd.modelMatrix});
                if(!drawRenderData(rd)) {
                    printf("ERROR: Could not draw a renderdata to depthFBO\n");
                    return false;
                };
            }
            return true;
        };

        if(shouldRender) {
            /** In order to do shadow-mapping, I need to render to FBO. In order to render to FBO
                I need to provide a view matrix for the sun. For now the sun will look at 0.0
                This will only give shadows in the 0.0 region. Later on this can probably be improved
                by letting the sun look at the point which is the foci of the camera, or something along
                those lines (this corresponds to computing shadowmap only where player is looking). **/
            const Scene& scene = scenes.front(); //This assignment to Scene& scene assumes that scenes.front() is only modified by this thread
            const glm::mat4 sunView = glm::lookAt(suncb.getPos(scene.sunRadians), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            const glm::mat4 sunPerspective = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
            const glm::mat4 sunvp = sunPerspective * sunView;
            if(!(
                render2depth(scene.renderDatas, sunvp) &&
                render2fbo(scene.renderDatas, scene.sunRadians, sunvp, scene.view, projection, mainFBO) &&
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
    if(scenes.offer(scene)) {
        shouldRender = true;
    }
}

void Renderer::setPrintFrameTime(bool printFrameTime) {
    this->printFrameTime = printFrameTime;
}

}
