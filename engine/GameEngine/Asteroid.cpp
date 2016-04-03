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
#include "SimplexNoise.h"
#include "Material.h"

#include <vector>
#include <cmath>

using namespace std;

bool Asteroid::OnInitialize()
{
	auto& material = Create<class Material>("asteroid-material");
	m_material = &material;

	auto& mesh = Create<Mesh>("asteroid-mesh");

	vector<Vector3> vertices;
	vector<GLushort> indices;


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
		GeometryProvider::Tessellate(vertices, indices, 3);
		GeometryProvider::Spherize(vertices);
	}
	

	GeometryProvider::Noisify(vertices, 4, 0.3f);
	GeometryProvider::FitToUnitCube(vertices);
	Bounds = BoundingSphere::FromVectors(vertices);


	mesh.Material = &material;
	mesh.Initialize(vertices, indices);

	m_mesh = &mesh;

	Transform->TranslationDrag = 0.f;

	material.SetUniform("EmissiveColorIntensity", Vector4(1, 0, 0, 0.2f));

	auto& handler = Create<InputHandler>("asteroid-input");

	float explodeSpeed = 0.01f;

	handler.Subscribe(GLFW_KEY_F10,
		DECL_KEYHANDLER
	{
		ExplosionFactor += explodeSpeed;
	}
	);

	handler.Subscribe(GLFW_KEY_F9,
		DECL_KEYHANDLER
	{
		ExplosionFactor = max(ExplosionFactor - explodeSpeed, 0);
	}
	);


	return WorldEntity::OnInitialize();
}

void Asteroid::OnPreRender(const GameTime& time)
{
	m_material->Bind();
	
	m_material->SetUniform("ColorByDepth", 1.f);
	m_material->SetUniform("ExplosionFactor", ExplosionFactor);
	m_material->SetUniform("IsExploding", true);

	WorldEntity::OnPreRender(time);
}


void Asteroid::OnUpdate(const GameTime& time)
{
	auto world = Transform->GetMatrix();
	auto view = Game::Camera().GetViewMatrix();

	auto mvp = world * view;

	Vector4 pos(0, 0, 0, 1);
	Vector3 bounds = pos;
	bounds.Y += 0.5f;

	pos = mvp.Transform(pos);
	bounds = mvp.Transform(bounds);
	float radius = (bounds - pos).Length() / 2.f;

	WorldEntity::OnUpdate(time);
}

void Asteroid::OnRender(const GameTime& time)
{
	m_material->Bind();

	Vector4 orange(1, 165.f / 255.f, 0, 0.125f);

	m_material->SetUniform("EmissiveColorIntensity", orange);

	WorldEntity::OnRender(time);


}




