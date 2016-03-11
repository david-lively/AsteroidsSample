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

#include <vector>
#include <cmath>

using namespace std;

bool Ship::OnInitialize()
{
	ConfigureInput();

	auto& material = Create<class Material>("ship-material");
	m_material = &material;

	material.FillType = PolygonMode::Line;

	auto& mesh = Create<Mesh>("ship-mesh");

	vector<float> vertices =
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


	vector<GLushort> indices =
	{
		0, 1
		,
		0, 2
		,
		3, 4
	};

	Bounds = BoundingBox::FromFloats(vertices);

	material.Build("Shaders/primitive");

	mesh.Material = &material;

	mesh.Initialize(vertices, indices);
	mesh.Type = BeginMode::Lines;
	m_mesh = &mesh;

	return true;

}

void Ship::OnUpdate(const GameTime& time)
{


}


void Ship::OnRender(const GameTime& time)
{
	auto& cam = Game::Camera();

	cam.Transform->Translation.Z = 7;


	m_material->Bind();
	m_material->SetUniform("World", Transform->GetMatrix());
	m_material->SetUniform("View", cam.GetViewMatrix());
	m_material->SetUniform("Projection", cam.GetProjectionMatrix());
}



void Ship::ConfigureInput()
{
	auto& handler = Create<InputHandler>("ship.input.handler");

	const float forwardSpeed = 0.5f;
	const float spinSpeed = 0.1f * PI;

	handler.Subscribe(GLFW_KEY_UP,
		DECL_KEYHANDLER
	{
		float speed = forwardSpeed * time.ElapsedSeconds();
		auto push = Transform->GetMatrix().Up() * speed;
		this->Transform->Push(push);
	}
	);

	handler.Subscribe(GLFW_KEY_DOWN,
		DECL_KEYHANDLER
	{
		float speed = forwardSpeed * time.ElapsedSeconds();
		auto push = Transform->GetMatrix().Up() * speed;
		this->Transform->Push(push * -1.f);
	}
	);


	handler.Subscribe(GLFW_KEY_RIGHT,
		DECL_KEYHANDLER
	{
		float speed = spinSpeed * time.ElapsedSeconds();
		this->Transform->Spin(Vector3(0, 0, speed));
	}

	);

	handler.Subscribe(GLFW_KEY_LEFT,
		DECL_KEYHANDLER
	{
		float speed = spinSpeed * time.ElapsedSeconds();
		this->Transform->Spin(Vector3(0, 0, -1.f * speed));
	}

	);

}


