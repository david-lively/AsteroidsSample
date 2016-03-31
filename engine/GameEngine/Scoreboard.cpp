#include "Scoreboard.h"

using namespace std;


Scoreboard::Scoreboard()
{
}


Scoreboard::~Scoreboard()
{
}


bool Scoreboard::OnInitialize()
{
	bool success = WorldEntity::OnInitialize();

	Transform->Move(6, 6, 0);

	for (int i = 0; i < 3; ++i)
	{
		Ship* ship = &Create<Ship>("lifeIndicator." + to_string(i));

		ship->Transform->Move(i, 0, 0);
		ship->EnableInput(false);

		
		m_ships.push_back(ship);
	}

	return success;
}

bool Scoreboard::OnPostInitialize()
{
	bool success = WorldEntity::OnPostInitialize();


	return success;
}

void Scoreboard::OnPreUpdate(const GameTime& time)
{
	static bool firstFrame = true;

	if (firstFrame)
	{
		for (auto shipPtr : m_ships)
		{
			shipPtr->EnableInput(false);
		}

		firstFrame = false;
	}
}



