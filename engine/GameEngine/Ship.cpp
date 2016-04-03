#define CONE_SHIP
//#define SPHERE_SHIP
//
//  Ship.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Common.h"
#include "Ship.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GeometryProvider.h"
#include "AxisHelper.h"
#include "WorldEntity.h"

#include <vector>
#include <cmath>

using namespace std;

bool Ship::OnInitialize()
{
	Transform->TranslationDrag = 0.01f;
	ConfigureInput();
	//CreateHelpers();
	CreateShipMesh();

	m_input->Enabled = m_inputEnabled;

	return WorldEntity::OnInitialize();
}


void Ship::CreateShipMesh()
{
	{
		auto& material = Create<class Material>("ship-material");
		m_material = &material;

		vector<Vector3> vertices;
		vector<GLushort> indices;

		auto& mesh = Create<Mesh>("ship-mesh");

		GeometryProvider::Cone(vertices, indices, 2.f, 0.25f, 6, false);


		//auto center = GeometryProvider::FindCenter(vertices);


		material.FillType = PolygonMode::Fill;
		mesh.Type = BeginMode::Triangles;

		material.Build("Shaders/ship");

		mesh.Material = &material;
		mesh.Initialize(vertices, indices);

		GeometryProvider::FitToUnitCube(vertices);
		Bounds = BoundingSphere::FromVectors(vertices);

		mesh.CullBackfaces = false;
		m_mesh = &mesh;
	}

}

void Ship::CreateHelpers()
{
	auto& axis = Create<AxisHelper>("axis.helper");
	axis.Transform->Move(0, 0.5f, 0);
}


void Ship::ConfigureInput()
{
	auto& handler = Create<InputHandler>("ship.input.handler");
	handler.Enabled = false;
	m_input = &handler;

	const float forwardSpeed = 0.5f;
	const float spinSpeed = 0.3f;

#define xform (*this->Transform)
	handler.Subscribe(GLFW_KEY_UP,
		DECL_KEYHANDLER
	{
		auto dir = Transform->GetMatrix().Up() * forwardSpeed;
		xform.Push(dir * forwardSpeed * time.ElapsedSeconds());
	}
	);

	handler.Subscribe(GLFW_KEY_DOWN,
		DECL_KEYHANDLER
	{
		auto dir = Transform->GetMatrix().Up() * (-1.f * forwardSpeed);
		xform.Push(dir * forwardSpeed * time.ElapsedSeconds());
	}
	);


	handler.Subscribe(GLFW_KEY_RIGHT,
		DECL_KEYHANDLER
	{
		auto spin = Vector3(0, 0, spinSpeed);
		xform.Spin(spin * time.ElapsedSeconds());
	}

	);

	handler.Subscribe(GLFW_KEY_LEFT,
		DECL_KEYHANDLER
	{
		auto spin = Vector3(0, 0, -1.f * spinSpeed);
		xform.Spin(spin * time.ElapsedSeconds());
	}

	);

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

	handler.Subscribe(GLFW_KEY_LEFT_BRACKET,
		DECL_KEYHANDLER
	{
		xform.Spin(0, 0.01f, 0);
	}
	);

	handler.Subscribe(GLFW_KEY_RIGHT_BRACKET,
		DECL_KEYHANDLER
	{
		xform.Spin(0, -0.01f, 0);
	}
	);

	handler.Subscribe(GLFW_KEY_R,
		DECL_KEYHANDLER
	{
		xform.Reset();
	}
	);

#undef xform

}

void Ship::OnPreUpdate(const  GameTime& time)
{
	bool wasExploding = IsExploding;

	if (IsExploding)
	{
		ExplosionTime += time.ElapsedSeconds();

		if (ExplosionTime >= ExplosionDuration)
		{
			ExplosionTime = 0.f;
			IsExploding = false;
			ExplosionFactor = 0.f;
		}
		else
		{
			ExplosionFactor = ExplosionTime / ExplosionDuration;
		}

	}
	else
		ExplosionTime = 0.f;

	TimeUntilCanFire = max(TimeUntilCanFire - time.ElapsedSeconds(), 0);;

	if (IsExploding)
		EnableInput(false);
	else if (wasExploding && !IsExploding)
	{
		Transform->Reset();
		EnableInput(true);
		IsRespawning = true;
	}

	WorldEntity::OnPreUpdate(time);
}

bool Ship::CanFire()
{
	return TimeUntilCanFire <= 0;
}

bool Ship::Fire()
{

	if (!CanFire())
		return false;

	TimeUntilCanFire = FireIntervalSec;

	return true;
}

void Ship::OnRender(const GameTime& time)
{
	m_material->Bind();

	m_material->SetUniform("ExplosionFactor", ExplosionFactor);
	m_material->SetUniform("IsExploding", IsExploding ? 1.f : 0.f);

	WorldEntity::OnRender(time);

}


void Ship::Explode(const GameTime& time, const float duration)
{
	ExplosionTime = 0.f;
	if (duration > 0)
		ExplosionDuration = duration;

	Vector3 spin((rand() % 10) / 10.f, (rand() % 10) / 10.f, (rand() % 10) / 10.f);

	Transform->Spin(spin * 0.1f);

	IsExploding = true;
}





