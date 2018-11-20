#ifndef GLFRAME_HPP
#define GLFRAME_HPP

#include <glbinding/gl/types.h> // GLuint
#include <glm/mat4x4.hpp>

namespace lowpoly3d {

class ShaderProgram;
class UniformBuffer;

/* Class that can draw a frame to the currently bound framebuffer */
class GLFrame final {
public:
	GLFrame();
	~GLFrame();
	void draw(
		const glm::mat4& frame,
		ShaderProgram& program,
		UniformBuffer& ubo,
		const float lineWidth);

private:
	gl::GLuint lineVertexBuffer, lineColorBuffer, lineVertexVAO;
	float frameSize = 0.1f;
};

}

#endif //GLFRAME_HPP