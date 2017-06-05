#ifndef UNIFORMBUFFER_HPP
#define UNIFORMBUFFER_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

namespace lowpoly3d {

/** UniformBuffer represents an uniform buffer. The uniformbuffer
	can be use some arbitrary data of type T, which means that
	the data is sent to GPU memory with "name" as uniform name. 
	All it's member are effectively constant throughout life-time,
	except ubo which cannot be initialized using initializer-list due
	to how OpenGLs glGenBuffer work **/
struct UniformBuffer {
	const std::string name;
	const gl::GLuint bindingPoint;
	gl::GLuint ubo;
	UniformBuffer(const std::string& name, const gl::GLuint bindingPoint);

	/** It's unsafe to copy UniformBuffer since on destruction glDeleteBuffers, so if a copy
		is destroyed before the "original" UniformBuffer is, the OpenGL buffer will be prematurely
		deleted and then bad things are bound to happen **/
	UniformBuffer(const UniformBuffer& other) = delete;
	virtual ~UniformBuffer();

	template<typename T>
	bool use(const T& data) const {
		gl::glBindBuffer(gl::GL_UNIFORM_BUFFER, ubo);
		gl::glBindBufferBase(gl::GL_UNIFORM_BUFFER, bindingPoint, ubo);
		gl::glBufferData(gl::GL_UNIFORM_BUFFER, sizeof(T), static_cast<const void*>(&data), gl::GL_DYNAMIC_DRAW);
		if(gl::glGetError() != gl::GL_NO_ERROR) {
			printf("ERROR: Could not send uniform data to UBO %s\n", name.c_str());
			return false;
		}
		return true;
	}
};

}

#endif //UNIFORMBUFFER_HPP