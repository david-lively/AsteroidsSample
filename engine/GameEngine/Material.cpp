//
//  Material.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include <string>
#include <map>
#include <memory>

#include "Material.h"
#include "Enums.h"
#include "Files.h"
#include "Game.h"
#include "Parser.h"
#include "Shader.h"

using namespace std;

std::map<std::string, Shader*> Material::m_programs;


//void Material::SetUniforms(const GameTime& time)
//{
//	Bind();
//
//	auto& environment = Game::Instance().Environment();
//	environment.Apply(*this, time);
//
//	check_gl_error();
//}

bool Material::Build(const std::string& path)
{
	if (m_programs.count(path) > 0)
	{
		Log::Info << "Shader " << path << " already created, reusing" << endl;
		Shader = *m_programs[path];
	}
	else
	{
		Shader.Build(path);
		m_programs[path] = &Shader;
	}

	return true;
}


void Material::Preprocess(std::string& source)
{
}

