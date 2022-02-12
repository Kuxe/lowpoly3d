#include "shaderprogrambank.hpp"

#include "shaderprogram.hpp"

#include <glbinding/gl/enum.h>
#include <unordered_map>

using namespace gl;

namespace lowpoly3d
{

	ShaderProgramHandle operator"" _sph(const char* str, std::size_t) {
		return ShaderProgramHandle(str);
	}

	struct ShaderProgramBank::Impl
	{
		Impl(std::filesystem::path const& iShaderDirectory)
			: mShaderDirectory(iShaderDirectory)
		{
			assert(std::filesystem::exists(mShaderDirectory));

			struct ShaderDefinition
			{
				gl::GLenum type;
				std::filesystem::path path;
			};

			std::vector<std::string> rawShaderProgramHandles {
				"default",
				"sun",
				"skybox",
				"water",
				"post-process",
				"passthrough",
				"simple",
				"depth",
				"debug",
				"color"
			};

			for(std::string const& iRawShaderProgramHandle : rawShaderProgramHandles) {
				mShaderProgramMap.emplace(iRawShaderProgramHandle, iRawShaderProgramHandle);
			}
		}

		bool link()
		{
			return
				mShaderProgramMap.at("default").link(
					GL_VERTEX_SHADER, mShaderDirectory / "shader.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "shader.frag",
					GL_GEOMETRY_SHADER, mShaderDirectory / "shader.geom") &&
				mShaderProgramMap.at("sun").link(
					GL_VERTEX_SHADER, mShaderDirectory / "sun.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "sun.frag") &&
				mShaderProgramMap.at("skybox").link(
					GL_VERTEX_SHADER, mShaderDirectory / "skybox.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "skybox.frag") &&
				mShaderProgramMap.at("water").link(
					GL_VERTEX_SHADER, mShaderDirectory / "water.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "water.frag") &&
				mShaderProgramMap.at("post-process").link(
					GL_VERTEX_SHADER, mShaderDirectory / "passthrough.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "postprocess.frag") &&
				mShaderProgramMap.at("passthrough").link(
					GL_VERTEX_SHADER, mShaderDirectory / "passthrough.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "passthrough.frag") &&
				mShaderProgramMap.at("simple").link(
					GL_VERTEX_SHADER, mShaderDirectory / "simple.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "simple.frag") &&
				mShaderProgramMap.at("depth").link(
					GL_VERTEX_SHADER, mShaderDirectory / "depth.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "depth.frag") &&
				mShaderProgramMap.at("debug").link(
					GL_VERTEX_SHADER, mShaderDirectory / "debug.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "debug.frag") &&
				mShaderProgramMap.at("color").link(
					GL_VERTEX_SHADER, mShaderDirectory / "shader.vert",
					GL_FRAGMENT_SHADER, mShaderDirectory / "color.frag",
					GL_GEOMETRY_SHADER, mShaderDirectory / "shader.geom");
		}

		ShaderProgram& get(ShaderProgramHandle const& iShaderProgramHandle) {
			assert(mShaderProgramMap.find(iShaderProgramHandle.raw) != end(mShaderProgramMap));
			return mShaderProgramMap.at(iShaderProgramHandle.raw);
		}

		ShaderProgram const& get(ShaderProgramHandle const& iShaderProgramHandle) const {
			assert(mShaderProgramMap.find(iShaderProgramHandle.raw) != end(mShaderProgramMap));
			return mShaderProgramMap.at(iShaderProgramHandle.raw);
		}

		ShaderProgram& operator[](ShaderProgramHandle const& iShaderProgramHandle) {
			return get(iShaderProgramHandle);
		}

		ShaderProgram const& operator[](ShaderProgramHandle const& iShaderProgramHandle) const {
			return get(iShaderProgramHandle);
		}

		std::size_t size() const { return mShaderProgramMap.size(); }

		std::filesystem::path mShaderDirectory;
		std::unordered_map<ShaderProgramHandle::Type, ShaderProgram> mShaderProgramMap;
	};

	ShaderProgramBank::ShaderProgramBank(std::filesystem::path const& iShaderDirectory)
		: mImpl(std::make_unique<Impl>(iShaderDirectory))
	{}

	ShaderProgramBank::~ShaderProgramBank() = default;

	bool ShaderProgramBank::link() {
		return mImpl->link();
	}

	ShaderProgram& ShaderProgramBank::get(ShaderProgramHandle const& iShaderProgramHandle) {
		return mImpl->get(iShaderProgramHandle);
	}

	ShaderProgram const& ShaderProgramBank::get(ShaderProgramHandle const& iShaderProgramHandle) const {
		return mImpl->get(iShaderProgramHandle);
	}
	
	ShaderProgram& ShaderProgramBank::operator[](ShaderProgramHandle const& iShaderProgramHandle) {
		return (*mImpl)[iShaderProgramHandle];
	}

	ShaderProgram const& ShaderProgramBank::operator[](ShaderProgramHandle const& iShaderProgramHandle) const {
		return (*mImpl)[iShaderProgramHandle];
	}

	std::size_t ShaderProgramBank::size() const { return mImpl->size(); }

} // End of namespace lowpoly3d