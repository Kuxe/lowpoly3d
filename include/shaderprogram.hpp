#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/functions-patches.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include "subber.hpp"
#include "events.hpp"
#include <string>

class ShaderProgram : public Subber<rPress> {
private:
	const std::string shaderName;
	const gl::GLuint programHandle;
	std::unordered_map<gl::GLenum, gl::GLuint> shaderHandles;
public:
	ShaderProgram(const std::string& shaderName);
	~ShaderProgram();

	/** Add a shader (eg GL_VERTEX_SHADER) to a shader program,
		compiles the shader and attaches it to this program.
		Returns true on success, otherwise false
	**/
	bool add(gl::GLenum shaderType, const std::string& source);
	bool remove(gl::GLenum shaderType);

	/** Link this program (must be called once before use!).
		Deletes added shaders, which frees up GPU memory.
		Returns true on success, otherwise false.
	**/
	bool link() const;

	/** Use this shader for following draw calls.
		Returns true on success, otherwise false.
	**/
	bool use() const;

	//Assign val to uniform named uniformName
	bool setUniform(const std::string& uniformName, const glm::mat4& m) const;
	bool setUniform(const std::string& uniformName, const glm::vec3& v) const;
	bool setUniform(const std::string& uniformName, const glm::vec2& v) const;
	bool setUniform(const std::string& uniformName, const gl::GLfloat& f) const;
	bool setTexture(const std::string& uniformName, const gl::GLuint& id, const gl::GLenum& target = gl::GL_TEXTURE_2D) const;
	bool setTextureMultisample(const std::string& uniformName, const gl::GLuint& id) const;

	//If r is pressed to a live-reload of the shaderprogram
	void notify(const rPress& event);
};

#endif //SHADERPROGRAM_HPP