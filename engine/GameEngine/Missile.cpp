//
//  Missile.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Missile.h"
#include "Common.h"
#include "Mesh.h"
#include "Material.h"
#include "GeometryProvider.h"
#include "Game.h"

#include <vector>
#include <cmath>

using namespace std;

bool Missile::OnInitialize()
{
	vector<Vector3> vertices;
	vector<GLushort> indices;

	Material.FillType = PolygonMode::Line;
	Mesh.Type = BeginMode::Lines;
	Mesh.CullBackfaces = false;

	
	Material.Build("Shaders/primitive");
	Material.Name = "Missile.primitive";

	vertices.push_back(Vector3(0, 0, 0));
	vertices.push_back(Vector3(0, +1.f, 0));

	indices.push_back(0);
	indices.push_back(1);
	
	Mesh.Bounds = BoundingSphere::FromVectors(vertices);
	Mesh.Initialize(vertices, indices);

	Transform.TranslationDrag = 0.f;

	Uniforms.SetUniform("EmissiveColorIntensity", Vector4(1, 1, 1, 1));


	return Drawable::OnInitialize();

}







