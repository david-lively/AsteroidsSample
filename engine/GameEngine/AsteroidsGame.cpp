//
//  SimpleGame.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include <map>
#include <vector>
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

	CreateAsteroids(1, m_itemsToWrap);

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

		camera.Transform->Reset();
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


	return true;
}


void AsteroidsGame::OnUpdate(const GameTime& time)
{
	/// wrap moving items to view frustum

	auto clipBounds = m_grid->Transform->TransformAABB(m_grid->Bounds);
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

void AsteroidsGame::OnPreRender(const GameTime& time)
{
	auto shipMaterial = m_ship->GetFirst<Material>();
	if (nullptr != shipMaterial)
		shipMaterial->SetLights(m_lights);

	auto asteroid = GetFirst<Asteroid>();

	if (nullptr != asteroid)
	{
		auto asteroidMaterial = asteroid->GetFirst<Material>();
		if (nullptr != asteroidMaterial)
			asteroidMaterial->SetLights(m_lights);
	}

}


void AsteroidsGame::CreateAsteroids(int count, vector<WorldEntity*>& entities)
{
	Log::Warning << "Ignoring asteroid count (" << count << ") - creating 1 for debug\n";

	auto& asteroid = Create<Asteroid>("asteroid");
	entities.push_back(&asteroid);

	auto& transform = *asteroid.Transform;

	transform.Scale = Vector3(1);

	/// start the asteroid moving...
	float radians = TO_RADIANS(rand() % 360);

	Vector3 dir(cosf(radians), sinf(radians), 0);

	transform.Push(dir * 0.05f);

	/// and make it spin
	Vector3 spinSpeed((rand() % 10) / 10.f, (rand() % 10) / 10.f, 0);


}


void AsteroidsGame::CreateLights(vector<Light*>& lights)
{
	auto& sun = Create<Light>("sun");

	sun.Direction = Vector3(-1, -1, 0);
	sun.Color = Vector4(1, 1, 0, 1);
	sun.Intensity = 0.5f;

	lights.push_back(&sun);

}




