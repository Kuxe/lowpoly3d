#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

/** Class representing a framebuffer.
	TODO: Make proper description **/
class Framebuffer final {
	gl::GLuint fbo, renderbuffer, texture;
public:
	Framebuffer(const gl::GLsizei width, const gl::GLsizei height, const gl::GLsizei samples = 0);
	~Framebuffer();
	bool use();
};


#endif //FRAMEBUFFER_HPP