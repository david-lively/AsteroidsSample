//
//  Ship.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Common.h"
#include "Grid.h"
#include "GeometryProvider.h"

#include <vector>
#include <cmath>

using namespace std;

bool Grid::OnInitialize()
{
    Material.FillType = PolygonMode::Line;
    
    vector<Vector3> vertices;
    GeometryProvider::LineGrid(vertices,20);

	Mesh.Bounds = BoundingSphere::FromVectors(vertices);
    
    Material.Build("Shaders/primitive");
    
    Mesh.Material = &Material;
    Mesh.Initialize(vertices);
    Mesh.Type = BeginMode::Lines;

    return true;
    
}

void Grid::OnUpdate(const GameTime& time)
{
	WorldEntity::OnUpdate(time);
}


