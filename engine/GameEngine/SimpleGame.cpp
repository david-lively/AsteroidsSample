//
//  SimpleGame.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include <vector>
#include <iostream>

using namespace std;

#include "SimpleGame.h"
#include "Mesh.h"
#include "Material.h"
#include "Files.h"





bool SimpleGame::OnCreateScene()
{
	check_gl_error();

	auto& cube = Create<Mesh>("simple-mesh");

	check_gl_error();

	/// create a screen-aligned quad

	vector<float> coordinates =
	{
		-0.5f, +0.5f, 0.f
		,
		+0.5f, +0.5f, 0.f
		,
		+0.5f, -0.5f, 0.f
		,
		-0.5f, -0.5f, 0.f
	};

	vector<GLushort> indices =
	{
		0, 1, 2
		,
		2, 3, 0
	};

	cube.Initialize(coordinates, indices);
    
    
    Log::Info << "Working directory is " << Files::GetCurrentDirectory() << endl;
    
    string vertexShaderSource;
    string fragmentShaderSource;

    auto& material = Create<Material>("simple-material");
    auto success = true;
    do
    {

#ifdef _MSC_VER
		if(!LoadShaders("Shaders\\simple", vertexShaderSource, fragmentShaderSource))
		{
			Log::Error << "Could not load shader source. Exiting\n";
			return false;
		}
#else
		if (!LoadShaders("GameEngine/Shaders/simple", vertexShaderSource, fragmentShaderSource))
		{
			Log::Error << "Could not load shader source. Exiting\n";
			return false;
		}
#endif
        
        success = material.Build(vertexShaderSource, fragmentShaderSource);
        
        if(!success)
        {
            cout << "Press enter to retry." << endl;
            getchar();
        }
        
    } while(!success);
    
	cube.Material = &material;
    
    return true;
}

bool SimpleGame::LoadShaders(const string& baseFilename, string& vertexShaderSource, string& fragmentShaderSource)
{
    auto vertFilename = baseFilename + ".vert.glsl";
    auto fragFilename = baseFilename + ".frag.glsl";
    
    if(!Files::Exists(vertFilename))
    {
        Log::Error << "Could not find vertex shader \"" << vertFilename << "\"\n";
        return false;
    }
    
    if(!Files::Exists(fragFilename))
    {
        Log::Error << "Could not find fragment shader \"" << vertFilename << "\"\n";
        return false;
    }
    
    vertexShaderSource = Files::Read(vertFilename);
    fragmentShaderSource = Files::Read(fragFilename);
    
    return true;
    
}

