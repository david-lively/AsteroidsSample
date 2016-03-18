//
//  Material.hpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <map>

#include "Common.h"
#include "GameObject.h"
#include "Matrix.h"
#include "Enums.h"
#include "WorldEntity.h"
#include "Light.h"


class Material : public GameObject
{
public:
	PolygonMode FillType = PolygonMode::Fill;

	bool Build(std::string vertexShaderSource, std::string fragmentShaderSource);

	bool Build(const std::string& path);

	void OnDispose() override;

	GLuint Program() const
	{
		return m_program;
	}


	/// <summary>
	/// Gets the shader information log.
	/// </summary>
	/// <param name="shader">The shader.</param>
	/// <returns></returns>
	std::string GetShaderLog(GLuint shader);
	std::string GetProgramLog(GLuint program);

	virtual void SetUniforms(const GameTime& time);

	void Bind()
	{
		gl::UseProgram(m_program);
	}

private:


#define UNIFORM_MAP_NAME(TYPENAME) m_uniformMap_ ## TYPENAME
#define DECL_UNIFORM_SETTER(TYPENAME)								\
	std::map<int, TYPENAME> UNIFORM_MAP_NAME(TYPENAME);				\
	void SetUniform(int location, const TYPENAME& value)			\
{																	\
	UNIFORM_MAP_NAME(TYPENAME)[location] = value;					\
}



#ifndef UNIFORM_SETTERS
#define UNIFORM_SETTERS

	DECL_UNIFORM_SETTER(int);
	DECL_UNIFORM_SETTER(float);
	DECL_UNIFORM_SETTER(Matrix);
	DECL_UNIFORM_SETTER(Vector4);
	DECL_UNIFORM_SETTER(Vector3);
	DECL_UNIFORM_SETTER(Vector2);

	std::map<int, const Light*> m_lights;

	void SetUniform(const Light& light);
	void SetAllUniforms(const GameTime& time);


#endif

#undef DECL_UNIFORM_SETTER

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
			auto location = (int)gl::GetUniformLocation(m_program, name.c_str());

			if (location >= 0)
				SetUniform(location, value);
		}

		void SetLights(const std::vector<Light*>& lights);


	private:
		GLuint m_program;

		bool CompileSuccessful(GLint program);
		void Preprocess(std::string& source);


	};

#endif /* Material_hpp */
