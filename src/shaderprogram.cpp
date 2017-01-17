#include "shaderprogram.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "uniformbuffer.hpp"

using namespace gl;

ShaderProgram::ShaderProgram(const std::string& name) : programHandle(glCreateProgram()), name(name) { }
ShaderProgram::~ShaderProgram() {
	glDeleteProgram(programHandle);
}

bool ShaderProgram::add(GLenum shaderType, const std::string& path) {

	//Load shader on path to sourcePtr
	std::ifstream ifs(path);
	if(!ifs.is_open()) {
		printf("Could open shader filestream for file %s\n", path.c_str());
		return false;
	}
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	ifs.close();
	const std::string sourceStr = buffer.str();
	const char* sourceStrPtr = sourceStr.c_str();

	//Create and compile shader
	GLuint shaderHandle = glCreateShader(shaderType);
	if(!shaderHandle) {
		printf("ERROR: call to glCreateShader() returned 0 for %s\n", path.c_str());
		return false;
	}

	glShaderSource(shaderHandle, 1, &sourceStrPtr, NULL);
    glCompileShader(shaderHandle);

    //Check if any errors occurred
    GLboolean success;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, reinterpret_cast<GLint*>(&success));
    if(success == GL_FALSE) {
    	GLint length;
	    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
	    std::vector<GLchar> log(length);
		glGetShaderInfoLog(shaderHandle, length, 0, &log[0]);
		printf("ERROR: Could not compile %s (%s)\n", path.c_str(), &log[0]);
		return false;
    }

    shaderHandles[shaderType] = shaderHandle;

	//Finally attach shader to program
    glAttachShader(programHandle, shaderHandle);

    /** Don't worry about this call. It only flags shaderHandle for deletion,
    	which is fine since the shaderHandle is attached to programHandle
    	and therefore the shader is only deleted after deletion of programHandle
    **/
    glDeleteShader(shaderHandle);
    return true;
}

bool ShaderProgram::remove(gl::GLenum shaderType) {
	try {
		glDetachShader(programHandle, shaderHandles.at(shaderType));
		shaderHandles.erase(shaderType);
	} catch (std::out_of_range e) {
		printf("ERROR: Could not detach shader (was shadertype %i ever created?)\n", static_cast<int>(shaderType));
		return false;
	}
	return true;
}

bool ShaderProgram::link() const {
	glLinkProgram(programHandle);
	GLboolean success = GL_TRUE;
	glGetProgramiv(programHandle, GL_LINK_STATUS, reinterpret_cast<GLint*>(&success));
	if(success == GL_FALSE) {
		GLint length;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> log(length);
		glGetProgramInfoLog(programHandle, length, &length, &log[0]);
		printf("ERROR: Could not link the shader program \"%s\" (%s)\n", name.c_str(), &log[0]);
		return false;
	}
	return true;

}

bool ShaderProgram::use() const {
	glUseProgram(programHandle);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Can not use shader program\n");
		return false;
	}
	return true;
}

bool ShaderProgram::setUniform(const std::string& uniformName, const glm::mat4& m) const {
	const GLint uniformLocation = glGetUniformLocation(programHandle, uniformName.c_str());
	if(uniformLocation == -1) {
		printf("ERROR: No uniform named %s in %s\n", uniformName.c_str(), name.c_str());
		return false;
	}

	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m));
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set uniform %s\n", uniformName.c_str());
		return false;
	}

	return true;
}

bool ShaderProgram::setUniform(const std::string& uniformName, const glm::vec3& v) const {
	const GLint uniformLocation = glGetUniformLocation(programHandle, uniformName.c_str());
	if(uniformLocation == -1) {
		printf("ERROR: No uniform named %s in %s\n", uniformName.c_str(), name.c_str());
		return false;
	}

	glUniform3fv(uniformLocation, 1, glm::value_ptr(v));
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set uniform %s\n", uniformName.c_str());
		return false;
	}

	return true;
}

bool ShaderProgram::setUniform(const std::string& uniformName, const glm::vec2& v) const {
	const GLint uniformLocation = glGetUniformLocation(programHandle, uniformName.c_str());
	if(uniformLocation == -1) {
		printf("ERROR: No uniform named %s in %s\n", uniformName.c_str(), name.c_str());
		return false;
	}

	glUniform2fv(uniformLocation, 1, glm::value_ptr(v));
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set uniform %s\n", uniformName.c_str());
		return false;
	}

	return true;
}

bool ShaderProgram::setUniform(const std::string& uniformName, const gl::GLfloat& f) const {
	const GLint uniformLocation = glGetUniformLocation(programHandle, uniformName.c_str());
	if(uniformLocation == -1) {
		printf("ERROR: No uniform named %s in %s\n", uniformName.c_str(), name.c_str());
		return false;
	}

	glUniform1f(uniformLocation, f);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set uniform %s\n", uniformName.c_str());
		return false;
	}

	return true;
}

bool ShaderProgram::setTexture(const std::string& uniformName, const GLuint& id, const GLenum& target) const {
	if(!glIsTexture(id)) {
		printf("ERROR: ShaderProgram could not set texture with id %u because %u is not a texture\n", id, id);
		return false;
	}

	const GLint uniformLocation = glGetUniformLocation(programHandle, uniformName.c_str());
	if(uniformLocation == -1) {
		printf("ERROR: No texture uniform named %s in %s\n", uniformName.c_str(), name.c_str());
		return false;
	}

	glBindTexture(target, id);
	glUniform1i(uniformLocation, 0);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set texture uniform %s\n", uniformName.c_str());
		return false;
	}
	return true;
}

bool ShaderProgram::setTextureMultisample(const std::string& uniformName, const GLuint& id) const {
	return setTexture(uniformName, id, GL_TEXTURE_2D_MULTISAMPLE);
}

bool ShaderProgram::setUBO(const std::string& blockName, const UniformBuffer& ubo) {
	GLuint blockIndex =  glGetUniformBlockIndex(programHandle, blockName.c_str());
	if(blockIndex == GL_INVALID_INDEX) {
		printf("ERROR: No uniform block index named \"%s\" in %s\n", blockName.c_str(), name.c_str());
		return false;
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, ubo.bindingPoint, ubo.ubo);
	glUniformBlockBinding(programHandle, blockIndex, ubo.bindingPoint);
	if(glGetError() != GL_NO_ERROR) {
		printf("ERROR: Could not set uniform buffer object in %s\n", name.c_str());
		return false;
	}
	return true;
}


void ShaderProgram::notify(const rPress& event) {
	for(const auto& pair : shaderHandles) {
		const auto& key = pair.first;
		if(!remove(key)) {
			printf("ERROR: Could not remove shader \"%s\" during live-reload\n", name.c_str());
			return;
		}
	}

	if(!(
		add(GL_VERTEX_SHADER, "../shaders/shader.vert") &&
		add(GL_FRAGMENT_SHADER, "../shaders/shader.frag") &&
		add(GL_GEOMETRY_SHADER, "../shaders/shader.geom") &&
		link())) {
		printf("ERROR: Can not live-reload shader program\n");
    }
}
