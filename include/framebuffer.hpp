#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "subber.hpp"
#include "events.hpp"

/** Class representing a framebuffer. In essence, call
	Framebuffer::use() before drawing anything to the screen,
	and you can use getTexture() which returns a handle to
	a texture with scene rendered onto it. Useful for
	post-processing or shadow-mapping **/
class Framebuffer final : Subber<OnResize> {
	gl::GLuint fbo, renderbuffer, texture;
public:
	Framebuffer(const gl::GLsizei width, const gl::GLsizei height, const gl::GLsizei samples = 0);
	~Framebuffer();
	bool use() const;
	gl::GLuint getTexture() const;
	void notify(const OnResize& evt);
};


#endif //FRAMEBUFFER_HPP