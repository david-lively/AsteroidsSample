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
		Log::Info << "Rendering Scoreboard" << std::endl;
		WorldEntity::OnRender(time);
	}

private:

	std::queue<Ship*> m_activeShips;
	std::queue<Ship*> m_inactiveShips;

};

#endif

