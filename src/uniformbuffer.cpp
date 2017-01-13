#include "uniformbuffer.hpp"

using namespace gl;

UniformBuffer::UniformBuffer(const std::string& name, const gl::GLuint bindingPoint)
	: name(name), bindingPoint(bindingPoint) {
    glGenBuffers(1, &ubo);
}

UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &ubo);
}