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
#include "UniformCollection.h"


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

	bool IsInScreenSpace;

	bool OnInitialize() override;

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

	UniformCollection Uniforms;

	template<typename T>
	void SetUniform(const std::string& name, const T& value)
	{
		Uniforms.SetUniform(name, value);
	}

	GLint GetUniformLocation(const std::string& name) const
	{
		return Uniforms.GetUniformLocation(name);
	}

private:

private:
	GLuint m_program;

	bool CompileSuccessful(GLint program);
	void Preprocess(std::string& source);

	// programs that have already been built, by name.
	static std::map<std::string, GLint> m_programs;


};

#endif /* Material_hpp */
