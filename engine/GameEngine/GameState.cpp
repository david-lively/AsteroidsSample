#include "GameState.h"


void LevelUpState::OnEnter(const GameTime& time)
{
	if (nullptr != OnLevelUp)
		OnLevelUp();
}

void ResetState::OnEnter(const GameTime& time)
{
	if (nullptr != OnReset)
		OnReset(time);

}
