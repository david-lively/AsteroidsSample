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
#include <thread>

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


bool AsteroidsGame::OnCreateScene()
{
	m_ship = &CreateShip();
	m_grid = &CreateGrid();
	m_scoreboard = &Create<Scoreboard>("scoreboard");

	m_grid->Enabled = false;

	CreateLights(m_lights);

	CreateAsteroids(4, 4, m_itemsToWrap);

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

	input.Subscribe(GLFW_KEY_E,
		DECL_KEYHANDLER
	{
		auto& camTransform = *Game::Instance().Camera().Transform;
		camTransform.Push(0.f, 0.1f, 0);
	}
	);

	input.Subscribe(GLFW_KEY_X,
		DECL_KEYHANDLER
	{
		auto& camTransform = *Game::Instance().Camera().Transform;
		camTransform.Push(0.f, -0.1f, 0);
	}
	);


	input.Subscribe(GLFW_KEY_D,
		DECL_KEYHANDLER
	{
		auto& camTransform = *Game::Instance().Camera().Transform;
		camTransform.Push(0.1f, 0, 0);
	}
	);


	input.Subscribe(GLFW_KEY_A,
		DECL_KEYHANDLER
	{
		auto& camTransform = *Game::Instance().Camera().Transform;
		camTransform.Push(-0.1f, 0, 0);
	}
	);

	input.Subscribe(GLFW_KEY_W,
		DECL_KEYHANDLER
	{
		auto& camTransform = *Game::Instance().Camera().Transform;
		camTransform.Push(0, 0, 0.1f);
	}
	);

	input.Subscribe(GLFW_KEY_S,
		DECL_KEYHANDLER
	{
		auto& camTransform = *Game::Instance().Camera().Transform;
		camTransform.Push(0, 0, -0.1f);
	}
	);

	input.Subscribe(GLFW_KEY_R,
		DECL_KEYHANDLER
	{
		auto& camera = Game::Camera();
		auto& translation = camera.Transform->Translation;

		camera.Transform->Reset();

		camera.Transform->Move(0, 0, 20);
	}
	);


	input.Subscribe(GLFW_KEY_F3,
		DECL_KEYHANDLER
	{
		bool forceWireframe = Environment().ForceWireframe = !Environment().ForceWireframe;

		Log::Debug << "Wireframe: " << forceWireframe << endl;

	}
	);

	input.Subscribe(GLFW_KEY_SPACE,
		DECL_KEYHANDLER
	{
		Fire(*m_ship);
	}

	);

	input.Subscribe(GLFW_KEY_CAPS_LOCK,
		DECL_KEYHANDLER
	{
		Log::Debug << "Triggering soft breakpoint...\n";
		DEBUG_BREAK;
	}

	);

	input.Subscribe(GLFW_KEY_0,
		DECL_KEYHANDLER
	{
		float& contrast = Environment().Contrast;

		contrast += 0.1f;

	}
	);

	input.Subscribe(GLFW_KEY_9,
		DECL_KEYHANDLER
	{
		float& contrast = Environment().Contrast;

		contrast -= 0.1f;

		Log::Info << "Contrast: " << contrast << endl;
	}
	);

	input.Subscribe(GLFW_KEY_F11,
		DECL_KEYHANDLER
	{
		Log::Info << "There are " << CountObjects() << " objects in the hierarchy." << endl;
	});
		


	Game::Camera().Transform->Move(0, 0, 20);

	return true;
}

void AsteroidsGame::OnUpdate(const GameTime& time)
{
	Game::OnUpdate(time);
}

void AsteroidsGame::OnPreUpdate(const GameTime& time)
{
	/// wrap moving items to view frustum
	DoCollisionCheck(time);
	DoWrapping(time);
}

