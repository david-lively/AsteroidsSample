#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>
#include <queue>
#include <iostream>

#include "WorldEntity.h"
#include "Ship.h"

class Scoreboard : public WorldEntity
{
public:
	int Score = 0;
	int LivesRemaining = 3;
	int AsteroidsRemaining = 4;
	int Level = 0;

	bool OnInitialize() override;

	int Kill();

	Scoreboard(const std::string& name) : WorldEntity(name)
	{

	}

	Scoreboard() : Scoreboard("scoreBoard")
	{

	}

	void OnRender(const GameTime& time) override
	{
		WorldEntity::OnRender(time);
	}

	void Reset();

	void LevelUp()
	{
		++Level;
	}

private:

	std::queue<Ship*> m_activeShips;
	std::queue<Ship*> m_inactiveShips;

};

#endif

