#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>

#include "WorldEntity.h"
#include "Ship.h"

class Scoreboard : public WorldEntity
{
public:
	int Score;
	int LivesRemaining;
	int AsteroidsRemaining;

	bool OnInitialize() override;
	bool OnPostInitialize() override;

	void OnPreUpdate(const GameTime& time) override;

	Scoreboard();
	~Scoreboard();

private:
	std::vector<Ship*> m_ships;

};

#endif

