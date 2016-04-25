//
//  Asteroid.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//
#include <vector>
#include <cmath>

#include "Asteroid.h"
#include "Common.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GeometryProvider.h"
#include "Material.h"
#include "Mesh.h"
#include "ModelStore.h"

using namespace std;

// stupid hack around stupid architecture

bool Asteroid::OnInitialize()
{
	vector<Vector3> vertices;
	vector<GLushort> indices;

	string modelName = "asteroid";
	auto& models = Game::Instance().Models;

	Material.FillType = PolygonMode::Fill;
	Material.Build("Shaders/asteroid");

	if (models.Exists(modelName))
	{
		this->Mesh = *models.Get(modelName);
		this->Mesh.Parent = this;
		this->Mesh.Material = &Material;
	}
	else
	{
		Log::Info << "Found mesh \"" << modelName << "\". Reusing." << endl;

		GeometryProvider::Icosahedron(vertices, indices);
		GeometryProvider::Tessellate(vertices, indices, 2);
		GeometryProvider::Spherize(vertices);
		GeometryProvider::Noisify(vertices, 20, 0.3f);
		GeometryProvider::FitToUnitCube(vertices);

		Mesh.Bounds = BoundingSphere::FromVectors(vertices);
		Mesh.Type = BeginMode::Triangles;
		Mesh.Material = &Material;
		Mesh.Initialize(vertices, indices);
		Mesh.CullBackfaces = true;

		models.Add(modelName, &Mesh);


	}

	Transform.TranslationDrag = 0.f;

	Uniforms.SetUniform("EmissiveColorIntensity", Vector4(1, 0, 0, 0.2f));

	float explodeSpeed = 0.02f;

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

	/*
	uniform vec3 ExplosionColorInner;
uniform vec3 ExplosionColorOuter;
uniform float ExplosionColorIntensity;

	*/

	Vector3 eci = Vector3(1, 1, 1);
	Vector3 eco = Vector3(1, 0, 1);
	float intensity = 1;

	Uniforms.SetUniform("ExplosionColorInner", eci);
	Uniforms.SetUniform("ExplosionColorOuter", eco);
	Uniforms.SetUniform("ExplosionColorIntensity", intensity);


	Explodable::OnRender(time);
}








