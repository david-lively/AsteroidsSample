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
    
    vector<float> vertices =
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
    
    
    vector<GLushort> indices =
    {
        0, 1
        ,
        0, 2
        ,
        3, 4
    };
    
    Bounds = BoundingBox::FromFloats(vertices);
    
    material.Build("Shaders/primitive");
    
    mesh.Material = &material;

    mesh.Initialize(vertices, indices);
    mesh.Type = BeginMode::Lines;
    m_mesh = &mesh;
    
    return true;
    
}

void Ship::OnUpdate(const GameTime& time)
{
    
    
}


void Ship::OnRender(const GameTime& time)
{
	auto& cam = Game::Camera();

	cam.Transform->Translation.Z = 7;

	WorldEntity::OnRender(time);
}
    
 



void Ship::ConfigureInput()
{
    auto& handler = Create<InputHandler>("ship.input.handler");
    
    const float forwardSpeed = 0.09f;
    const float spinSpeed = 0.01f;
    
#define xform (*this->Transform)
    handler.Subscribe(GLFW_KEY_UP,
                      DECL_KEYHANDLER
                      {
                          auto dir = Transform->GetMatrix().Up() * forwardSpeed;
						  xform.Push(dir * forwardSpeed);
                      }
                      );
    
    handler.Subscribe(GLFW_KEY_DOWN,
                      DECL_KEYHANDLER
                      {
                          auto dir = Transform->GetMatrix().Up() * (-1.f * forwardSpeed);
						  xform.Push(dir * forwardSpeed);
                      }
                      );
    
    
    handler.Subscribe(GLFW_KEY_RIGHT,
                      DECL_KEYHANDLER
                      {
                          auto spin = Vector3(0,0,spinSpeed);
                          xform.Spin(spin);
                      }
                      
                      );
    
    handler.Subscribe(GLFW_KEY_LEFT,
                      DECL_KEYHANDLER
                      {
                          auto spin = Vector3(0,0,-1.f * spinSpeed);
                          xform.Spin(spin);
                      }
                      
                      );

#undef xform
    
}


