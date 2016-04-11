#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Common.h"
#include "GameObject.h"

class GameState : public GameObject
{
public:
	int LivesRemaining = 3;
	int Score = 0;

	int Level;

	GameState();
	~GameState();
	
	virtual GameState& GetNext(const GameTime& time)
	{
		return *this;
	}

};

class SplashScreenState : public GameState
{
public:
private:


};



#endif

