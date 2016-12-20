#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <thread>
#include <algorithm>
#include <cmath>
#include "renderer.hpp"
#include "scene.hpp"
#include "model.hpp"
#include "renderdata.hpp"
#include "deltatime.hpp"
#include "camera.hpp"
#include "events.hpp"
#include "shaderprogram.hpp"

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

bool Renderer::drawRenderData(const ShaderProgram& program, const RenderData& rd, const glm::mat4& vp) const {
    program.use();
    //Calculate the MVP-matrix
    const glm::mat4 mvp = vp * rd.modelMatrix;

    //Send MVP matrix (per renderData) to vertex shader
    if(!program.setUniform("mvp", mvp)) {
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
}

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

    while(!glfwWindowShouldClose(window)) {
        /** Run simulation one tick and query for data which I (the renderer) wants **/
        rq.update();
        const std::vector<RenderData>& renderDatas = rq.getRenderDatas();
        const RenderData& sunRd = rq.getSunRenderData();
        const Camera& camera = rq.getCamera();

        DeltaTime::start();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Lambda function for rendering the scene
        //Useful to encapsulate rendering calls and required calls in lambda, because it just so happens
        //that it is handy to render the scene several times per frame (MSAA)
        const auto renderScene = [&]() {
            if(!program.use()) {
                printf("ERROR: Could not use shader program\n");
                return false;
            }

            /** Compute clear color as function of time to match the sunset and sunrise **/
            glm::vec3 sunPos = {
                sunRd.modelMatrix[3]
            };

            //Add blue tone at night
            const glm::vec3 sunsetColor(250, 214, 165);
            const glm::vec3 sunriseColor(49,47,102);
            const glm::vec3 cloudyColor(189,190,192);
            const glm::vec3 noonColor(64, 156, 255);
            const glm::vec3 midnightColor(5,0,27);
            const auto sin01 = [](float t) { return 0.5f*sinf(t)+0.5f; };
            const auto fastsiglerp = [](const glm::vec3& a, const glm::vec3& b, float t) -> glm::vec3 {
                return a*(t/(1.0f+fabsf(t))) + b*((1.0f-t)/(1.0f+fabsf(t)));
            };
            glm::vec3 timeOfDayColor = fastsiglerp(midnightColor, noonColor, sin01(rq.getSunOmega())) / 255.0f;
            const glm::vec3 extraSunDay = glm::max(glm::vec3(0.0f, 0.0f, 0.0f), glm::sqrt(sunriseColor*2.0f));
            const glm::vec3 clearColor = timeOfDayColor + extraSunDay/255.0f;
            glClearColor(clearColor.x, clearColor.y, clearColor.z, 255.0f/255.0f);
            //Send position of sun to fragment shader
            if(!program.setUniform("timeOfDayColor", timeOfDayColor)) {
                return false;
            }

            const glm::mat4 view = camera.get();
            const glm::mat4 projection = glm::perspective(glm::radians(50.0f), 16.0f/9.0f, 0.1f, 1000.0f);
            if(!program.setUniform("view", view)) return false;
            if(!program.setUniform("projection", projection)) return false;

            //Send position of sun to fragment shader
            if(!program.setUniform("sunPos", sunPos)) {
                return false;
            }

            //Send window resolution to fragment shader
            if(!program.setUniform("windowResolution", {width, height})) {
                return false;
            }

            const glm::mat4 projectionMatrix = glm::perspective(glm::radians(50.0f), 16.0f/9.0f, 0.1f, 1000.0f);
            const glm::mat4 vp = projectionMatrix * camera.get();
            //TODO: Redesign such that instanced indexed drawing is used
            for(const auto& rd : renderDatas) {
                drawRenderData(program, rd, vp);
            }

            /** Praise the sun! **/
            if(!drawRenderData(sunProgram, sunRd, vp)) {
                printf("ERROR: Could not draw the sun\n");
                return false;
            }

            return true;
        };

        if(!renderScene()) {
            printf("ERROR: Could not render scene\n");
            return false;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        handleHeldKeys();
        std::this_thread::sleep_until(DeltaTime::deltaTimeStart + std::chrono::microseconds(1666));
        DeltaTime::stop();
    }
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
