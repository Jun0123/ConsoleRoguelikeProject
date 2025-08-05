#include "Game.h"
#include "Level/GameLevel.h"
#include "Level/DungeonLevel.h"
Game::Game()
{
	AddLevel(new DungeonLevel());
}