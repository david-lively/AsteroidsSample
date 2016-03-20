//
//  SimpleGame.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include <map>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>


using namespace std;

#include "AsteroidsGame.h"
#include "Mesh.h"
#include "Material.h"
#include "Files.h"
#include "InputHandler.h"
#include "Grid.h"
#include "Asteroid.h"
#include "Bounds.h"
#include "Light.h"

void QuitGame(const GameObject& sender, const GameTime& time)
{
	Log::Info << "Closing application\n";
	glfwSetWindowShouldClose(Game::Instance().Window(), true);
}

bool AsteroidsGame::OnCreateScene()
{
	CreateLights(m_lights);
	m_ship = &CreateShip();
	m_grid = &CreateGrid();

	CreateAsteroids(4, m_itemsToWrap);

	m_itemsToWrap.push_back(m_ship);

	auto& camera = Camera();


	auto& input = Create<InputHandler>("asteroids-input");


	input.Subscribe(GLFW_KEY_ESCAPE,
		DECL_KEYHANDLER
	{
		Log::Info << "Quit key received, closing.\n";
		glfwSetWindowShouldClose(Game::Instance().Window(), true);
	}

	);

	input.Subscribe(GLFW_KEY_F1,
		DECL_KEYHANDLER
	{
		if (nullptr != m_grid)
		m_grid->Enabled = !m_grid->Enabled;
	}

	);

	input.Subscribe(GLFW_KEY_W,
		DECL_KEYHANDLER
	{
		auto& camera = Game::Camera();

		camera.Transform->Spin(Vector3(0.01f, 0, 0));
	}
	);

	input.Subscribe(GLFW_KEY_S,
		DECL_KEYHANDLER
	{
		auto& camera = Game::Camera();

		camera.Transform->Spin(Vector3(-0.01f, 0, 0));
	}
	);

	input.Subscribe(GLFW_KEY_E,
		DECL_KEYHANDLER
	{
		auto& camera = Game::Camera();
		auto& translation = camera.Transform->Translation;

		camera.Transform->Reset();
	
		camera.Transform->Move(0, 0, 9);
	}
	);

	input.Subscribe(GLFW_KEY_F2,
		DECL_KEYHANDLER
	{
		static bool evenOdd = true;
		float fov = evenOdd ? TO_RADIANS(45.f) : TO_RADIANS(120.f);

		Game::Camera().SetFieldOfView(fov);

		evenOdd = !evenOdd;

	}
	);

	Game::Camera().Transform->Move(0, 0, 9);

	return true;
}

void AsteroidsGame::OnUpdate(const GameTime& time)
{
	auto& ship = *m_ship;
	auto shipBounds = ship.Transform->TransformAABB(ship.Bounds);



	for (auto asteroid : m_asteroids)
	{
		auto bounds = asteroid->Transform->TransformAABB(asteroid->Bounds);

		if (shipBounds.Intersects(bounds) != IntersectionType::Disjoint)
			;


	}

}

void AsteroidsGame::OnPreUpdate(const GameTime& time)
{
	/// wrap moving items to view frustum

	auto& camera = Game::Instance().Camera();
	auto& viewMatrix = camera.GetViewMatrix();

	for (auto entityPtr : m_itemsToWrap)
	{
		auto& entity = *entityPtr;
		auto& worldView = entity.Transform->GetMatrix() * viewMatrix;

		Vector3 center(0);
		Vector3 bound(0.5f, 0, 0);

		center = worldView.Transform(center);
		bound = worldView.Transform(bound);
	
		float radius = (bound - center).Length();

		auto material = entity.GetFirst<Material>();

		if (nullptr != material)
		{
			Vector3 containment;

			if (camera.ContainsSphere(center, radius, containment))
			{
				material->SetUniform("Color", Vector4(0, 1.f, 0, 1));
			}
			else
			{
				Vector3 newPosition = entity.Transform->Translation;

				if (containment.X < 1)
					newPosition.X *= -0.99f;

				if (containment.Y < 1)
					newPosition.Y *= -0.99f;

				entity.Transform->Move(newPosition);

				Vector4 color(containment.X, containment.Y, containment.Z, 1);
				material->SetUniform("Color", color);
			}
		}

	}

}

Ship& AsteroidsGame::CreateShip()
{
	auto& ship = Create<Ship>("ship");

	ship.Transform->Scale = Vector3(1.5f);

	return ship;
}

Grid& AsteroidsGame::CreateGrid()
{
	auto& grid = Create<Grid>("grid");

	grid.Transform->Scale = Vector3(20);

	return grid;

}

float randFloat()
{
	return (rand() % 100) * 1.f / 100.f;

}

void AsteroidsGame::CreateAsteroids(int count, vector<WorldEntity*>& entities)
{
	Log::Warning << "Ignoring asteroid count (" << count << ") - creating 1 for debug\n";

	auto& asteroid = Create<Asteroid>("asteroid");
	entities.push_back(&asteroid);
	m_asteroids.push_back(&asteroid);


	auto& transform = *asteroid.Transform;

	transform.Scale = Vector3(1.5f);
	transform.Move(randFloat() * 10, randFloat() * 10, 0);

	/// start the asteroid moving...
	float radians = TO_RADIANS(rand() % 360);

	Vector3 dir(cosf(radians), sinf(radians), 0);

	transform.Push(dir * 0.01f);
	/// and make it spin
	transform.Spin(Vector3(0, 0.001f, 0));


	--count;

	if (count > 0)
		CreateAsteroids(count, entities);
}


void AsteroidsGame::CreateLights(vector<Light*>& lights)
{
	vector<float> positions =
	{
		-1,1,0
		,
		1,1,0
		,
		1,-1,0
		,
		-1,-1,0
	};

	vector<float> colors =
	{
		1, 0, 0
		,
		0, 1, 0
		,
		0, 0, 1
		,
		1, 0, 1
	};

	int lightCount = min(positions.size(), colors.size()) / 3;
	auto& environment = Environment();

	for (int i = 0; i < 3 * lightCount; i += 3)
	{
		Vector3 pos(positions[i], positions[i + 1], positions[i + 2]);
		Vector4 color(colors[i], colors[i + 1], colors[i + 2], 1);

		auto& l = environment.CreateLight();

		l.Direction = -1.f * pos;
		l.Color = color;
		l.Intensity = 1.f;
		l.Position = pos * 9.f;
		l.Transform->Move(l.Position);
		
		lights.push_back(&l);
	}

}




