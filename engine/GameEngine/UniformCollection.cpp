#include "UniformCollection.h"
#include "Common.h"
#include "Game.h"
#include "Material.h"

#include <map>
#include <vector>
#include <string>

using namespace std;


void UniformCollection::Apply(const GameTime& time)
{
	SetGlobalUniforms(time);
	SetLocalUniforms(time);
}


GLint UniformCollection::GetUniformLocation(const std::string& name) const
{
	return m_shader->GetUniformLocation(name);
}

void UniformCollection::SetGlobalUniforms(const GameTime& time)
{
	auto& cam = Game::Camera();

	SetUniform("GameTimeTotalSeconds", time.TotalSeconds());
	SetUniform("View", cam.GetViewMatrix());
	SetUniform("Projection", cam.GetProjectionMatrix());
}

void UniformCollection::SetLocalUniforms(const GameTime& time)
{
#define UNIFORM_SETTER_BEGIN(t)						\
	{ if (UNIFORM_MAP_NAME(t).size() > 0)			\
	{												\
	auto& uniformMap = UNIFORM_MAP_NAME(t);			\
	for (auto u : uniformMap)						\
	{												\

#define UNIFORM_SETTER_END	}}}

	UNIFORM_SETTER_BEGIN(int)
		gl::Uniform1i(u.first, u.second);
	check_gl_error();
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(float)
		gl::Uniform1f(u.first, u.second);
	check_gl_error();
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(Vector2)
		gl::Uniform2f(u.first, u.second.X, u.second.Y);
	check_gl_error();
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(Vector3)
		gl::Uniform3f(u.first, u.second.X, u.second.Y, u.second.Z);
	check_gl_error();
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(Vector4)
		gl::Uniform4f(u.first, u.second.X, u.second.Y, u.second.Z, u.second.W);
	check_gl_error();
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(Matrix)
		gl::UniformMatrix4fv(u.first, 1, false, &u.second.m00);
	check_gl_error();
	UNIFORM_SETTER_END;

	UNIFORM_SETTER_BEGIN(bool)
		gl::Uniform1f(u.first, u.second);
	check_gl_error();
	UNIFORM_SETTER_END;

	// vec3 arrays (vector<Vector3>, etc.)
	{
		if (m_uniformMap_Matrix.size() > 0)
		{
			auto& uniformMap = m_uniformMap_vec3Arrays;
			for (auto u : uniformMap)
			{
				gl::Uniform3fv(u.first, u.second.size(), (const GLfloat*)u.second.data());
			}
			check_gl_error();
		}
	}

	check_gl_error();

}


/// previously created uniform collections.
map<std::string, UniformCollection*> shaderUniformCollections;

void UniformCollection::Initialize(Shader& shader)
{
	m_shader = &shader;

	if (shaderUniformCollections.count(shader.Name) > 0)
	{
		Log::Info << "Found existing uniform map for shader " << shader.Name << ". Copying. " << endl;
		auto& other = *shaderUniformCollections[shader.Name];

		m_locations = other.m_locations;
		m_locationNames = other.m_locationNames;

		return;
	}

	shaderUniformCollections[shader.Name] = this;

	auto program = m_shader->Handle;

	m_locations.clear();

	Log::Debug << "Initializing uniform map for shader " << m_shader->Name << " ( " << program << ") ..." << endl;

	GLint prevProgram;
	gl::GetIntegerv(gl::CURRENT_PROGRAM, &prevProgram);

	m_shader->Bind();

	check_gl_error();

	GLint maxNameLength;

	gl::GetProgramiv(program, gl::ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

	check_gl_error();

	maxNameLength = 1024 + 3 + 1; // max GLSL identifier length, plus 3 for possible array subscript "[0]", plus 1 for null terminator.


	char* nameBuffer = new char[maxNameLength];

	GLint uniformCount = 0;
	gl::GetProgramiv(program, gl::ACTIVE_UNIFORMS, &uniformCount);

	check_gl_error();

	for (int i = 0; i < uniformCount; ++i)
	{
		GLenum type;
		GLsizei nameLength = 0;

		gl::GetActiveUniformName(program, i, maxNameLength, &nameLength, nameBuffer);
		//gl::GetActiveUniform(program, i, maxNameLength, &nameLength, nullptr, &type, nameBuffer);

		check_gl_error();

		if (0 == nameLength)
			continue;

		GLint location = gl::GetUniformLocation(program, nameBuffer);

		check_gl_error();

		if (location < 0)
			continue;

		//m_locations[string(nameBuffer)] = location;
		if (m_locationNames.count(location) > 0)
		{
			Log::Error << "Invalid state - multiple uniforms at same location!";
			DEBUG_BREAK;
		}


		Log::Info << "Found shader uniform " << location << " \"" << nameBuffer << "\"" << endl;

		/// strip array subscript from names that have them (light arrays, etc)
		string nameStr = string(nameBuffer);

		auto j = nameStr.find('[');
		
		if (j != string::npos)
		{
			nameStr = nameStr.substr(0, j);
		}

		m_locations[nameStr] = location;
		m_locationNames[location] = nameStr;
	}

	if (nullptr != nameBuffer)
		delete nameBuffer;

	gl::UseProgram(prevProgram);


	Log::Info << "Uniform map initialization DONE" << endl;

	check_gl_error();


}




