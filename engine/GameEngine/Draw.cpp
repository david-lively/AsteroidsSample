//
//  Draw.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Draw.h"
#include "Common.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GeometryProvider.h"
#include "Material.h"

#include <vector>
#include <cmath>

using namespace std;

bool Draw::OnInitialize()
{
	vector<Vector3> vertices;
	vector<GLushort> indices;

	Material.FillType = PolygonMode::Line;
	Mesh.Type = BeginMode::Lines;
	Material.Build("Draw");

	GeometryProvider::Circle(vertices, indices, Vector3::Zero, 0.5f, 12);


	Mesh.Material = &Material;
	Mesh.Initialize(vertices, indices);
	Mesh.CullBackfaces = false;

	Transform.TranslationDrag = 0.f;

	Uniforms.SetUniform("EmissiveColorIntensity", Vector4(1, 0, 0, 0.2f));

	float explodeSpeed = 0.02f;

	return Explodable::OnInitialize();
}

void Draw::OnRender(const GameTime& time)
{
	Material.Bind();

	Vector4 orange(1, 165.f / 255.f, 0, 0.125f);

	Uniforms.SetUniform("ColorByDepth", 1.f);
	Uniforms.SetUniform("EmissiveColorIntensity", orange);

	Explodable::OnRender(time);
}







