//
//  Ship.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Asteroid.h"
#include "Common.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GeometryProvider.h"

#include <vector>
#include <cmath>

using namespace std;

bool Asteroid::OnInitialize()
{
    auto& material = Create<class Material>("ship-material");
    m_material = &material;
    
    material.FillType = PolygonMode::Fill;
    
    auto& mesh = Create<Mesh>("asteroid-mesh");
    
    vector<Vector3> vertices;
    vector<GLushort> indices;
    
    GeometryProvider::Icosahedron(vertices, indices);

	GeometryProvider::Tessellate(vertices, indices, 3);

	GeometryProvider::Spherize(vertices);

	Bounds = BoundingBox::FromVectors(vertices);
    
    material.Build("Shaders/lit");
    
    mesh.Material = &material;
    
    mesh.Initialize(vertices, indices);
    mesh.Type = BeginMode::Triangles;

    m_mesh = &mesh;
    
    Transform->Drag = 0.f;
    
    return true;
    
}

void Asteroid::OnUpdate(const GameTime& time)
{
    auto world = Transform->GetMatrix();
    auto view = Game::Camera().GetViewMatrix();
    
    auto mvp = world * view;
    
    Vector4 pos(0,0,0,1);
    Vector3 bounds = pos;
    bounds.Y += 0.5f;
    
    pos = mvp.Transform(pos);
    bounds = mvp.Transform(bounds);
    float radius = (bounds - pos).Length() / 2.f;
    
}



