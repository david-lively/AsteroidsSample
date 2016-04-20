#include "StateMachine.h"
#include "GameState.h"
#include "AsteroidsGame.h"
#include "Scoreboard.h"


StateMachine::StateMachine()
{
}


StateMachine::~StateMachine()
{
}

bool StateMachine::OnInitialize()
{
	m_start = &Create<GameState>("start");

	auto& splash = Create<SplashScreenState>("splash");
	splash.WaitSeconds = 3.f;


	auto& reset = Create<ResetState>("reset");

	m_start->NextState = &splash;

	splash.NextState = &reset;

	auto& levelUp = Create<LevelUpState>("levelUp");
	auto& playState = Create<class PlayState>("play");

	playState.NextState = &playState;
	playState.OnLevelComplete = &levelUp;
	playState.OnDead = &reset;

	levelUp.NextState = &playState;
	levelUp.WaitSeconds = 5;
	levelUp.OnLevelUp = [this]()
	{
		this->Scoreboard->LevelUp();

		int numAsteroids = 4 + this->Scoreboard->Level;

		this->Scoreboard->AsteroidsRemaining = numAsteroids;

		this->Game->CreateAsteroids(numAsteroids);
	};



	playState.Ship = Ship;
	playState.Scoreboard = Scoreboard;

	reset.NextState = &playState;
	reset.OnReset = [this](const GameTime& time)
	{
		this->Game->Reset(time);
		//auto& sb = *this->Scoreboard;

		//sb.LivesRemaining = 3;
		//sb.Score = 0;
		//sb.AsteroidsRemaining = 4;
		//


		//this->Ship->Reset();
	};

	m_current = m_start;

	PlayState = &playState;

	return GameObject::OnInitialize();
}


void StateMachine::OnPreUpdate(const GameTime& time)
{
	if (nullptr != m_current)
		m_current = &m_current->GetNext(time);


	GameObject::OnPreUpdate(time);
}

