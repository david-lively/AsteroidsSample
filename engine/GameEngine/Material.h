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
#include <memory>

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
	bool IsInScreenSpace;
	Shader& Shader;
	bool Blend = true;
	bool DepthTesting = true;

	Material(const std::string& name) : GameObject(name), Shader(Create<class Shader>(name + ".shader"))
	{

	}

	Material() : Material("material")
	{

	}

	Material(Material& toCopy) :
		FillType(toCopy.FillType)
		, IsInScreenSpace(toCopy.IsInScreenSpace)
		, Shader(toCopy.Shader)
	{
	}

	bool Build(const std::string& path);

	GLuint Program() const
	{
		return Shader.Handle;
	}

	void Bind()
	{
		Shader.Bind();
	}

	void Unbind()
	{
		Shader.Unbind();
	}

	GLint GetAttribLocation(const std::string& name) const
	{
		return Shader.GetAttribLocation(name);
	}

private:
	void Preprocess(std::string& source);

	// programs that have already been built, by name.
	static std::map<std::string, class Shader*> m_programs;
};

#endif /* Material_hpp */
