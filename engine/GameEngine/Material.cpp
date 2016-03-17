//
//  Material.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Material.h"
#include "Enums.h"
#include "Files.h"
#include "Game.h"

#include <string>
using namespace std;



void Material::SetUniforms(const GameTime& time)
{
	Bind();
	SetUniform("GameTimeTotalSeconds", time.TotalSeconds());

	auto& cam = Game::Camera();

	SetUniform("View", cam.GetViewMatrix());
	SetUniform("Projection", cam.GetProjectionMatrix());

	SetAllUniforms(time);
}

void Material::SetAllUniforms(const GameTime& time)
{
#define UNIFORM_SETTER_BEGIN(t) \
	{ if (UNIFORM_MAP_NAME(t).size() > 0)								\
	{																\
	auto& uniformMap = UNIFORM_MAP_NAME(t);							\
	for (auto u : uniformMap)									\
	{																\

#define UNIFORM_SETTER_END	}}}

	UNIFORM_SETTER_BEGIN(int)
		gl::Uniform1i(u.first, u.second);
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(float)
		gl::Uniform1f(u.first, u.second);
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(Vector3)
		gl::Uniform3f(u.first, u.second.X, u.second.Y, u.second.Z);
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(Vector4)
		gl::Uniform4f(u.first, u.second.X, u.second.Y, u.second.Z, u.second.W);
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(Matrix)
		gl::UniformMatrix4fv(u.first, 1, false, &u.second.m00);
	UNIFORM_SETTER_END;

	SetAllUniformLights(time);


}

void Material::SetUniform(const Light& light)
{
	m_lights[light.Id] = &light;
}

void Material::SetLights(const std::vector<Light*>& lights)
{
	m_lights.clear();

	for (auto lightPtr : lights)
	{
		m_lights[lightPtr->Id] = lightPtr;
	}

}



void Material::SetAllUniformLights(const GameTime& time)
{
	auto countIndex = gl::GetUniformLocation(m_program, "LightCount");
	if (countIndex < 0)
		return;


	std::vector<Vector4> lightColorIntensities;
	lightColorIntensities.resize(m_lights.size());

	std::vector<Vector3> lightDirections;
	lightDirections.reserve(m_lights.size());

	for (auto lightPtr : m_lights)
	{
		auto light = *lightPtr.second;
		if (light.Enabled)
		{
			Vector4 colorIntensity;

			colorIntensity.X = light.Color.X;
			colorIntensity.Y = light.Color.Y;
			colorIntensity.Z = light.Color.Z;
			colorIntensity.W = light.Intensity;

			lightColorIntensities.push_back(colorIntensity);
			lightDirections.push_back(light.Direction);
		}

	}

	auto lightCount = min(lightColorIntensities.size(), lightDirections.size());
	gl::Uniform1i(countIndex, lightCount);

	auto location = gl::GetUniformLocation(m_program, "LightColorIntensity");
	if (location >= 0)
		gl::Uniform4fv(location, lightCount, (const GLfloat*)lightColorIntensities.data());

	location = gl::GetUniformLocation(m_program, "LightDirection");
	if (location >= 0)
		gl::Uniform3fv(location, lightCount, (const GLfloat*)lightDirections.data());

}




bool Material::Build(string vertexShaderSource, string fragmentShaderSource)
{
	check_gl_error();

	GLuint vertexShader = gl::CreateShader((GLenum)ShaderType::VertexShader);
	GLint vertSourceLength = (GLint)vertexShaderSource.length();
	GLchar* vertSourceStr = (GLchar*)vertexShaderSource.c_str();

	gl::ShaderSource(vertexShader, 1, &vertSourceStr, &vertSourceLength);

	gl::CompileShader(vertexShader);

	if (!CompileSuccessful(vertexShader))
	{
		auto vertexShaderlog = GetShaderLog(vertexShader);
		Log::Error << "Vertex Shader compilation error\n" << vertexShaderlog << endl;
		return false;
	}

	check_gl_error();

	GLuint fragmentShader = gl::CreateShader((GLenum)ShaderType::FragmentShader);
	GLint fragSourceLength = (GLint)fragmentShaderSource.length();
	GLchar* fragmentSourceStr = (GLchar*)fragmentShaderSource.c_str();

	gl::ShaderSource(fragmentShader, 1, &fragmentSourceStr, &fragSourceLength);

	gl::CompileShader(fragmentShader);
	check_gl_error();

	if (!CompileSuccessful(fragmentShader))
	{
		auto fragmentShaderLog = GetShaderLog(fragmentShader);
		Log::Error << "Fragment Shader compilation error\n" << fragmentShaderLog << endl;
		return false;
	}


	/// link the shaders into a program
	/// at minimum, a program MUST have a vertex shader and a fragment shader
	m_program = gl::CreateProgram();

	gl::AttachShader(m_program, vertexShader);
	gl::AttachShader(m_program, fragmentShader);

	check_gl_error();

	gl::LinkProgram(m_program);

	check_gl_error();

	Log::Info << "Successfully built program.\n";

	return true;

}

void Material::OnDispose()
{
	if (m_program > 0)
	{
		gl::UseProgram(0);
		gl::DeleteProgram(m_program);

		m_program = 0;
	}

}

string Material::GetShaderLog(GLuint shader)
{
	string log = "";
	GLint logLength = 0;

	gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &logLength);

	if (logLength > 0)
	{
		char *logBuffer = new char[logLength];

		/// this is useless when using std::string
		GLint returnedLength;

		gl::GetShaderInfoLog(shader, logLength, &returnedLength, logBuffer);
		log = string(logBuffer);

		delete logBuffer;
	}

	return log;
}

/// <summary>
/// Gets the program information log.
/// </summary>
/// <param name="program">The program.</param>
/// <returns></returns>
string Material::GetProgramLog(GLuint program)
{
	string log = "";
	GLint logLength = 0;

	/// this is useless.
	GLint returnedLength;

	gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &logLength);

	if (logLength > 0)
	{
		char* logBuffer = new char[logLength + 1];

		gl::GetProgramInfoLog(program, logLength, &returnedLength, logBuffer);

		log = string(logBuffer);

		delete logBuffer;
	}

	return log;
}

/// check if the shader compiled successfully
bool Material::CompileSuccessful(GLint program)
{
	GLint status;

	gl::GetShaderiv(program, gl::COMPILE_STATUS, &status);

	return status != (GLint)false;
}


bool Material::Build(const std::string& path)
{
	auto vertFilename = path + ".vert.glsl";
	auto fragFilename = path + ".frag.glsl";

	if (!Files::Exists(vertFilename))
	{
		Log::Error << "Could not find vertex shader \"" << vertFilename << "\"\n";
		return false;
	}

	if (!Files::Exists(fragFilename))
	{
		Log::Error << "Could not find fragment shader \"" << fragFilename << "\"\n";
		return false;
	}


	bool success = true;

	do
	{

		Log::Info << "Loading vertex shader \"" << vertFilename << "\"\n";
		auto vertexShaderSource = Files::Read(vertFilename);

		Log::Info << "Loading fragment shader \"" << fragFilename << "\"\n";
		auto fragmentShaderSource = Files::Read(fragFilename);

		success = Build(vertexShaderSource, fragmentShaderSource);

		if (!success)
		{
			cout << "Press enter to retry." << endl;
			getchar();
		}

	} while (!success);

	return true;

}
