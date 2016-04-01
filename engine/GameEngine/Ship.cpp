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
	Transform->Drag = 0.01f;
	ConfigureInput();
	//CreateHelpers();
	CreateShipMesh();

	return WorldEntity::OnInitialize();
}

void Ship::CreateShipMesh()
{
	auto& material = Create<class Material>("ship-material");
	m_material = &material;

	vector<Vector3> vertices;
	vector<GLushort> indices;

	auto& mesh = Create<Mesh>("ship-mesh");

#ifdef CONE_SHIP
	GeometryProvider::Cone(vertices, indices, 1.5f, 0.5f, 12);
	auto center = GeometryProvider::FindCenter(vertices);

	Log::Debug << "Ship center: " << center << endl;
	GeometryProvider::FitToUnitCube(vertices);

	material.FillType = PolygonMode::Fill;
	mesh.Type = BeginMode::Triangles;

	material.Build("Shaders/ship");
#else
	material.FillType = PolygonMode::Line;
	mesh.Type = BeginMode::Lines;
	material.Build("Shaders/primitive");


	vector<float> coordinates =
	{
		0, 0.5f, 0
		,
		1 / 3.f, -0.5f, 0
		,
		-1 / 3.f, -0.5f, 0
		,
		-1 / 4.f, -0.25f, 0
		,
		+1 / 4.f, -0.25f, 0

	};

	vertices = GeometryProvider::ToVectors(coordinates);

	indices =
	{
		0, 1
		,
		0, 2
		,
		3, 4
	};


#endif

	GeometryProvider::FitToUnitCube(vertices);
	GeometryProvider::Circle(vertices, indices, Vector3::Zero, 0.5f, 12);

	mesh.Material = &material;
	mesh.Initialize(vertices, indices);

	Bounds = BoundingSphere::FromVectors(vertices);

	m_mesh = &mesh;
}

void Ship::CreateHelpers()
{
	auto& axis = Create<AxisHelper>("axis.helper");
	axis.Transform->Move(0, 0.5f, 0);
}




void Ship::ConfigureInput()
{
	auto& handler = Create<InputHandler>("ship.input.handler");
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

	handler.Subscribe(GLFW_KEY_LEFT_BRACKET,
		DECL_KEYHANDLER
	{
		ExplodeScale += 0.2f;
		Log::Debug << "ship explode " << ExplodeScale << endl;
	}
	);

	handler.Subscribe(GLFW_KEY_RIGHT_BRACKET,
		DECL_KEYHANDLER
	{
		ExplodeScale -= 0.2f;
		Log::Debug << "ship explode " << ExplodeScale << endl;
	}
	);

#undef xform

}

void Ship::OnPreUpdate(const  GameTime& time)
{

	TimeUntilCanFire -= time.ElapsedSeconds();

	if (TimeUntilCanFire < 0)
		TimeUntilCanFire = 0;

	bool wasExploding = ExplodeTimeRemaining > 0;
	
	ExplodeTimeRemaining = max(ExplodeTimeRemaining - time.ElapsedSeconds(), 0);

	bool isExploding = ExplodeTimeRemaining > 0;
	
	return;

	if (isExploding)
		EnableInput(false);
	else if (wasExploding && !isExploding)
	{
		Transform->Reset();
		EnableInput(true);
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

	auto moveBy = ExplodeTimeRemaining;

	m_material->SetUniform("MoveBy", moveBy);

	WorldEntity::OnRender(time);

}


void Ship::Explode(const GameTime& time, const float explosionTime)
{
	ExplodeTimeRemaining = explosionTime;

	//Transform->Spin(0, 0, 0.02f);
	//EnableInput(false);
}





