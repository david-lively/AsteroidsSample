//
//  Asteroid.cpp
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
#include "Material.h"

#include <vector>
#include <cmath>

using namespace std;

bool Asteroid::OnInitialize()
{
	vector<Vector3> vertices;
	vector<GLushort> indices;

	if (TwoD)
	{
		Material.FillType = PolygonMode::Line;
		Mesh.Type = BeginMode::Lines;
		Material.Build("Shaders/primitive");

		GeometryProvider::Circle(vertices, indices, Vector3::Zero, 0.5f, 12);

	}
	else
	{
		Material.FillType = PolygonMode::Fill;
		Mesh.Type = BeginMode::Triangles;
		Material.Build("Shaders/asteroid");
		GeometryProvider::Icosahedron(vertices, indices);
		GeometryProvider::Tessellate(vertices, indices, 2);
		GeometryProvider::Spherize(vertices);
	}


	GeometryProvider::Noisify(vertices, 20, 0.3f);
	GeometryProvider::FitToUnitCube(vertices);
	Bounds = BoundingSphere::FromVectors(vertices);


	Mesh.Material = &Material;
	Mesh.Initialize(vertices, indices);
	Mesh.CullBackfaces = false;

	Transform.TranslationDrag = 0.f;

	Uniforms.SetUniform("EmissiveColorIntensity", Vector4(1, 0, 0, 0.2f));

	float explodeSpeed = 0.02f;

	Input.Subscribe(GLFW_KEY_B,
		DECL_KEYHANDLER
	{
		Broken = !Broken;
	}
	);

	return Explodable::OnInitialize();
}

void Asteroid::OnUpdate(const GameTime& time)
{
	//auto world = Transform.GetMatrix();
	//auto view = Game::Camera().GetViewMatrix();

	//auto mvp = world * view;

	//Vector4 pos(0, 0, 0, 1);
	//Vector3 bounds = pos;
	//bounds.Y += 0.5f;

	//pos = mvp.Transform(pos);
	//bounds = mvp.Transform(bounds);
	//float radius = (bounds - pos).Length() / 2.f;

	Explodable::OnUpdate(time);
}

void Asteroid::OnRender(const GameTime& time)
{
	Material.Bind();

	Vector4 orange(1, 165.f / 255.f, 0, 0.125f);

	Uniforms.SetUniform("ColorByDepth", 1.f);
	Uniforms.SetUniform("EmissiveColorIntensity", orange);

	Explodable::OnRender(time);
}








