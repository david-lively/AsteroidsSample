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
#include "Hud.h"
#include "Draw.h"

bool AsteroidsGame::OnCreateScene()
{
	m_ship = &CreateShip();
	m_grid = &CreateGrid();
	CreateTextPool();

	m_text->Enabled = false;
	//CreateMap();
	//m_map = &CreateMap();

	m_stateMachine = &CreateStateMachine();

	m_scoreboard = &Create<Scoreboard>("scoreboard");

	m_hud = &Create<Hud>("hud");
	m_hud->Transform.Move(-50.f, 0.5f, 0);

	m_stateMachine->Ship = m_ship;
	m_stateMachine->Scoreboard = m_scoreboard;

	m_grid->Enabled = false;

	CreateLights(m_lights);

	CreateAsteroids(4);


	Log::Info << "Created and queued " << m_asteroids.size() << " asteroids." << endl;

	ExpandMissilePool(10);

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

	input.Subscribe(GLFW_KEY_R,
		DECL_KEYHANDLER
	{
		static float lastResetTime = 0.f;

		float currSeconds = Time.TotalSeconds();
		if (currSeconds - lastResetTime > 0.5f)
		{
			lastResetTime = currSeconds;
			Reset(time);
		}


		//static int prev = GLFW_RELEASE;

		//int current = glfwGetKey(Window(), GLFW_KEY_R);

		//if (current == GLFW_RELEASE && prev == GLFW_PRESS)
		//{
		//	Log::Info << "Reloading all shaders." << endl;
		//	Reload("shaders");
		//}

		//Log::Info << "Key R: curr " << current << " prev " << prev << endl;

		//prev = current;
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
		auto& camTransform = Game::Instance().Camera().Transform;
		camTransform.Push(0.f, 0.1f, 0);
	}
	);

	input.Subscribe(GLFW_KEY_X,
		DECL_KEYHANDLER
	{
		auto& camTransform = Game::Instance().Camera().Transform;
		camTransform.Push(0.f, -0.1f, 0);
	}
	);


	input.Subscribe(GLFW_KEY_D,
		DECL_KEYHANDLER
	{
		auto& camTransform = Game::Instance().Camera().Transform;
		camTransform.Push(0.1f, 0, 0);
	}
	);


	input.Subscribe(GLFW_KEY_A,
		DECL_KEYHANDLER
	{
		auto& camTransform = Game::Instance().Camera().Transform;
		camTransform.Push(-0.1f, 0, 0);
	}
	);

	input.Subscribe(GLFW_KEY_W,
		DECL_KEYHANDLER
	{
		auto& camTransform = Game::Instance().Camera().Transform;
		camTransform.Push(0, 0, 0.1f);
	}
	);

	input.Subscribe(GLFW_KEY_S,
		DECL_KEYHANDLER
	{
		auto& camTransform = Game::Instance().Camera().Transform;
		camTransform.Push(0, 0, -0.1f);
	}
	);

	input.Subscribe(GLFW_KEY_R,
		DECL_KEYHANDLER
	{
		auto& camera = Game::Camera();
		auto& translation = camera.Transform.Translation;

		camera.Transform.Reset();
		camera.Transform.Move(0, 0, 20);
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


	input.Subscribe(GLFW_KEY_F10,
		DECL_KEYHANDLER
	{
		Environment().TestFloat += 0.005f;
		Log::Info << "Testfloat: " << Environment().TestFloat << "\r";
	}
	);

	input.Subscribe(GLFW_KEY_F9,
		DECL_KEYHANDLER
	{
		Environment().TestFloat -= 0.005f;
		Log::Info << "Testfloat: " << Environment().TestFloat << "\r";
	}
	);





	Game::Camera().Transform.Move(0, 0, 20);

	return true;
}

void AsteroidsGame::OnUpdate(const GameTime& time)
{
	UpdateStatus();
	Game::OnUpdate(time);
}

void AsteroidsGame::OnPreUpdate(const GameTime& time)
{
	m_scoreboard->AsteroidsRemaining = m_asteroids.size() - m_inactiveAsteroids.size();
	/// wrap moving items to view frustum
	DoCollisionCheck(time);
	DoWrapping(time);
}

Ship& AsteroidsGame::CreateShip()
{
	auto& ship = Create<Ship>("ship");

	ship.OnExploded =
		[=](const GameTime& time, GameObject& sender)
	{
		Ship* s = dynamic_cast<Ship*>(&sender);
		s->EnableInput(true);
		s->Transform.Reset();

	};

	ship.Transform.Scale = Vector3(2.f);

	ship.EnableInput(true);

	auto& text = ship.Create<Text>("score.text");

	text.Data = "The quick brown fox jumps over the lazy dog.";

	m_text = &text;

	return ship;
}

Grid& AsteroidsGame::CreateGrid()
{
	auto& grid = Create<Grid>("grid");

	grid.Transform.Scale = Vector3(20);

	return grid;

}

float randFloat()
{
	return (rand() % 100) * 1.f / 100.f;

}

Asteroid& AsteroidsGame::CreateAsteroid()
{
	auto& asteroid = Create<Asteroid>("asteroid");

	asteroid.TwoD = false;

	auto& transform = asteroid.Transform;

	transform.RotationDrag = 0.f;
	transform.TranslationDrag = 0.f;

	m_asteroids.push_back(&asteroid);
	m_itemsToWrap.push_back(&asteroid);
	asteroid.OnExitFrustum = FrustumAction::Wrap;

	asteroid.OnExploded =
		[=](const GameTime& time, GameObject& sender)
	{
		sender.Enabled = false;
		Asteroid* a = dynamic_cast<Asteroid*>(&sender);
		this->m_inactiveAsteroids.push(a);
	};

	asteroid.OnReset =
		[=]()
	{

	}
	;

	return asteroid;

}

void AsteroidsGame::CreateAsteroids(const int count, const int level)
{
	for (int i = 0; i < count; ++i)
	{
		float spread = 8.f;

		float theta = (i + 1) * 1.f / count * TO_RADIANS(360);
		theta += TO_RADIANS(45.f);

		Vector3 center(cosf(theta) * spread, sinf(theta) * spread, 0);

		auto& asteroid = GetAsteroid();
		/// start the asteroid moving...
		float radians = TO_RADIANS(rand() % 360);

		Vector3 dir(cosf(radians), sinf(radians), 0);
		asteroid.Transform.Reset();
		asteroid.Transform.Push(dir * 0.005f);
		/// and make it spin
		asteroid.Transform.Spin(Vector3(0, 0, 0.005f));
		asteroid.Transform.Move(center);
		asteroid.Enabled = true;
	}
}

void AsteroidsGame::ExpandMissilePool(const int count)
{
	Log::Info << "Expanding missile pool. Adding " << count << " items.";

	for (int i = 0; i < count; ++i)
	{
		auto& missile = GetMissile(true);
		DisableMissile(missile);
	}

	Log::Info << "New missile pool size is " << m_allMissiles.size();
}


void AsteroidsGame::CreateLights(vector<Light*>& lights)
{
	vector<float> positions =
	{
		0, 0, 1
		,
		//0, 0, 1
		//,
		//0, 0, -1
		//,
		1, 0, 0
		//,
		//-1, 0, 0
		//,
		//0, 1, 0
		//,
		//0, -1, 0
	};

	vector<float> colors =
	{
		1, 1, 1
		,
		0, 1, 0
		,
		0, 0, 1
		,
		1, 0, 1
		,
		1, 1, 1
		,
		1, 1, 0
	};

	int lightCount = min(positions.size(), colors.size()) / 3;
	auto& environment = Environment();

	for (int i = 0; i < 3 * lightCount; i += 3)
	{
		Vector3 pos(positions[i], positions[i + 1], positions[i + 2]);
		Vector4 color(colors[i], colors[i + 1], colors[i + 2], 1);

		auto& l = environment.CreateLight();

		l.Direction = pos;
		l.Color = color;
		l.Intensity = 1.f;
		l.Position = pos;
		l.Transform.Move(l.Position);
		//l.Transform.Sequence = TransformSequence::ScaleTranslateRotate;
		l.Transform.Spin(0, 0, TO_RADIANS(10.f));

		lights.push_back(&l);
	}

}

void AsteroidsGame::UpdateStatus()
{
	int activeCount = m_asteroids.size() - m_inactiveAsteroids.size();


	string info = "Score: " + to_string(m_scoreboard->Score) + " asteroids " + to_string(activeCount) + " lives " + to_string(m_scoreboard->LivesRemaining);

	m_hud->Transform.Translation = Vector3(-1, 1, Environment().TestFloat);
	m_hud->Data = info;

	m_text->RenderPosition.X = -1;
	m_text->RenderPosition.Y = +1;

}

Drawable& AsteroidsGame::GetMissile(bool forceCreateNew)
{

	if (0 == m_inactiveMissiles.size() || forceCreateNew)
	{
		auto& missile = Create<Missile>("missile");

		m_allMissiles.push_back(&missile);
		DisableMissile(missile);

		m_itemsToWrap.push_back(&missile);

		missile.OnExitFrustum = FrustumAction::Recycle;
	}

	auto& m = *m_inactiveMissiles.front();
	m_inactiveMissiles.pop();
	m.Transform.Reset();
	m.Enabled = true;

	return m;
}

Asteroid& AsteroidsGame::GetAsteroid(bool forceCreateNew)
{
	Asteroid* item = nullptr;

	if (forceCreateNew || m_inactiveAsteroids.size() == 0)
	{
		item = &CreateAsteroid();
	}
	else
	{
		item = m_inactiveAsteroids.front();
		m_inactiveAsteroids.pop();
	}

	item->Enabled = true;
	item->BreakPlanes.clear();

	item->PointValue = 10;
	item->BreaksRemaining = 2;
	item->Reset(Game::Instance().Time);
	item->Transform.Scale = Vector3(4.f);

	return *item;
}


void AsteroidsGame::Fire(Ship& ship)
{
	if (!ship.Fire())
		return;


	auto& missile = GetMissile();
	auto shipMatrix = m_ship->Transform.GetMatrix();

	auto up = shipMatrix.Up();

	missile.Transform.Reset();

	missile.Transform.SetRotation(m_ship->Transform.Rotation);
	missile.Transform.Move(shipMatrix.Translation());

	float missileSpeed = 0.04f;
	missile.Transform.Push(up * missileSpeed);

	Log::Debug << Time.FrameNumber() << " Fire! missile (" << missile.Id << ")" << endl;
}

vector<tuple<WorldEntity*, WorldEntity*>> AsteroidsGame::GetCollisionPairs()
{
	throw;
}

bool AsteroidsGame::CanRespawn()
{
	return true;

	/*
	check ship against the asteroids to make sure it can safely respawn
	at the center of the screen
	*/
	auto& ship = *m_ship;

	for (auto* asteroidPtr : m_asteroids)
	{


	}


}

void AsteroidsGame::OnReset(const GameTime& time)
{
	m_scoreboard->Reset();

	while (m_inactiveAsteroids.size() > 0)
		m_inactiveAsteroids.pop();

	for (auto a : m_asteroids)
	{
		if (a->Enabled)
			a->Explode(time);

		a->Enabled = false;
		m_inactiveAsteroids.push(a);
	}

	m_ship->Reset(time);
	m_ship->EnableInput(true);

	CreateAsteroids(4);// m_scoreboard->AsteroidsRemaining);
}

void AsteroidsGame::DoCollisionCheck(const GameTime& time)
{
	Ship& ship = *m_ship;

	if (ship.IsExploding)
		return;


	auto shipBounds = ship.Bounds();

	shipBounds = ship.Transform.TransformSphere(shipBounds);

	for (auto ptr : m_asteroids)
	{
		if (!ptr->Enabled || ptr->IsExploding)
			continue;

		auto* asteroid = dynamic_cast<Asteroid*>(ptr);
		if (nullptr == asteroid || !asteroid->Enabled)
			continue;

		auto asteroidBounds = asteroid->Bounds();
		asteroidBounds = asteroid->Transform.TransformSphere(asteroidBounds);

		if (asteroidBounds.Intersects(shipBounds))
		{
			Vector3 dir = shipBounds.Center - asteroidBounds.Center;

			ship.Explode(time, 3.f);
			ship.EnableInput(false);
			m_scoreboard->Kill();

			break;
		}

#if 1
		for (auto* missile : m_allMissiles)
		{
			if (!missile->Enabled)
				continue;

			auto missileBounds = missile->TransformedBounds;

			if (missileBounds.Intersects(asteroid->TransformedBounds))
			{

				m_scoreboard->Score += asteroid->PointValue;

				DisableMissile(*missile);

				if (asteroid->BreaksRemaining-- <= 0)
				{
					asteroid->Explode(time, 1.f);
					m_text->Transform.Translation = asteroid->Transform.Translation;
					m_text->Data = "BOOM!";
					m_text->Transform.Scale = Vector3(20);
				}
				else
				{
					Log::Info << "Break asteroid " << asteroid->Name << endl;

					for (int i = -1; i <= 1; i += 2)
					{
						auto& newAsteroid = GetAsteroid();

						newAsteroid.Transform = asteroid->Transform;
						newAsteroid.BreakPlanes = asteroid->BreakPlanes;
						newAsteroid.BreaksRemaining = asteroid->BreaksRemaining;

						newAsteroid.PointValue *= 2;

						float pushSpeed = asteroid->PointValue / 1000.f;
						Vector3 pushDir = missile->Transform.Velocity().Normalized();
						float t = pushDir.X;

						pushDir.X = -pushDir.Y;
						pushDir.Y = t;

						newAsteroid.Transform.Push(pushDir * pushSpeed * i);
						newAsteroid.Transform.Scale *= 0.75f;
					}

					/// explode the old asteroid so we get some nice particulates around the split
					asteroid->Explode(time, 1.f);

				}

				return;

			}



		}
#endif
	}


}

void AsteroidsGame::DisableMissile(Drawable& missile)
{
	m_inactiveMissiles.push(&missile);
	missile.Enabled = false;

}


void AsteroidsGame::DoWrapping(const GameTime& time)
{

	auto& camera = Game::Instance().Camera();
	auto& viewMatrix = camera.GetViewMatrix();

	for (auto entityPtr : m_itemsToWrap)
	{
		if (!entityPtr->Enabled)
			continue;

		auto& entity = *entityPtr;
		auto& worldView = entity.Transform.GetMatrix() * viewMatrix;

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
				Vector3 newPosition = entity.Transform.Translation;

				if (containment.X < 1)
					newPosition.X *= -0.99f;

				if (containment.Y < 1)
					newPosition.Y *= -0.99f;

				entity.Transform.Move(newPosition);
			}
			else if (FrustumAction::Recycle == entityPtr->OnExitFrustum)
			{

				auto* ptr = dynamic_cast<Missile*>(entityPtr);

				if (nullptr != ptr)
				{
					DisableMissile(*ptr);
				}
				else
					ptr->Enabled = false;

			}
		}

	}

}



StateMachine& AsteroidsGame::CreateStateMachine()
{
	auto& fsm = Create<StateMachine>("asteroids.fsm");
	fsm.Game = this;

	return fsm;


}

WorldEntity& AsteroidsGame::CreateMap()
{
	auto& m = Create<Draw>("map");

	m.Transform.Scale = Vector3(2);

	return m;

}

void AsteroidsGame::CreateTextPool()
{
}