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
#include "Parser.h"
#include "Shader.h"
#include <string>
#include <map>

using namespace std;

std::map<std::string, Shader*> Material::m_programs;


void Material::SetUniforms(const GameTime& time)
{
	Bind();

	check_gl_error();
	auto& environment = Game::Instance().Environment();

	SetUniform("World", environment.CurrentMatrix());

	environment.Apply(*this, time);


	check_gl_error();

	Uniforms.Apply(time);

	check_gl_error();
}

bool Material::OnInitialize()
{
	Uniforms.Initialize(*m_shader);
	return true;
}


bool Material::Build(const std::string& path)
{
	if (nullptr != m_shader)
	{
		Log::Error << "Material " << Name << " has already been built. Dispose and rebuild." << endl;
		return false;
	}

	if (m_programs.count(path) > 0)
	{
		Log::Info << "Shader " << path << " already created, reusing" << endl;
		m_shader = m_programs[path];
	}
	else
	{
		m_shader = &Create<Shader>(Name);
		m_shader->Build(path);

		m_programs[path] = m_shader;
	}

	return true;
}


void Material::Preprocess(std::string& source)
{
	Parser parser;



}

