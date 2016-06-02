#include <string>
#include <map>

using namespace std;


#include "Shader.h"
#include "Common.h"
#include "Files.h"
#include "Enums.h"
#include "ShaderProcessor.h"

Shader::Shader() : Shader("unknown.shader")
{

}

Shader::Shader(const string& name) : Resource(name)
{

}

Shader::~Shader()
{
}

void Shader::OnReload(const string& tag)
{
	if (tag == "shader")
		Build(m_basePath);
}

static map<ShaderType, string> extensions =
{
	{ ShaderType::FragmentShader, "frag" },
	{ ShaderType::VertexShader, "vert" },
	{ ShaderType::GeometryShader, "geom" },
	{ ShaderType::TessEvaluationShader, "tessEval" },
	{ ShaderType::TessControlShader, "tessControl" },
	{ ShaderType::ComputeShader, "compute" }
};


GLuint Shader::Build(const string& basePath)
{
	m_basePath = basePath;

	Log::Info << "Building m_shaders for \"" << basePath << "\"" << endl;


	map<ShaderType, GLuint> shaders;

	GLuint programId = gl::CreateProgram();
	bool success = true;

	do
	{
		for (auto ext : extensions)
		{
			auto filename = basePath + "." + ext.second + ".glsl";

			if (!Files::Exists(filename))
				continue;

			if (shaders.count(ext.first) > 0)
			{
				auto id = shaders[ext.first];
				gl::DetachShader(programId, id);
				gl::DeleteShader(id);
			}

			shaders[ext.first] = CompileShader(filename, ext.first);
		}

		if (shaders.size() == 0)
		{
			success = false;
			Log::Error << "No shaders found! Press enter to retry.\n";
			getchar();
			continue;
		}

		for (auto shader : shaders)
		{
			gl::AttachShader(programId, shader.second);
		}

		gl::LinkProgram(programId);

		GLint linkStatus;
		gl::GetProgramiv(programId, gl::LINK_STATUS, &linkStatus);

		if (!linkStatus)
		{
			success = false;

			Log::Error << "Program link error for \"" << basePath << "\"" << endl;
			Log::Error << GetProgramLog(programId) << endl;
			Log::Error << "Press enter to retry." << endl;
			
			getchar();

		}


	} while (!success);

	Handle = programId;

	return Handle;
}


string Shader::GetShaderFilename(const std::string& basePath, ShaderType shaderType) const
{
	return basePath + "." + extensions[shaderType] + ".glsl";
}

GLuint Shader::CompileShader(const std::string& path, const ShaderType shaderType)
{
	/*
	1. Generate a shader
	2. Preprocess, emit any found errors (missing #includes, etc.)
	3. Pass to driver -gl::CompileShader
	4. Check compile log, emit any errors
	5. Repeat until successful
	6. Return GL id for the shader.
	*/
	bool success = true;
	
	auto shader = gl::CreateShader((GLenum)shaderType);
	
	do
	{
		string source = Files::Read(path);
		source = ShaderProcessor::Parse(source);

		const GLchar* const src = source.c_str();
		const GLint len = source.size();


		gl::ShaderSource(shader, 1, &src, &len);

		gl::CompileShader(shader);

		GLint status;
		gl::GetShaderiv(shader, gl::COMPILE_STATUS, &status);

		success = status != 0;

		if (!success)
		{
			Log::Error << "Error when building shader \"" << path << "\"" << endl;
			Log::Error << "Log:\n" << GetShaderLog(shader) << endl;
			Log::Error << "Press enter to retry.\n";
			getchar();
		}

	} while (!success);

	return shader;

}



bool Shader::LoadSources(const std::string& basePath, std::map<ShaderType, std::string>& sources)
{
	ShaderProcessor preProcessor;

	// linked programs require at least a vertex and fragment shader. 
	bool hasFragmentShader = false;
	bool hasVertexShader = false;

	for (auto typeExt : extensions)
	{
		string filename = GetShaderFilename(basePath, typeExt.first);

		Log::Info << "Looking for shader file  \"" << filename << "\"" << endl;

		if (!Files::Exists(filename))
			continue;

		Log::Info << "Found shader file \"" << filename << "\"" << endl;

		string source = preProcessor.Parse(Files::Read(filename));

		sources[typeExt.first] = ShaderProcessor::Parse(Files::Read(filename));

		hasFragmentShader |= ShaderType::FragmentShader == typeExt.first;
		hasVertexShader |= ShaderType::VertexShader == typeExt.first;
	}

	if (!hasVertexShader)
	{
		Log::Error << "No vertex shader found!" << endl;
	}
	if (!hasFragmentShader)
	{
		Log::Error << "No fragment shader found!" << endl;
	}

	return hasVertexShader && hasFragmentShader;
}

/// check if the shader compiled successfully
bool Shader::CompileSuccessful(const GLint shader)
{
	GLint status;

	gl::GetShaderiv(shader, gl::COMPILE_STATUS, &status);
	check_gl_error();

	return status != (GLint)false;
}

/// check if the shader compiled successfully
bool Shader::LinkSuccessful(const GLint program)
{
	GLint status;

	gl::GetProgramiv(program, gl::LINK_STATUS, &status);

	check_gl_error();

	return status != (GLint)false;
}

string Shader::GetProgramLog(const GLuint program)
{
	string log = "";
	GLint logLength = 0;

	/// this is useless.
	GLint returnedLength;

	gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &logLength);
	check_gl_error();

	if (logLength > 0)
	{
		char* logBuffer = new char[logLength + 1];

		gl::GetProgramInfoLog(program, logLength, &returnedLength, logBuffer);
		check_gl_error();

		log = string(logBuffer);

		delete logBuffer;
	}

	return log;
}

string Shader::GetShaderLog(GLuint shader)
{
	string log = "";
	GLint logLength = 0;

	gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &logLength);
	check_gl_error();

	if (logLength > 0)
	{
		char *logBuffer = new char[logLength];

		/// this is useless when using std::string
		GLint returnedLength;

		gl::GetShaderInfoLog(shader, logLength, &returnedLength, logBuffer);

		check_gl_error();

		log = string(logBuffer);

		delete logBuffer;
	}

	return log;
}

void Shader::Bind()
{
	gl::UseProgram(Handle);
}

void Shader::Unbind()
{
	gl::UseProgram(0);
}

