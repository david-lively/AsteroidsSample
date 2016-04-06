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

	auto& material = this->Material;
	auto& mesh = this->Mesh;

	if (TwoD)
	{
		material.FillType = PolygonMode::Line;
		mesh.Type = BeginMode::Lines;
		material.Build("Shaders/primitive");

		GeometryProvider::Circle(vertices, indices, Vector3::Zero, 0.5f, 12);

	}
	else
	{
		material.FillType = PolygonMode::Fill;
		mesh.Type = BeginMode::Triangles;
		material.Build("Shaders/asteroid");
		GeometryProvider::Icosahedron(vertices, indices);
		GeometryProvider::Tessellate(vertices, indices, 4);
		GeometryProvider::Spherize(vertices);
	}
	

	GeometryProvider::Noisify(vertices, 4, 0.3f);
	GeometryProvider::FitToUnitCube(vertices);
	Bounds = BoundingSphere::FromVectors(vertices);


	mesh.Material = &material;
	mesh.Initialize(vertices, indices);

	Transform.TranslationDrag = 0.f;

	Uniforms.SetUniform("EmissiveColorIntensity", Vector4(1, 0, 0, 0.2f));

	float explodeSpeed = 0.01f;

	Input.Subscribe(GLFW_KEY_F10,
		DECL_KEYHANDLER
	{
		ExplosionFactor += explodeSpeed;
	}
	);

	Input.Subscribe(GLFW_KEY_F9,
		DECL_KEYHANDLER
	{
		ExplosionFactor = max(ExplosionFactor - explodeSpeed, 0);
	}
	);


	return Explodable::OnInitialize();
}

void Asteroid::OnPreRender(const GameTime& time)
{
	Uniforms.SetUniform("ColorByDepth", 1.f);
	Uniforms.SetUniform("ExplosionFactor", ExplosionFactor);
	Uniforms.SetUniform("IsExploding", true);

	Explodable::OnPreRender(time);
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

	Uniforms.SetUniform("EmissiveColorIntensity", orange);


	Explodable::OnRender(time);
}




