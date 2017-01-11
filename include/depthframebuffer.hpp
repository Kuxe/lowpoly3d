#ifndef DEPTHFRAMEBUFFER_HPP
#define DEPTHFRAMEBUFFER_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "subber.hpp"
#include "events.hpp"

/** Class representing a framebuffer. In essence, call
	DepthFramebuffer::use() before drawing anything to the screen,
	and you can use getTexture() which returns a handle to
	a texture with depth of scene rendered onto it. Useful for
	shadow-mapping **/
class DepthFramebuffer final : Subber<OnResize> {
	gl::GLuint fbo, texture;
public:
	DepthFramebuffer(const gl::GLsizei width, const gl::GLsizei height);
	~DepthFramebuffer();
	bool use() const;
	gl::GLuint getTexture() const;
	void notify(const OnResize& evt);
};


#endif //DEPTHFRAMEBUFFER_HPP