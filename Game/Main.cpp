#include <iostream>
#include "Game/Game.h"
#include "Level/GameLevel.h"
#include "MapMaker/DungeonMaker.h"
#include "MapMaker/MonsterElite.h"
#include "Utils/AStarPathFinder.h"

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
	
	Game game;
	game.Run();

	//DungeonMaker dungeonSet(200, 50, 0.4f, 0.7f, 3);

    /*int* grid = new int[9*10];
    
    std::vector< std::vector<int>> map =
    {
            {1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
            {1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
            {1, 1, 1, 0, 1, 1, 0, 1, 0, 1},
            {0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
            {1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
            {1, 0, 1, 1, 1, 1, 0, 1, 0, 0},
            {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 0, 0, 0, 1, 0, 0, 1}
    };

    int k = 0;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            grid[k++] = map[i][j];
        }
    }

    std::vector<Node*> path;
    std::vector<char> road;
    road.emplace_back(1);
    road.emplace_back(' ');
    AStarPathfinder pathfinder(grid,10,9,Vector2(0,0),Vector2(9,8), road,path);
    for (Node*& node : path)
    {
        std::cout << node->x << ',' << node->y << std::endl;
        delete node;
        node = nullptr;
    }
    path.clear();
    delete[] grid;*/

}

