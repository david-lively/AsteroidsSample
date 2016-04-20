#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <vector>
#include <memory>

#include "GameObject.h"
#include "GameState.h"
#include "Ship.h"
#include "Scoreboard.h"

class StateMachine : public GameObject
{
public:
	class AsteroidsGame* Game;
	PlayState* PlayState;
	Ship* Ship;
	Scoreboard* Scoreboard;
	
	
	StateMachine();
	~StateMachine();



	StateMachine(const std::string& name) : GameObject(name)
	{

	}

	bool OnInitialize();

	void Start();
	void OnPreUpdate(const GameTime& time) override;


private:
	GameState* m_start = nullptr;
	GameState* m_current = nullptr;

};

#endif

