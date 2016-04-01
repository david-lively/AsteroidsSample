#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>
#include <queue>

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

	int Kill();

	void OnPreUpdate(const GameTime& time) override;

	Scoreboard();
	~Scoreboard();

private:

	std::queue<Ship*> m_activeShips;
	std::queue<Ship*> m_inactiveShips;

};

#endif

