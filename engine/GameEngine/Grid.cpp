//
//  Ship.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Common.h"
#include "Grid.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GeometryProvider.h"

#include <vector>
#include <cmath>

using namespace std;

bool Grid::OnInitialize()
{
    auto& material = Create<class Material>("ship-material");
    m_material = &material;
    
    material.FillType = PolygonMode::Line;
    
    auto& mesh = Create<Mesh>("ship-mesh");

    vector<Vector3> vertices;
    
    GeometryProvider::LineGrid(vertices,20);
    
    Bounds = BoundingBox::FromVectors(vertices);
    
    material.Build("Shaders/primitive");
    
    mesh.Material = &material;
    
    mesh.Initialize(vertices);
    mesh.Type = BeginMode::Lines;

    m_mesh = &mesh;
    
    return true;
    
}

void Grid::OnRender(const GameTime& time)
{
    auto& cam = Game::Camera();
    
    m_material->Bind();
    m_material->SetUniform("World", Transform->GetMatrix());
    m_material->SetUniform("View",cam.GetViewMatrix());
    m_material->SetUniform("Projection",cam.GetProjectionMatrix());
}
