#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <thread>
#include <algorithm>
#include <cmath>
#include "renderer.hpp"
#include "model.hpp"
#include "renderdata.hpp"
#include "deltatime.hpp"
#include "camera.hpp"
#include "events.hpp"
#include "shaderprogram.hpp"
#include "framebuffer.hpp"
#include "depthframebuffer.hpp"

using namespace gl;

/*************/
/** Statics **/
/*************/

std::unordered_set<int> Renderer::keysHeld;

static void error_callback(int error, const char* description) {
    printf("Error: %s\n (%i)", description, error);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    publish(OnResize{width, height});
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        Renderer::keysHeld.insert(key);

        if(key == GLFW_KEY_F3) {
            publish(DrawGridToggle());
        }

    } else if(action == GLFW_RELEASE) {
        Renderer::keysHeld.erase(key);
    }
}

static void cursor_enter_callback(GLFWwindow* window, int focused) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if(focused) {
        publish(CursorEnterWindow{xpos, ypos});
    } else {
        publish(CursorExitWindow{xpos, ypos});
    }
}

static void mouse_callback(GLFWwindow* window, double x, double y) {
    publish(MouseEvent{x, y});
}

/*********************/
/** Private methods **/
/*********************/

//Empty over here AS IT SHOuLD BE USE LAMBDAS

/*********************/
/** Public methods **/
/*********************/

