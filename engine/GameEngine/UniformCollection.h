#ifndef UNIFORMCOLLECTION_H
#define UNIFORMCOLLECTION_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "Common.h"
#include "Matrix.h"
#include "Vectors.h"


class Drawable;
// forward declaration
class Shader;

class UniformCollection
{
public:
	UniformCollection()
	{

	}

	~UniformCollection()
	{

	}

	void Initialize(Shader& shader);

	class Drawable* Drawable;

private:
	Shader* m_shader = nullptr;


#define UNIFORM_MAP_NAME(TYPENAME) m_uniformMap_ ## TYPENAME
#define DECL_UNIFORM_SETTER(TYPENAME)								\
	std::map<int, TYPENAME> UNIFORM_MAP_NAME(TYPENAME);				\
	void SetUniform(int location, const TYPENAME& value)			\
	{																	\
	UNIFORM_MAP_NAME(TYPENAME)[location] = value;					\
	}


	DECL_UNIFORM_SETTER(int);
	DECL_UNIFORM_SETTER(float);
	DECL_UNIFORM_SETTER(Matrix);
	DECL_UNIFORM_SETTER(Vector4);
	DECL_UNIFORM_SETTER(Vector3);
	DECL_UNIFORM_SETTER(Vector2);
	DECL_UNIFORM_SETTER(bool);


	void SetGlobalUniforms(const GameTime& time);
	void SetLocalUniforms(const GameTime& time);

	std::map<std::string, GLint> m_locations;
	std::map<GLint, std::string> m_locationNames;

	std::map<int, std::vector<Vector3>> m_uniformMap_vec3Arrays;

	void SetUniform(int location, const std::vector<Vector3>& vec3Array)
	{
		m_uniformMap_vec3Arrays[location] = vec3Array;
	}

public:
	GLint GetUniformLocation(const std::string& name) const;
	/*
	set a uniform shader value, if it exists

	We use this method to check if a uniform is available before actually trying to set its value.
	In some implementations, calling a gl::Uniform*() function with an invalid location parameter will
	generate an OpenGL error.
	*/
	template<typename T>
	void SetUniform(const std::string& name, const T& value)
	{
		if (m_locations.count(name) > 0)
		{
			auto location = m_locations[name];
			SetUniform(location, value);
		}

	}

	void Apply(const GameTime& time);


};

#endif
