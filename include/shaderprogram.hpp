#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <glbinding/gl/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <string>
#include "subber.hpp"
#include "events.hpp"

namespace lowpoly3d {

struct UniformBuffer;
class ShaderProgram : public Subber<rPress> {
private:
	const gl::GLuint programHandle;

	/** Each shader metdata in this shaderprogram is stored "shaders".
		These metadata are needed when live-recompiling shaders **/
	struct ShaderMetadata {
		gl::GLuint handle;
		std::string path;
	};

	std::unordered_map<gl::GLenum, ShaderMetadata> shaders;
	std::unordered_map<std::string, const UniformBuffer*> ubos;
public:
	const std::string name;

	ShaderProgram(const std::string& shaderName);
	~ShaderProgram();

	/** Add a shader (eg GL_VERTEX_SHADER) to a shader program,
		compiles the shader and attaches it to this program.
		Returns true on success, otherwise false. You can also
		call add(shaderType, "source", shaderType, "source" ...)
	**/

	bool add(gl::GLenum shaderType, const std::string& source);
	bool add() const { return true; }
	template<typename T, typename S, typename... Pack>
	bool add(const T& t, const S& s, const Pack&... pack) {
		return add(t, s) && add(pack...);
	}

	bool remove(gl::GLenum shaderType);

	/** Link this program (must be called once before use!).
		Deletes added shaders, which frees up GPU memory.
		Returns true on success, otherwise false.
		You can also use this similarily to that of variadic add()
		which in addition to variadic add() also will link.
		Most of the time you probably want to use variadic link().
	**/
	bool link() const;
	template<typename T, typename S, typename... Pack>
	bool link(const T& t, const S& s, const Pack&... pack) {
		return add(t, s) && add(pack...) && link();
	}

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

	bool setUBO(const std::string& blockName, const UniformBuffer& ubo);

	//If r is pressed to a live-reload of the shaderprogram
	void notify(const rPress& event);
};

}

#endif //SHADERPROGRAM_HPP