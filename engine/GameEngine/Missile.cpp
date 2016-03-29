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
	auto& material = Create<class Material>("Missile-material");
	m_material = &material;

	auto& mesh = Create<Mesh>("Missile-mesh");

	vector<Vector3> vertices;
	vector<GLushort> indices;

	material.FillType = PolygonMode::Line;
	mesh.Type = BeginMode::Lines;
	
	material.Build("Shaders/primitive");

	vertices.push_back(Vector3(0, 0, 0));
	vertices.push_back(Vector3(0, +1.f, 0));

	indices.push_back(0);
	indices.push_back(1);

	Bounds = BoundingBox::FromVectors(vertices);
	Sphere = BoundingSphere::FromVectors(vertices);
	mesh.Material = &material;
	mesh.Initialize(vertices, indices);


	m_mesh = &mesh;

	Transform->Drag = 0.f;

	material.SetUniform("EmissiveColorIntensity", Vector4(1, 0, 0, 0.2f));
	return WorldEntity::OnInitialize();

}







