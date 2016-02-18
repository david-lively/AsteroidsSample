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
#include "GeometryProvider.h"





bool SimpleGame::OnCreateScene()
{
	check_gl_error();

	auto& cube = Create<Mesh>("simple-mesh");

	check_gl_error();

	/// create a screen-aligned quad
    
    vector<Vector3> vertices;
    vector<GLushort> indices;
    
    GeometryProvider::Cube(vertices, indices);
    
	cube.Initialize(vertices, indices);
    
    
    Log::Info << "Working directory is " << Files::GetCurrentDirectory() << endl;
    
    string vertexShaderSource;
    string fragmentShaderSource;

    auto& material = Create<Material>("simple-material");
    auto success = true;
    do
    {

		if(!LoadShaders("simple", vertexShaderSource, fragmentShaderSource))
		{
			Log::Error << "Could not load shader source. Exiting\n";
			return false;
		}

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
    auto vertFilename = ShaderFolder + baseFilename + ".vert.glsl";
    auto fragFilename = ShaderFolder + baseFilename + ".frag.glsl";
    
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
    
    Log::Info << "Loading vertex shader \"" << vertFilename << "\"\n";
    vertexShaderSource = Files::Read(vertFilename);

    Log::Info << "Loading fragment shader \"" << fragFilename << "\"\n";
    fragmentShaderSource = Files::Read(fragFilename);
    
    return true;
    
}

