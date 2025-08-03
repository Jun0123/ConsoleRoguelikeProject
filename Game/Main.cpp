#include <iostream>
#include "Game/Game.h"
#include "Level/GameLevel.h"
#include "MapMaker/DungeonMaker.h"
#include "MapMaker/MonsterElite.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//Game game;
	//game.Run();

	DungeonMaker dungeonSet(200, 50, 0.4f, 0.7f, 3);

}

