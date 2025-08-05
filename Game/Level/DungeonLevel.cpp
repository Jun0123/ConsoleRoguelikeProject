#include "DungeonLevel.h"
#include "Engine.h"
#include "MapMaker/DungeonMaker.h"
#include "Actors/DGPlayer.h"

#define MAPSIZEX 100
#define MAPSIZEY 50

DungeonLevel::DungeonLevel()
{
	LoadMap();
	SpawnPlayer();
	
	player->TestEvent<DungeonLevel, &DungeonLevel::CanMoveActor>(this);
}

DungeonLevel::~DungeonLevel()
{
	SafeDeleteArray(map);
}

void DungeonLevel::BeginPlay()
{
	super::BeginPlay();
	
}

void DungeonLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void DungeonLevel::Render()
{
	if (map != nullptr)
	{
		for (int y = 0; y < MAPSIZEY; ++y)
		{
			for (int x = 0; x < MAPSIZEX; ++x)
			{
				Engine::Get().WriteToValue(Vector2(x, y), (char)map[y * MAPSIZEX + x]);
			}
		}
	}

	super::Render();
}

//플레이어 방 타입을 찾고 해당 방 중심에 플레이어를 스폰
void DungeonLevel::SpawnPlayer()
{
	size_t playerRoomIdx = -1;

	//플레이어 방 탐색
	for (size_t roomTypeIdx = 0; roomTypeIdx < allRoomTypes.size(); ++roomTypeIdx)
	{
		if (allRoomTypes[roomTypeIdx] == RoomType::Player)
		{
			playerRoomIdx = roomTypeIdx;
			break;
		}
	}

	//예외처리
	if (playerRoomIdx == -1 || playerRoomIdx > allRooms.size())
	{
		Engine::Get().Quit();
		return;
	}
	
	Rect roomInfo = allRooms[playerRoomIdx].GetRoomInfo();
	//스폰
	player = new DGPlayer(Vector2(roomInfo.position.x + roomInfo.width / 2, roomInfo.position.y + roomInfo.height / 2));
	AddActor(player);
}

void DungeonLevel::SpawnEnemies(float deltaTime)
{

}

void DungeonLevel::LoadMap()
{
	map = new char[MAPSIZEX * MAPSIZEY];
	DungeonMaker dungeonSet(map, MAPSIZEX, MAPSIZEY, 0.4f, 0.7f, 3);
	dungeonSet.GetAllRooms(allRooms);
	dungeonSet.GetAllRoomTypes(allRoomTypes);




}

void DungeonLevel::CanMoveActor(int x,int y)
{
	if (map[y * MAPSIZEX + x] == 0)
	{
		return;
	}
	player->Move(x,y);
}
