#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <minmax.h>
#include <string>


#include "Common.h"
#include "GameObject.h"
#include "Sprite.h"

class GameState : public GameObject
{
public:
	GameState()
	{

	}

	GameState(const std::string& name) : GameObject(name)
	{

	}

	float TimeInState = 0.f;

	GameState* NextState = nullptr;

	virtual GameState& GetNext(const GameTime& time)
	{
		if (nullptr != NextState)
		{
			OnExit(time);
			NextState->OnEnter(time);
			return *NextState;
		}
		else
			return *this;
	}

	void OnUpdate(const GameTime& time) override
	{
		GameObject::OnUpdate(time);

		TimeInState += time.ElapsedSeconds();
	}


	virtual void OnEnter(const GameTime& time)
	{
		TimeInState = 0.f;
	}

	virtual void OnExit(const GameTime& time)
	{
		
	}

};


class SplashScreenState : public GameState
{
public:
	int WaitForSeconds = 4;
	Sprite* Title = nullptr;

	SplashScreenState()
	{

	}

	SplashScreenState(const std::string& name) : GameState(name)
	{

	}

	bool OnInitialize() override
	{
		Title = &Create<Sprite>("splash.screen");

		Title->Load("Textures/AsteroidsTitle.png");
		Title->Enabled = false;

		return GameState::OnInitialize();
	}


	GameState& GetNext(const GameTime& time) override
	{
		if (TimeInState >= WaitForSeconds)
		{
			return GameState::GetNext(time);
		}
		else
			return *this;
	}

	void OnEnter(const GameTime& time) override
	{
		Title->Enabled = true;
	}

	void OnExit(const GameTime& time) override
	{
		Title->Enabled = false;
	}

private:
};

class NoState : public GameState
{
public:

	NoState(const std::string& name) : GameState(name)
	{
		NextState = this;
	}
};

class PlayState : public GameState
{
public:
	NotifyEvent CreateGame;
	
	PlayState(const std::string& name) : GameState(name)
	{

	}

	bool OnInitialize() override
	{
		if (nullptr != CreateGame)
			CreateGame();

		return GameState::OnInitialize();
	}

private:
};





#endif

