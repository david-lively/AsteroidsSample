#ifndef SHADER_H
#define SHADER_H

#include "Resource.h"
#include "Enums.h"
#include "UniformCollection.h"

#include <string>
#include <map>

class Shader :
	public Resource
{
public:
	
	Shader();
	Shader(const std::string& name);
	~Shader();

	GLuint Build(const std::string& basePath);
	void Bind() override;
	void Unbind() override;

	GLint GetAttribLocation(const std::string& name)
	{
		return gl::GetAttribLocation(Handle, name.c_str());
	}

	GLuint GetUniformLocation(const std::string& name)
	{
		return gl::GetUniformLocation(Handle, name.c_str());
	}

	void OnReload(const std::string& tag) override;

private:
	std::string m_basePath;
	std::map<ShaderType, GLuint> m_shaders;

	std::string Preprocess(const std::string& source);

	std::string GetShaderFilename(const std::string& basePath, const ShaderType shaderType) const;
	bool LoadSources(const std::string& basePath, std::map<ShaderType, std::string>& sources);
	static bool CompileSuccessful(const GLint shader);
	static bool LinkSuccessful(const GLint program);

	static std::string GetProgramLog(const GLuint program);
	static std::string GetShaderLog(GLuint shader);

	GLint CompileShader(const std::string& source);



};

#endif

