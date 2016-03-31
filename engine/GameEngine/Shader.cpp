#include <string>
#include <map>

using namespace std;


#include "Shader.h"
#include "Common.h"
#include "Files.h"
#include "Enums.h"


Shader::Shader() : Shader("unknown.shader")
{

}

Shader::Shader(const string& name) : Resource(name)
{

}

Shader::~Shader()
{
}

GLuint Shader::Build(const string& basePath)
{
	check_gl_error();

	map<ShaderType, string> sources;
	bool success = false;

	Log::Info << "Building shaders for \"" << basePath << "\"" << endl;

	GLuint programId = 0;


	map<ShaderType, GLuint> shaders;

	do
	{
		sources.clear();

		if (!(success = LoadSources(basePath, sources)))
		{
			Log::Error << "Could not load required sources for shader \"" << basePath << "\"" << endl;
		}

		if (success)
		{
			for (auto& typeSource : sources)
			{
				Log::Info << "Compiling shader " << to_string(typeSource.first) << "..." << endl;
				ShaderType shaderType = typeSource.first;
				string source = typeSource.second;

				GLuint shader;

				if (shaders.count(shaderType) > 0)
					shader = shaders[shaderType];
				else
					shader = gl::CreateShader((GLenum)shaderType);

				check_gl_error();

				GLchar* sourceStr = (GLchar*)source.c_str();
				GLint sourceLength = (GLint)source.length();

				gl::ShaderSource(shader, 1, &sourceStr, &sourceLength);

				check_gl_error();

				gl::CompileShader(shader);

				if (!CompileSuccessful(shader))
				{
					Log::Error << "Shader compilation error for " << to_string(shaderType) << " " << GetShaderFilename(basePath, shaderType) << endl;
					Log::Error << GetShaderLog(shader) << endl;
					success = false;
				}
				else
					shaders[shaderType] = shader;
			}
		}

		if (!success)
		{
			cout << "Press enter to retry." << endl;
			getchar();
		}


	} while (!success);

	check_gl_error();

	/// link the program
	Handle = gl::CreateProgram();

	for (auto shader : shaders)
	{
		gl::AttachShader(Handle, shader.second);
		check_gl_error();
	}

	gl::LinkProgram(Handle);

	if (!LinkSuccessful(Handle))
	{
		Log::Error << "Link error for shader \"" << basePath << "\"" << endl << GetProgramLog(Handle) << endl;

		throw;
	}

	for (auto shader : shaders)
	{
		gl::DeleteShader(shader.second);
	}

	return Handle;
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


string Shader::GetShaderFilename(const std::string& basePath, const ShaderType shaderType) const
{
	return basePath + "." + extensions[shaderType] + ".glsl";
}



bool Shader::LoadSources(const std::string& basePath, std::map<ShaderType, std::string>& sources)
{
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

		sources[typeExt.first] = Files::Read(filename);

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
