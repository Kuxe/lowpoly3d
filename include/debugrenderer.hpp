#ifndef DEBUGRENDERER_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <cmath>
#include <mutex>
#include "shaderprogram.hpp"
#include "framebuffer.hpp"

namespace lowpoly3d {

/** Class encapsualting graphical debugging functionality.
	This class is assumed to be a member of a parent renderer.
	Whenever the "render" method is called, the debug renderer draws
	the debug primitives (whose coordinates are in world-space) onto
	the binded framebuffer. **/
class DebugRenderer {
	gl::GLuint vbo, vao;
	bool inited = false;

	void error(const char* str) {
		if(gl::glGetError() != gl::GL_NO_ERROR) printf("DebugRenderer: %s\n", str);
	}


public:

	/** Initializes the debug renderer - send lines, circles etc to GPU memory **/
	bool init() {
		using namespace gl;

		//Send line primitive to GPU
		glGenVertexArrays(1, &vao);
		error("Could not generate vao");
		glBindVertexArray(vao);

		const GLfloat lineVertices[] {
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, -1.0f,
			0.0f, 1.0f, -1.0f,
			0.0f, 0.0f, +1.0f,
			1.0f, 0.0f, +1.0f,
			0.0f, 1.0f, +1.0f,
			0.0f, 0.0f, +1.0f,
			0.0f, 1.0f, +1.0f,
			1.0f, 0.0f, +1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 1.0f, -1.0f,
			1.0f, 0.0f, -1.0f,
		};
		glGenBuffers(1, &vbo);
		error("Could not generate vbo");
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		error("Could not bind vbo");
		glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
		error("Could not send line primitive to GPU");

		//Tell OpenGL how it should interpret data in buffers when vao is "on"
		error("Could not bind vao");
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		error("Could not set vertex attrib pointer");
		glEnableVertexAttribArray(0);
		error("Could not enable vertex attrib array");
		glBindVertexArray(0);
		error("Could not unbind vao");
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		error("Could not unbind vbo");

		return inited = true;
	}

	//Render debug primitives. Returns true on success, otherwise false.
	bool render(const glm::mat4& vp, const Framebuffer& fb, const ShaderProgram& sp) {
		using namespace gl;

		if(!inited) {
			printf("ERROR: DebugRenderer has not been initialized. Did you forget to call DebugRenderer::init()?\n");
			return false;
		}

		//FIXME: Figure out why no triangles are shown

		if(!fb.ok()) {
			printf("ERROR: Debug renderer was given a non-ok framebuffer");
			return false;
		}

		if(!fb.use()) {
			printf("ERROR: Debug renderer could not use framebuffer");
			return false;
		}
		glViewport(0, 0, 1920, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if(!sp.use()) {
			printf("ERROR: Debug renderer could not use shaderprogram");
			return false;
		}
		if(!sp.setUniform("mvp", glm::translate(vp, glm::vec3(-10.0f, 5.0f, 2.0f)))) {
			printf("DebugRenderer: Could not set mvp uniform when rendering");
			return false;
		}

		glBindVertexArray(vao);
		error("Could not bind vao when rendering");
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		error("Could not bind vbo when rendering");
		glDrawArrays(GL_TRIANGLES, 0, 12);
		error("Could not draw line");
		glBindVertexArray(0);
		error("Could not unbind vao");

		return true;
	}
};

} //End of namespace lowpoly3d

#endif //DEBUGRENDERER_HPP