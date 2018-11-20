#include "glframe.hpp"

#include <array>

#include <glbinding/gl/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "mvpuniformdata.hpp"
#include "shaderprogram.hpp"
#include "uniformbuffer.hpp"

namespace lowpoly3d {

using namespace gl;

GLFrame::GLFrame() {
	const glm::mat3 identity(1.0f);
	
	std::array<glm::vec3, 6> lineVerts = {
		//x-axis
		glm::vec3(0.0f, 0.0f, 0.0f),
		identity[0],

		//y-axis
		glm::vec3(0.0f, 0.0f, 0.0f),
		identity[1],

		//z-axis
		glm::vec3(0.0f, 0.0f, 0.0f),
		identity[2],
	};

	std::array<glm::vec3, 6> lineColors = {
		//x-axis
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		//y-axis
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		//z-axis
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};
	
	glGenBuffers(1, &lineVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lineVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerts), lineVerts.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &lineVertexVAO);
	glBindVertexArray(lineVertexVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &lineColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lineColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineColors), lineColors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLFrame::~GLFrame() {
	glDeleteBuffers(1, &lineVertexBuffer);
	glDeleteVertexArrays(1, &lineVertexVAO);
}

void GLFrame::draw(const glm::mat4& frame, ShaderProgram& program, UniformBuffer& ubo, const float lineWidth) {
	GLfloat storedLineWidth;
	glGetFloatv(GL_LINE_WIDTH, &storedLineWidth);
	glLineWidth(lineWidth);

	program.use();
	ubo.use<MVPUniformData>(frame);

	glBindVertexArray(lineVertexVAO);

	glDrawArrays(GL_LINES, 0, 6);

	glLineWidth(storedLineWidth); // Restore previous line width

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not draw world axes\n");
	}
}

}