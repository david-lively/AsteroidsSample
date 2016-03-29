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
#include "Shader.h"


class Material : public GameObject
{
public:
	PolygonMode FillType = PolygonMode::Fill;

	bool Build(const std::string& path);

	GLuint Program() const
	{
		return m_shader->Handle;
	}

	bool IsInScreenSpace;

	bool OnInitialize() override;

	virtual void SetUniforms(const GameTime& time);

	void Bind()
	{
		m_shader->Bind();
	}

	void Unbind()
	{
		m_shader->Unbind();
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

	GLint GetAttribLocation(const std::string& name) const
	{
		return m_shader->GetAttribLocation(name);
	}

private:
	Shader* m_shader = nullptr;

	void Preprocess(std::string& source);

	// programs that have already been built, by name.
	static std::map<std::string, Shader*> m_programs;


};

#endif /* Material_hpp */
