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

	Transform->Move(13, 10, 0);

	for (int i = 0; i < 3; ++i)
	{
		Ship* ship = &Create<Ship>("lifeIndicator." + to_string(i));

		ship->Transform->Move(i, 0, 0);
		
		m_activeShips.push(ship);
	}

	return success;
}

int Scoreboard::Kill()
{
	if (m_activeShips.size() > 0)
	{
		auto shipPtr = m_activeShips.front();
		shipPtr->Enabled = false;
		m_activeShips.pop();
		m_inactiveShips.push(shipPtr);
	}
	else
	{
		while (m_inactiveShips.size() > 0)
		{
			auto shipPtr = m_inactiveShips.front();
			shipPtr->Enabled = true;
			m_inactiveShips.pop();
			m_activeShips.push(shipPtr);
		}
	}



	LivesRemaining = m_activeShips.size();

	


	return LivesRemaining;

}



bool Scoreboard::OnPostInitialize()
{
	bool success = WorldEntity::OnPostInitialize();


	return success;
}

void Scoreboard::OnPreUpdate(const GameTime& time)
{
	//static bool firstFrame = true;

	//if (firstFrame)
	//{
	//	for (auto shipPtr : m_ships)
	//	{
	//		shipPtr->EnableInput(false);
	//	}

	//	firstFrame = false;
	//}
}



