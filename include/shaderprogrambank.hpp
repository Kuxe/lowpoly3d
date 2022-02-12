#ifndef SHADERPROGRAMBANK_HPP
#define SHADERPROGRAMBANK_HPP

#include <filesystem>
#include <string>
#include <memory>

namespace lowpoly3d
{

	class ShaderProgram;

	struct ShaderProgramHandle {
		ShaderProgramHandle(std::string raw)
			: raw(std::move(raw))
		{}

		using Type = std::string;
		Type raw;
	};

	ShaderProgramHandle operator"" _sph(const char* str, std::size_t);


	class ShaderProgramBank final
	{
	public:
		ShaderProgramBank(std::filesystem::path const& iShaderDirectory);
		~ShaderProgramBank();
		
		// Link all shaders
		bool link();

		ShaderProgram& get(ShaderProgramHandle const& iShaderProgramHandle);
		ShaderProgram const& get(ShaderProgramHandle const& iShaderProgramHandle) const;
		ShaderProgram& operator[](ShaderProgramHandle const& iShaderProgramHandle);
		ShaderProgram const& operator[](ShaderProgramHandle const& iShaderProgramHandle) const;

		std::size_t size() const;

	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;
	};

} // End of namespace lowpoly3d

#endif // SHADERPROGRAMBANK_HPP