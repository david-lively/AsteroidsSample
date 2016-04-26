#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <minmax.h>
#include <string>


#include "Common.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Ship.h"
#include "Scoreboard.h"

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

class WaitState : public GameState
{
public:
	float WaitSeconds = 0;

	WaitState(const std::string& name) : GameState(name)
	{

	
	}

	GameState& GetNext(const GameTime& time) override
	{
		if (TimeInState >= WaitSeconds)
		{
			return GameState::GetNext(time);
		}
		else
			return *this;
	}



private:
};


class SplashScreenState : public WaitState
{
public:
	Sprite* Title = nullptr;

	SplashScreenState() : SplashScreenState("splash")
	{

	}

	SplashScreenState(const std::string& name) : WaitState(name)
	{

	}

	bool OnInitialize() override
	{
		Title = &Create<Sprite>("splash.screen");

		Title->Load("Textures/AsteroidsTitle.png");
		Title->Enabled = false;
		Title->InScreenSpace = true;

		return GameState::OnInitialize();
	}

	//GameState& GetNext(const GameTime& time) override
	//{
	//	if (TimeInState >= WaitForSeconds)
	//	{
	//		return GameState::GetNext(time);
	//	}
	//	else
	//		return *this;
	//}

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
	Ship* Ship = nullptr;
	Scoreboard* Scoreboard = nullptr;

	GameState* OnLevelComplete = nullptr;
	GameState* OnDead = nullptr;

	PlayState(const std::string& name) : GameState(name)
	{

	}

	bool OnInitialize() override
	{
		if (nullptr != CreateGame)
			CreateGame();

		return GameState::OnInitialize();
	}

	bool IsDead()
	{
		return Scoreboard->LivesRemaining == 0;
	}

	bool IsLevelComplete()
	{
		bool levelComplete = Scoreboard->AsteroidsRemaining == 0;
		
		return levelComplete;

	}

	GameState& GetNext(const GameTime& time) override
	{
		if (IsDead())
			NextState = OnDead;
		else if (IsLevelComplete())
			NextState = OnLevelComplete;
		else
			NextState = this;

		return GameState::GetNext(time);
	}

private:


};

class LevelUpState : public WaitState
{
public:
	NotifyEvent OnLevelUp;

	LevelUpState(const std::string& name) : WaitState(name)
	{

	}

	LevelUpState() : LevelUpState("levelup")
	{

	}

	void OnEnter(const GameTime& time) override;

private:
};

class ResetState : public WaitState
{
public:
	NotifyTimeEvent OnReset;


	ResetState(const std::string& name) : WaitState(name)
	{

	}

	ResetState() : ResetState("levelup")
	{

	}

	void OnEnter(const GameTime& time) override;


};





#endif