bool Renderer::initialize() {
    glfwSetErrorCallback(error_callback);
    if(!glfwInit()) {
        fprintf(stderr, "Could not load glfw (call to glfwInit returned 0)\n");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(480, 270, "Lowpoly3D", NULL, NULL);
    if(!window) {
        fprintf(stderr, "Could open window (call to glfwCreateWindow returned NULL)\n");
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(23.0f/255.0f, 126.0f/255.0f, 137.0f/255.0f, 255.0f/255.0f);
    glEnable(GL_MULTISAMPLE);
    return true;
}

void Renderer::terminate() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

int Renderer::loadModel(const Model& model) {
    //Send model to GPU memory and return handle
    //to the model stored in GPU memory such that
    //we later on easily can tell the GPU
    //"Mr GPU, please render the model on handle which I've stored in your memory!"
    //which is done with a glBindVertex(handle)-call

    GLuint vertexBuffer, colorBuffer, indexBuffer, vertexArray;

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * model.vertices.size(), &model.vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * model.colors.size(), &model.colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triangle) * model.triangles.size(), &model.triangles[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    const auto error = glGetError();
    if(error != GL_NO_ERROR) {
        printf("ERROR: Could not load model\n");
        return -1;
    }

    triangles[vertexArray] = model.triangles.size();

    glBindVertexArray(0); //Dont let subsequent calls work on vertexArray
    return vertexArray;
}

std::vector<int> Renderer::loadModels(const std::vector<Model>& models) {
    std::vector<int> vertexArrays(models.size());
    std::transform(models.begin(), models.end(), vertexArrays.begin(), [this](const Model& m) { return this->loadModel(m); });
    return vertexArrays;
}

bool Renderer::render(RenderQuerier& rq) const {
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
    ShaderProgram program("Default shader");
    if(!(
        program.add(GL_VERTEX_SHADER, "../shaders/shader.vert") &&
        program.add(GL_FRAGMENT_SHADER, "../shaders/shader.frag") &&
        program.add(GL_GEOMETRY_SHADER, "../shaders/shader.geom") && 
        program.link())) {
        return false;
    }

    ShaderProgram sunProgram("Sun shader");
    if(!(
        sunProgram.add(GL_VERTEX_SHADER, "../shaders/sun.vert") &&
        sunProgram.add(GL_FRAGMENT_SHADER, "../shaders/sun.frag") &&
        sunProgram.link())) {
        return false;
     }

     ShaderProgram skyboxProgram("Skybox shader");
     if(!(
        skyboxProgram.add(GL_VERTEX_SHADER, "../shaders/skybox.vert") &&
        skyboxProgram.add(GL_FRAGMENT_SHADER, "../shaders/skybox.frag") &&
        skyboxProgram.link())) {
        return false;
    }

    ShaderProgram waterProgram("Water shader");
    if(!(
        waterProgram.add(GL_VERTEX_SHADER, "../shaders/water.vert") &&
        waterProgram.add(GL_FRAGMENT_SHADER, "../shaders/water.frag") &&
        waterProgram.link())) {
        return false;
    }

    ShaderProgram postprocessProgram("Post-process shader");
    if(!(
        postprocessProgram.add(GL_VERTEX_SHADER, "../shaders/passthrough.vert") &&
        postprocessProgram.add(GL_FRAGMENT_SHADER, "../shaders/postprocess.frag") &&
        postprocessProgram.link())) {
        return false;
    }

    ShaderProgram depthProgram("Depth shader");
    if(!(
        depthProgram.add(GL_VERTEX_SHADER, "../shaders/depth.vert") &&
        depthProgram.add(GL_FRAGMENT_SHADER, "../shaders/depth.frag") &&
        depthProgram.link())) {
        return false;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    /** mainFBO is the fbo where the actual full-scene is rendered.
        The FBO pipeline is as follows:
                          draw mainFBO w MSAA          Render mainFBO to postProcessFBO w postfx shaders (no MSAA)           Blit postfxFBO to defaultFBO
            depthFBO --------------------> mainFBO ----------------------------------------------------------> postfxFBO ----------------------------> defaultFBO
    **/
    constexpr GLsizei numMultisamples = 4;
    constexpr uint16_t SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
    Framebuffer mainFBO("Main FBO", width, height, numMultisamples);
    Framebuffer postfxFBO("PostFX FBO", width, height);
    DepthFramebuffer depthFBO(SHADOW_WIDTH, SHADOW_HEIGHT);

    std::unordered_map<int, const ShaderProgram&> programs;
    programs.insert({0, program});
    programs.insert({1, sunProgram});
    programs.insert({2, skyboxProgram});
    programs.insert({3, waterProgram});

    rq.rendererActive = true;

    while(!glfwWindowShouldClose(window)) {

        /** Explanation of threading in renderer (why and how):
            First of all, the simulation part of game (logic, physics, AI etc)
            should be decoupled from the rendering engine. This is because
            the code would get messy otherwise. In the best of worlds, only a
            minimal set of data flows from the "game" into the renderer.
            This minimal set of data is, for most part, called "RenderData".
            Other information that the renderer need from the game is position of sun,
            position of camera and some other things. So an interface which the game implements
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
        **/

        DeltaTime::start();
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        const glm::mat4 projection = glm::perspective(glm::radians(50.0f), 16.0f/9.0f, 0.1f, 1000.0f);

        /** Draws a set of renderdatas for any view-projection matrix.
            This lambda does NOT bind anything but the vertex array of each render data,
            so you need to specify what fbo or what shaders to use beforehand **/
        //TODO: Redesign such that instanced indexed drawing is used
        const auto drawRenderData = [&](const RenderData& rd, const glm::mat4& vp) {
            //Calculate the MVP-matrix and send it to vertex shader
            /** FIXME: THIS IS UGLY BECAUSE IT ASSUMES THAT ALL SHADERS HAVE "mvp" UNIFORM! NOT NECCESARILY TRUE **/
            /** IDEALLY SHADERS SHOULD KNOW THEIR UNIFORMS SUCH THAT I THEY AUTOMATICALLY SET THEM ON .use() **/
            if(!program.setUniform("mvp", vp * rd.modelMatrix)) {
                printf("ERROR: Could not set mvp in shader\n");
                return false;
            }

            glBindVertexArray(rd.vertexArray); 
            try {
                glDrawElements(GL_TRIANGLES, triangles.at(rd.vertexArray)*3, GL_UNSIGNED_SHORT, nullptr);
            } catch(std::out_of_range oor) {
                printf("ERROR: Could not lookup vertex array %i given by render data!\n", rd.vertexArray);
                return false;
            }
            return true;
        };

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
            const float t = clippedcos(rq.getSunOmega(), 5.0f);
            return lerp(midnightColor, noonColor, t) / 255.0f;
        }

        /** Draws a set of renderdatas, the sun from POV of camera to an framebuffer object
            This is the place where the whole scene, with per-model shaders, is drawn **/
        //TODO: Set uniforms as a function of current shader (skybox shaders warrants for other uniforms)
        const auto render2fbo = [&](
            const RenderDatas& rds,
            const RenderData& sunRd,
            const glm::mat4& sunvp,
            const glm::mat4& view,
            const glm::mat4& projection,
            const Framebuffer& fbo) {
            fbo.use();
            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if(!program.use()) {
                printf("ERROR: Could not use shader program\n");
                return false;
            }

            /** Compute clear color as function of time to match the sunset and sunrise **/
            const glm::vec3 sunPos = { sunRd.modelMatrix[3] };
            const glm::vec3 timeOfDayColor = timeOfDayColorLambda(t);
            glClearColor(timeOfDayColor.x, timeOfDayColor.y, timeOfDayColor.z, 1.0f);

            program.use();
            if(!program.setUniform("view", view)) return false;
            if(!program.setUniform("projection", projection)) return false;
            if(!program.setUniform("sunPos", sunPos)) return false;
            if(!program.setUniform("windowResolution", {width, height})) return false;
            if(!program.setUniform("timeOfDayColor", timeOfDayColor)) return false;
            if(!program.setTexture("shadowmap", depthFBO.getTexture())) return false;

            skyboxProgram.use();
            if(!skyboxProgram.setUniform("skyColor", timeOfDayColor)) return false;
            if(!skyboxProgram.setUniform("sunPos", sunPos)) return false;
            if(!skyboxProgram.setUniform("view", view)) return false;
            if(!skyboxProgram.setUniform("projection", projection)) return false;
            if(!skyboxProgram.setUniform("windowResolution", glm::vec2(width, height))) return false;

            /** vp is the view-projection matrix. Model matrix is provided per renderdata,
                so later on vp will be multiplied with model matrix once per renderdata **/
            const glm::mat4 vp = projection * view;
            for(const auto& rd : rds) {

                //FIXME: This is dirty solution. I only need to send sunmvp to shader matrix if programs.at(rd.shader) return a shader where sunmvp is needed
                program.use();
                if(!program.setUniform("sunmvp", sunvp * rd.modelMatrix)) return false;
                //End of fixme

                try {
                    programs.at(rd.shader).use();
                } catch (std::out_of_range) {
                    printf("ERROR: Could not draw RenderData, there is no shader %i (there are 0...%llu shaders)\n", rd.shader, programs.size());
                    return false;
                }

                if(!drawRenderData(rd, vp)) {
                    printf("ERROR: Could not draw a renderdata\n");
                    return false;
                };
            }

            /** Praise the sun! **/
            if(!drawRenderData(sunRd, vp)) {
                printf("ERROR: Could not draw the sun\n");
                return false;
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
            if(!postprocessProgram.setUniform("resolution", glm::vec2(width, height))) return false; 
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
                if(!drawRenderData(rd, viewproj)) {
                    printf("ERROR: Could not draw a renderdata to depthFBO\n");
                    return false;
                };
            }
            return true;
        };

        /** Critical section **/
        if(rq.shouldRender) {
            bool ok = true;
            const RenderDatas& rds = rq.getRenderDatas(); //Shared data
            const RenderData& sunRd = rq.getSunRenderData(); //Shared data
            const Camera& camera = rq.getCamera(); //Shared data
            const glm::mat4& view = camera.get();

            /** In order to do shadow-mapping, I need to render to FBO. In order to render to FBO
                I need to provide a view matrix for the sun. For now the sun will look at 0.0
                This will only give shadows in the 0.0 region. Later on this can probably be improved
                by letting the sun look at the point which is the foci of the camera, or something along
                those lines (this corresponds to computing shadowmap only where player is looking).
                The lookat point will probably depend on the camera (which is a shared resource)
                and I dont want to introduce any race conditions by mistake later on, so keep this
                code here **/
            const glm::vec3 sunPos = glm::column(sunRd.modelMatrix, 3);
            const glm::mat4 sunView = glm::lookAt(sunPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            const glm::mat4 sunPerspective = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
            const glm::mat4 sunvp = sunPerspective * sunView;
            ok =
                render2depth(rds, sunvp) &&
                render2fbo(rds, sunRd, sunvp, view, projection, mainFBO) &&
                render2screen();

            rq.signalSimulation();
            if(!ok) {
                rq.rendererActive = false;
                return false;
            }
        }
        /** End of critical section **/

        glfwPollEvents();
        handleHeldKeys();
        glfwSwapBuffers(window);

        if(glGetError() != GL_NO_ERROR) {
            printf("ERROR: Unknown error in renderer!\n");
            return false;
        }

        DeltaTime::stop();
    }
    rq.rendererActive = false; //This one take care of all possible waits happening _later_ in time
    rq.cv.notify_one(); //This one take care of all possible waits that happened _before_ in time
    //So any wait is dealt with here!
    return true;
}

void Renderer::handleHeldKeys() const {
    for(int key : keysHeld) {
        switch(key) {
            case GLFW_KEY_ESCAPE: {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            } break;
            case GLFW_KEY_W: {
                publish(wPress());
            } break;
            case GLFW_KEY_A: {
                publish(aPress());
            } break;
            case GLFW_KEY_S: {
                publish(sPress());
            } break;
            case GLFW_KEY_D: {
                publish(dPress());
            } break;
            case GLFW_KEY_R: {
                publish(rPress());
            } break;
            case GLFW_KEY_E: {
                publish(ePress());
            } break;
            case GLFW_KEY_Q: {
                publish(qPress());
            } break;
            case GLFW_KEY_P: {
                publish(pPress());
            } break;
            case GLFW_KEY_T: {
                publish(tPress());
            } break;
        }
    }
}

void Renderer::notify(const DrawGridToggle& event) {
    drawGrid = !drawGrid;
}
