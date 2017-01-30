#ifndef UNIFORMBUFFER_HPP
#define UNIFORMBUFFER_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

namespace lowpoly3d {

struct UniformBuffer {
	const std::string name;
	gl::GLuint ubo, bindingPoint;
	UniformBuffer(const std::string& name, const gl::GLuint bindingPoint);
	~UniformBuffer();

	template<typename T>
	bool use(const T& data) {
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