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
#include "WorldEntity.h"
#include "Primitive.h"
#include <vector>
#include <cmath>

using namespace std;

bool Ship::OnInitialize()
{
	Transform.TranslationDrag = 0.01f;
	Transform.RotationDrag = 0.125f;
	ConfigureInput();
	CreateShipMesh();

	CreateAccessories();

	m_input->Enabled = m_inputEnabled;

	return Explodable::OnInitialize();
}


void Ship::CreateShipMesh()
{
	vector<Vector3> vertices;
	vector<GLushort> indices;

	GeometryProvider::Cone(vertices, indices, 2.f, 0.25f, 6, false);
	//GeometryProvider::Icosahedron(vertices, indices);
	//GeometryProvider::Tessellate(vertices, indices, 2);

	GeometryProvider::FitToUnitCube(vertices);
	Mesh.Bounds = BoundingSphere::FromVectors(vertices);

	Material.FillType = PolygonMode::Fill;
	Mesh.Type = BeginMode::Triangles;

	Material.Build("Shaders/ship");

	Mesh.Initialize(vertices, indices);


	Mesh.CullBackfaces = false;

}

void Ship::ConfigureInput()
{
	auto& handler = Create<InputHandler>("ship.input.handler");
	handler.Enabled = false;
	m_input = &handler;

	const float forwardSpeed = 0.5f;
	const float spinSpeed = 1.f;

#define xform (this->Transform)
	handler.Subscribe(GLFW_KEY_UP,
		DECL_KEYHANDLER
	{
		auto dir = Transform.GetMatrix().Up() * forwardSpeed;
		xform.Push(dir * forwardSpeed * time.ElapsedSeconds());
	}
	);

	handler.Subscribe(GLFW_KEY_DOWN,
		DECL_KEYHANDLER
	{
		auto dir = Transform.GetMatrix().Up() * (-1.f * forwardSpeed);
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
	Explodable::OnPreUpdate(time);

	bool wasExploding = IsExploding;

	TimeUntilCanFire = max(TimeUntilCanFire - time.ElapsedSeconds(), 0);;

	//if (IsExploding)
	//	EnableInput(false);
	//else if (wasExploding && !IsExploding)
	//{
	//	Transform.Reset();
	//	EnableInput(true);
	//	IsRespawning = true;
	//}

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


void Ship::CreateAccessories()
{
	return;

	WorldEntity* parent = this;

	for (int i = 0; i < 5; ++i)
	{
		auto& orbiter = parent->Create<Primitive>("ship.orbiter.asteroid");

		if (0 == i)
			m_orbiter = &orbiter;

		orbiter.Transform.Sequence = TransformSequence::ScaleTranslateRotate;// ::ScaleRotateTranslate;// TranslateRotate;
		orbiter.Transform.Scale *= 0.5f;
		orbiter.Transform.Move(2.f, 0, 0);
		orbiter.Transform.Spin(0, 0, 10);// TO_RADIANS(10.f));
		orbiter.Transform.RotationDrag = 0.f;

		parent = &orbiter;

	}

}