Ship& AsteroidsGame::CreateShip()
{
	auto& ship = Create<Ship>("ship");

	ship.Transform->Scale = Vector3(1.5f);

	ship.EnableInput(true);

	//ship.Transform->Spin(Vector3(0, 0.1f, 0));

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


void AsteroidsGame::CreateAsteroids(const int count, const int total, vector<WorldEntity*>& entities)
{
	Log::Info << "createAsteroids count " << count << " total " << total << " have " << entities.size() << " items " << endl;
	float spread = 8.f;

	float theta = (count + 1) * 1.f / total * TO_RADIANS(360);
	theta += TO_RADIANS(45.f);

	Vector3 center(cosf(theta) * spread, sinf(theta) * spread, 0);

	auto& asteroid = Create<Asteroid>("asteroid");
	m_asteroids.push_back(&asteroid);
	
	asteroid.TwoD = false;

	entities.push_back(&asteroid);

	auto& transform = *asteroid.Transform;

	transform.Scale = Vector3(4.f);
	transform.Move(center);

	/// start the asteroid moving...
	float radians = TO_RADIANS(rand() % 360);

	Vector3 dir(cosf(radians), sinf(radians), 0);

	transform.Push(dir * 0.005f);
	/// and make it spin
	transform.Spin(Vector3(0, 0, 0.005f));

	if (count > 1)
		CreateAsteroids(count - 1, total, entities);
}


//void AsteroidsGame::CreateAsteroids(const int count, const int total, vector<WorldEntity*>& entities)
//{
//	createAsteroids(*this, count, total, entities);
//	//thread t1(createAsteroids,*this,count,total,entities);
//	//
//	//t1.join();
//
//}
//

void AsteroidsGame::CreateLights(vector<Light*>& lights)
{
	vector<float> positions =
	{
		-1, 1, 0
		,
		1, 1, 0
		,
		1, -1, 0
		,
		-1, -1, 0
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

Missile& AsteroidsGame::GetAMissile()
{
	Missile* missile = nullptr;

	if (m_inactiveMissiles.size() > 0)
	{
		/// do we have any reusable missiles that have already been created?
		missile = m_inactiveMissiles.front();
		missile->Enabled = true;
		m_inactiveMissiles.pop();
	}
	else
	{
		missile = &Create<Missile>("missile");
		m_itemsToWrap.push_back(missile);
		m_allMissiles.push_back(missile);
	}

	return *missile;
}


void AsteroidsGame::Fire(Ship& ship)
{
	if (!ship.Fire())
		return;

	Log::Debug << Time.FrameNumber() << " Fire!\n";

	auto& missile = GetAMissile();
	auto shipMatrix = m_ship->Transform->GetMatrix();

	auto up = shipMatrix.Up();
	
	missile.Transform->Reset();
	
	missile.Transform->SetRotation(m_ship->Transform->Rotation);
	missile.Transform->Move(shipMatrix.Translation());

	float missileSpeed = 0.04f;
	missile.Transform->Push(up * missileSpeed);
}

void AsteroidsGame::DoCollisionCheck(const GameTime& time)
{
	Ship& ship = *m_ship;
	
	if (ship.IsExploding)
		return;
	
	auto shipBounds = ship.Bounds;

	shipBounds = ship.Transform->TransformSphere(shipBounds);

	for (Asteroid* asteroid : m_asteroids)
	{
		auto asteroidBounds = asteroid->Bounds;
		asteroidBounds = asteroid->Transform->TransformSphere(asteroidBounds);

		if (asteroidBounds.Intersects(shipBounds))
		{
			Vector3 dir = shipBounds.Center - asteroidBounds.Center;
			
			//ship.Transform->Bounce(dir);
			ship.Explode(time, 5.f);
			m_scoreboard->Kill();

			Log::Info << "Ship hit asteroid " << asteroid->Name << endl;

			//m_ship->Transform->Stop();
			//m_ship->Transform->Move(0, 0, 0);
		}


	}

}

void AsteroidsGame::DoWrapping(const GameTime& time)
{
	auto title = "Missiles: active " + to_string(m_allMissiles.size()) + " inactive " + to_string(m_inactiveMissiles.size());


	auto& camera = Game::Instance().Camera();
	auto& viewMatrix = camera.GetViewMatrix();

	for (auto entityPtr : m_itemsToWrap)
	{
		if (!entityPtr->Enabled)
			continue;

		auto& entity = *entityPtr;
		auto& worldView = entity.Transform->GetMatrix() * viewMatrix;

		Vector3 center(0);
		Vector3 bound(0.5f, 0, 0);

		center = worldView.Transform(center);
		bound = worldView.Transform(bound);

		float radius = (bound - center).Length();

		Vector3 containment;

		if (!camera.ContainsSphere(center, radius, containment))
		{
			if (FrustumAction::Wrap == entityPtr->OnExitFrustum)
			{
				Vector3 newPosition = entity.Transform->Translation;

				if (containment.X < 1)
					newPosition.X *= -0.99f;

				if (containment.Y < 1)
					newPosition.Y *= -0.99f;

				entity.Transform->Move(newPosition);
			}
			else if (FrustumAction::Recycle == entityPtr->OnExitFrustum)
			{
				entityPtr->Enabled = false;

				Missile* ptr = dynamic_cast<Missile*>(entityPtr);

				if (nullptr != ptr)
					m_inactiveMissiles.push(ptr);

			}
		}

	}

}



