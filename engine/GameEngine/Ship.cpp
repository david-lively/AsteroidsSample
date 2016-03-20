//
//  Ship.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Common.h"
#include "Ship.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GeometryProvider.h"

#include <vector>
#include <cmath>

using namespace std;

bool Ship::OnInitialize()
{
    ConfigureInput();
    
    auto& material = Create<class Material>("ship-material");
    m_material = &material;
    
    material.FillType = PolygonMode::Line;
    
    auto& mesh = Create<Mesh>("ship-mesh");
    
    vector<float> coordinates =
    {
        0,0.5f, 0
        ,
        1/3.f, -0.5f, 0
        ,
        -1/3.f, -0.5f, 0
        ,
        -1/4.f, -0.25f, 0
        ,
        +1/4.f, -0.25f, 0
        
    };

	auto vertices = GeometryProvider::ToVectors(coordinates);

    vector<GLushort> indices =
    {
        0, 1
        ,
        0, 2
        ,
        3, 4
    };
    
	//GeometryProvider::Circle(vertices, indices, Vector3(0), 0.6f, 12);

    Bounds = BoundingBox::FromVectors(vertices);
    
    material.Build("Shaders/primitive");
    
    mesh.Material = &material;

    mesh.Initialize(vertices, indices);
    mesh.Type = BeginMode::Lines;
    m_mesh = &mesh;
    
    return true;
    
}

void Ship::OnUpdate(const GameTime& time)
{
	WorldEntity::OnUpdate(time);
    
    
}


void Ship::OnRender(const GameTime& time)
{

	WorldEntity::OnRender(time);
}
    
 



void Ship::ConfigureInput()
{
    auto& handler = Create<InputHandler>("ship.input.handler");
    
    const float forwardSpeed = 0.5f;
    const float spinSpeed = 0.3f;
    
#define xform (*this->Transform)
    handler.Subscribe(GLFW_KEY_UP,
                      DECL_KEYHANDLER
                      {
                          auto dir = Transform->GetMatrix().Up() * forwardSpeed;
						  xform.Push(dir * forwardSpeed * time.ElapsedSeconds());
                      }
                      );
    
    handler.Subscribe(GLFW_KEY_DOWN,
                      DECL_KEYHANDLER
                      {
                          auto dir = Transform->GetMatrix().Up() * (-1.f * forwardSpeed);
						  xform.Push(dir * forwardSpeed * time.ElapsedSeconds());
                      }
                      );
    
    
    handler.Subscribe(GLFW_KEY_RIGHT,
                      DECL_KEYHANDLER
                      {
                          auto spin = Vector3(0,0,spinSpeed);
						  xform.Spin(spin * time.ElapsedSeconds());
                      }
                      
                      );
    
    handler.Subscribe(GLFW_KEY_LEFT,
                      DECL_KEYHANDLER
                      {
                          auto spin = Vector3(0,0,-1.f * spinSpeed);
						  xform.Spin(spin * time.ElapsedSeconds());
                      }
                      
                      );

#undef xform
    
}


