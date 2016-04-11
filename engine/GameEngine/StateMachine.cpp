#include "StateMachine.h"
#include "GameState.h"


StateMachine::StateMachine()
{
}


StateMachine::~StateMachine()
{
}

bool StateMachine::OnInitialize()
{
	m_start = &Create<GameState>("start");
	
	auto splash = &Create<SplashScreenState>("splash");

	auto playState = &Create<class PlayState>("play");
	

	auto& wait = Create<WaitState>("wait");
	wait.WaitSeconds = 3;


	m_start->NextState = splash;
	splash->NextState = playState;
	playState->NextState = &wait;
	wait.NextState = &wait;
	
	m_current = m_start;

	PlayState = playState;

	PlayState->Ship = Ship;
	PlayState->Scoreboard = Scoreboard;


	return GameObject::OnInitialize();
}


void StateMachine::OnPreUpdate(const GameTime& time)
{
	if (nullptr != m_current)
		m_current = &m_current->GetNext(time);


	GameObject::OnPreUpdate(time);
}

