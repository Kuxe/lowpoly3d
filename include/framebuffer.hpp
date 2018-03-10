#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "subber.hpp"
#include "events.hpp"

namespace lowpoly3d {

/** Class representing a framebuffer. In essence, call
	Framebuffer::use() before drawing anything to the screen,
	and you can use getTexture() which returns a handle to
	a texture with scene rendered onto it. Useful for
	post-processing **/
class Framebuffer final : Subber<OnResize> {
	const std::string framebufferName;
	gl::GLuint fbo, renderbuffer, texture;
	const gl::GLsizei samples;
	const bool multisampled;
public:
	Framebuffer(const std::string& framebufferName, const gl::GLsizei width, const gl::GLsizei height, const gl::GLsizei samples = 0);
	~Framebuffer();
	bool use(const gl::GLenum target = gl::GL_FRAMEBUFFER) const;
	bool ok() const;
	gl::GLuint getTexture() const;
	void notify(const OnResize& evt);
};

}

#endif //FRAMEBUFFER_HPP