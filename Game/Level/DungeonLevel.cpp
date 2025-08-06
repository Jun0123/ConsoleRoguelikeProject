#include "DungeonLevel.h"
#include "Engine.h"
#include "MapMaker/DungeonMaker.h"
#include "Actors/DGPlayer.h"

#define MAPSIZEX 100
#define MAPSIZEY 50
#define MAPWINDOWPOSITIONX 10
#define MAPWINDOWPOSITIONY 7
#define MAPWINDOWSIZEX 80
#define MAPWINDOWSIZEY 35

DungeonLevel::DungeonLevel()
{
	//맵 생성
	LoadMap();
	//플레이어 스폰
	SpawnPlayer();

	if (player != nullptr)
	{
		player->TestEvent<DungeonLevel, &DungeonLevel::CanMoveActor>(this);
	}
	
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
	//화면 테두리 그리기
	//창 관리할 클래스 생성
	//

	if (map != nullptr)
	{
		mapWindow.Render();

		//for (int y = 0; y < MAPSIZEY; ++y)
		//{
		//	for (int x = 0; x < MAPSIZEX; ++x)
		//	{
		//		Engine::Get().WriteValueToBuffer(Vector2(x, y), (char)map[y * MAPSIZEX + x]);
		//	}
		//}
	}



	super::Render();
}

bool DungeonLevel::CanFindPlayer(DGCharacter*& playerCharacter)
{
	if (player != nullptr)
	{
		playerCharacter = player;
		return true;
	}
	return false;
}

Vector2 DungeonLevel::GetPositionWindowToMap(Vector2 inScreenPosition)
{
	return mapWindow.GetPositionWindowToMap(inScreenPosition);
}

Vector2 DungeonLevel::GetPositionMapToWindow(Vector2 inMapPosition)
{
	return mapWindow.GetPositionMapToWindow(inMapPosition);
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
	if (playerRoomIdx == -1 || playerRoomIdx > allRooms.size()-1)
	{
		Engine::Get().Quit();
		__debugbreak();
		return;
	}
	
	Rect roomInfo = allRooms[playerRoomIdx].GetRoomInfo();

	//게임화면 위치 (왼쪽 상단 엣지)
	Vector2 mapWindowPosition(MAPWINDOWPOSITIONX, MAPWINDOWPOSITIONY);
	//게임화면 사이즈
	Vector2 mapWindowSize(MAPWINDOWSIZEX, MAPWINDOWSIZEY);
	//지도 크기
	Vector2 mapSize(MAPSIZEX, MAPSIZEY);
	//지도 상의 플레이어 위치
	Vector2 playerPositionOnMap(roomInfo.position.x + roomInfo.width / 2, roomInfo.position.y + roomInfo.height / 2);
	
	//카메라 위치를 캐릭터가 가운데로 오게 지정
	Vector2 cameraPosition(playerPositionOnMap.x - MAPWINDOWSIZEX / 2, playerPositionOnMap.y-MAPWINDOWSIZEY / 2);
	//카메라 범위가 맵 범위를 넘어서면 넘지 않게 조정
	if (cameraPosition.x + MAPWINDOWSIZEX >= MAPSIZEX)
	{
		cameraPosition.x = MAPSIZEX - MAPWINDOWSIZEX;
	}
	else if (cameraPosition.x < 0)
	{
		cameraPosition.x = 0;
	}
	if (cameraPosition.y + MAPWINDOWSIZEY >= MAPSIZEY)
	{
		cameraPosition.y = MAPSIZEY - MAPWINDOWSIZEY;
	}
	else if (cameraPosition.y < 0)
	{
		cameraPosition.y = 0;
	}
	//mapWindow 설정
	mapWindow.SetMapWindow(mapWindowPosition, mapWindowSize, cameraPosition, map, mapSize);
	//맵의 플레이어 위치를 화면 위치로 변환하여 스폰
	player = new DGPlayer(mapWindow.GetPositionMapToWindow(playerPositionOnMap));
	AddActor(player);
}

void DungeonLevel::SpawnEnemies()
{
	
}

void DungeonLevel::LoadMap()
{
	map = new char[MAPSIZEX * MAPSIZEY];
	DungeonMaker dungeonSet(map, MAPSIZEX, MAPSIZEY, 0.4f, 0.7f, 3);
	dungeonSet.GetAllRooms(allRooms);
	dungeonSet.GetAllRoomTypes(allRoomTypes);




}

void DungeonLevel::CanMoveActor(int x, int y)
{
	// 스크린화면 좌표로 비교할때
	Vector2 moveToPosition(x, y);
	Vector2 dir = moveToPosition - player->Position();

	//스크린 좌표를 지도 좌표로 변환
	Vector2 playerMovePosition = mapWindow.GetPositionWindowToMap(moveToPosition);

	// 지도로 좌표 비교
	if (map[playerMovePosition.y * MAPSIZEX + playerMovePosition.x] == 0)
	{
		return;
	}

	Vector2 windowPosition = mapWindow.GetWindowPosition();
	Vector2 windowSize = mapWindow.GetWindowSize();
	Vector2 cameraPosition = mapWindow.imagePosition + dir;

	//이동한 위치의 카메라 범위가 지도를 벗어난다면 화면상에서 캐릭터 이동
	if ((dir.x > 0 && cameraPosition.x + MAPWINDOWSIZEX >= MAPSIZEX) ||
		(dir.x < 0 && cameraPosition.x < 0) ||
		(dir.y > 0 && cameraPosition.y + MAPWINDOWSIZEY >= MAPSIZEY) ||
		(dir.y < 0 && cameraPosition.y < 0)
		)
	{
		//플레이어만 이동
		player->Move(x, y);
	}
	else
	{
		// 오른쪽 이동할때 카메라 중심을 기점으로 왼쪽에 있으면 캐릭터 이동 오른쪽에 있으면 화면 이동
		if ((dir.x > 0 && playerMovePosition.x < cameraPosition.x + windowSize.x / 2 + 1) ||
			(dir.x < 0 && playerMovePosition.x > cameraPosition.x + windowSize.x / 2 - 1) ||
			(dir.y > 0 && playerMovePosition.y < cameraPosition.y + windowSize.y / 2 + 1) ||
			(dir.y < 0 && playerMovePosition.y > cameraPosition.y + windowSize.y / 2 - 1)
			)
		{
			//플레이어만 이동
			player->Move(x, y);
		}
		else 
		{
			//카메라 이동
			mapWindow.MoveCamera(dir);
		}
	}
}


