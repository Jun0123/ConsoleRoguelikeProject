#include "DungeonMaker.h"
#include "Utils/Utils.h"
#include "MonsterRoom.h"
#include "MonsterElite.h"
#include "MonsterBossRoom.h"
#include "TreasurechestRoom.h"
#include "PlayerRoom.h"
#include "Utils/AStarPathfinder.h"

#include <iostream>
#include <bits.h>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <memory>

#define MAPBASEPOINTX 0
#define MAPBASEPOINTY 0
#define STARTDEPTH 0
#define MONSTER_ELITE_ROOMTYPE_COUNT 5
#define MONSTER_BOSS_ROOMTYPE_COUNT 1
#define TREASURECHEST_ROOMTYPE_COUNT 3
#define PLAYER_ROOMTYPE_COUNT 1

/*
	Todo : 맵생성
	1. 방만들기 v
	2. 길만들기 v
	3. 벽만들기
	3. 맵에 관련된 오브젝트로 변환하기
	4. 랜덤으로 아이템, 적 스폰 하기
*/

DungeonMaker::DungeonMaker(int inMapWidth, int inMapHeight, float inMinRatio, float inMaxRatio, int inMaxDepth)
	: mapWidth(inMapWidth), mapHeight(inMapHeight), minRatio(inMinRatio), maxRatio(inMaxRatio), maxDepth(inMaxDepth)
{
	map = new int[mapHeight * mapWidth];
	memset(map, 0, sizeof(int) * mapHeight * mapWidth);

	// maxDepth로 방이 몇개 만들어지는지 계산
	// 방 개수만큼 방 타입 지정 및 랜덤 정렬
	SetRandomRoomType(static_cast<int>(std::pow(2, maxDepth)));

	rootNode = new RoomNode(Vector2(MAPBASEPOINTX, MAPBASEPOINTY), inMapWidth, inMapHeight, 0.0f);
	DivideMap(rootNode, STARTDEPTH);
	MakeRoad(rootNode);
	PrintMap();
}

DungeonMaker::~DungeonMaker()
{
	if (nullptr != map)
	{
		delete[] map;
		map = nullptr;
	}

	if (nullptr != rootNode)
	{
		delete rootNode;
		rootNode = nullptr;
	}

	allRoomTypes.clear();
	allRooms.clear();
}

void DungeonMaker::DivideMap(RoomNode* node, int depth)
{
	// 제일 깊은 노드에서 방만들기
	if (depth >= maxDepth)
	{
		MakeRoom(node);
		return;
	}

	// 최대 길이를 구해서 나눌 기준을 정한다.
	int maxLen = node->GetWidth() > node->GetHeight() ? node->GetWidth() : node->GetHeight();
	int split = static_cast<int>(Utils::RandomFloat(maxLen * minRatio, maxLen * maxRatio));


	// 너비가 높이보다 값이 크다면 좌/우로 분할한다.
	if (node->GetWidth() > node->GetHeight())
	{
		if (depth == maxDepth - 1)
		{
			// Depth가 제일 높은 노드에서 방 타입에 맞춰서 방생성
			MakeRoomTypeLeftNode(node, Vector2(node->GetPosition().x, node->GetPosition().y), split, node->GetHeight(), 1 - static_cast<float>(split) / maxLen);
			MakeRoomTypeRightNode(node,Vector2(node->GetPosition().x + split, node->GetPosition().y), node->GetWidth() - split, node->GetHeight(), 1 - static_cast<float>(split) / maxLen);
		}
		else
		{
			node->leftNode = new RoomNode(Vector2(node->GetPosition().x, node->GetPosition().y), split, node->GetHeight(), 1 - static_cast<float>(split) / maxLen);
			node->rightNode = new RoomNode(Vector2(node->GetPosition().x + split, node->GetPosition().y), node->GetWidth() - split, node->GetHeight(), 1 - static_cast<float>(split) / maxLen);
		}
		

		for (int y = node->GetPosition().y; y < node->GetPosition().y + node->GetHeight(); ++y)
		{
			// 1차원 배열  ->  2차원 배열 인덱스 구하기  (세로 좌표 * mapWidth) + 가로 좌표
			map[y * mapWidth + node->rightNode->GetPosition().x] = 1;
		}
	}
	// 높이가 너비보다 값이 크다면 위/아래로 분할한다.
	else
	{
		if (depth == maxDepth - 1)
		{
			// Depth가 제일 높은 노드에서 방 타입에 맞춰서 방생성
			MakeRoomTypeLeftNode(node, Vector2(node->GetPosition().x, node->GetPosition().y), node->GetWidth(), split, 1 - static_cast<float>(split) / maxLen);
			MakeRoomTypeRightNode(node, Vector2(node->GetPosition().x, node->GetPosition().y + split), node->GetWidth(), node->GetHeight() - split, 1 - static_cast<float>(split) / maxLen);
		}	
		else
		{
			node->leftNode = new RoomNode(Vector2(node->GetPosition().x, node->GetPosition().y), node->GetWidth(), split, 1 - static_cast<float>(split) / maxLen);
			node->rightNode = new RoomNode(Vector2(node->GetPosition().x, node->GetPosition().y + split), node->GetWidth(), node->GetHeight() - split, 1 - static_cast<float>(split) / maxLen);
		}
	

		for (int x = node->leftNode->GetPosition().x; x < node->leftNode->GetPosition().x + node->leftNode->GetWidth(); ++x)
		{
			map[node->rightNode->GetPosition().y* mapWidth + x] = 1;
		}
	}

	//분할한 노드를 이어서 분할
	DivideMap(node->leftNode, depth + 1);
	DivideMap(node->rightNode, depth + 1);

	//// 왼쪽 노드의 중심 좌표와 오른쪽 노드 길 연결
	//if (node->leftNode->rect.position.x == node->rightNode->rect.position.x)
	//{
	//	//위 아래
	//	MakeVerticalRoad(node);
	//}
	//else
	//{
	//	//좌 우
	//	MakeHorizontalRoad(node);
	//}

	////모든 방이 연결되게 만들기 위해 자신의 Rect 정보를 자식의 Rect 정보로 대입
	//int randomChild = Utils::Random(0, 1);
	//randomChild == 0 ? node->room = node->leftNode->room : node->room = node->rightNode->room;
}

//방만들기
//마지막 깊이라면 해당 노드 안에 방만들기
//방의 가로 세로 길이를 넘지 않고 랜덤으로 생성
void DungeonMaker::MakeRoom(RoomNode* node)
{
	int tempWidth = node->GetWidth();
	int tempHeight= node->GetHeight();

	//작은 Rect 방은 크게, 큰 Rect 방은 작게 길이 지정
	if (node->GetRatio() > 0.5)
	{
		node->room.width= (static_cast<int>(node->GetWidth() * node->GetRatio()));
		node->room.height=(static_cast<int>(node->GetHeight() * node->GetRatio()));
	}
	else
	{
		node->room.width= (static_cast<int>(node->GetWidth() * node->GetRatio() * Utils::RandomFloat(1.5f, 1.7f)));
		node->room.height=(static_cast<int>(node->GetHeight() * node->GetRatio() * Utils::RandomFloat(1.5f, 1.7f)));
	}
	//예외 처리
	// 길이가 충분하지 않다면 기존 위치 값으로 진행
	if (tempWidth <= node->room.width + 2 || tempHeight <= node->room.height + 2)
	{
		node->room.width= tempWidth;
		node->room.height=tempHeight;
		node->room.position = node->rect.position;
	}
	else
	{
		// 인덱스값을 랜덤으로 지정
		// 최대 인덱스 = 마지막 인덱스 - 길이 - 1
		node->room.position = (Vector2(
			Utils::Random(node->GetPosition().x + 1, node->GetPosition().x + tempWidth - node->room.width) ,
			Utils::Random(node->GetPosition().y + 1, node->GetPosition().y + tempHeight - node->room.height)
		));
	}
	//벽 그리기
	
	//방 크기만큼 그리기
	for (int h = node->room.position.y; h < node->room.position.y + node->room.height; ++h)
	{
		std::fill(map + h * mapWidth + node->room.position.x, map + h * mapWidth + node->room.position.x + node->room.width - 1, 3);
	}
}



void DungeonMaker::MakeRoad(const RoomNode* node)
{
	/* Todo :
	문제 1 : 상위 노드에서 길을 만들 경우 중간에 방을 뚫고 길을 만든다.
        A*알고리즘->player추적에도 사용할 것이므로 재활용할 수 있게 만들기
        반목문 - allRooms 다음 idx를 A*알고리즘으로 길 연결
	문제 2: 보스Room을 안거치고 모든 길을 어떻게 연결하는가?
        방법 1 :1 - 2 - 3 - 4 - 5 - 6 과 같이 일자형으로 구현하는 방법, 6에 보스방
        방법 2 : ex) 보스가 i번방이라면 i-1과 i+1을 연결
	*/

	std::vector<char> road;
	road.emplace_back(7);
	road.emplace_back(0);
	road.emplace_back(1);

	for (int roomNum = 0; roomNum < (int)allRooms.size()-1; ++roomNum)
	{
		std::vector<Node*> path;
		int lCenterY=0;
		int lCenterX=0;
		int rCenterY=0;
		int rCenterX=0;
		// 왼쪽 노드의 중심 좌표와 오른쪽 노드 길 연결
		if (node->leftNode->rect.position.x != node->rightNode->rect.position.x)
		{
			//위 아래
			//왼쪽노드 하단 중간
			lCenterY = allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height - 1;
			lCenterX = allRooms[roomNum]->room.position.x + (allRooms[roomNum]->room.width / 2);
			//오른쪽노드 상단 중간
			rCenterY = allRooms[roomNum + 1]->room.position.y;
			rCenterX = allRooms[roomNum + 1]->room.position.x + (allRooms[roomNum + 1]->room.width / 2);
		}
		else
		{
			//좌 우
			//왼쪽노드 우측 중간
			lCenterY = allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height / 2;
			lCenterX = allRooms[roomNum]->room.position.x + allRooms[roomNum]->room.width - 2;
			//오른쪽노드 좌측 중간
			rCenterY = allRooms[roomNum + 1]->room.position.y + allRooms[roomNum + 1]->room.height / 2;
			rCenterX = allRooms[roomNum + 1]->room.position.x;
		}
		
		map[lCenterY * mapWidth + lCenterX] = 7;
		map[rCenterY * mapWidth + rCenterX] = 7;
		AStarPathfinder pathfinder(map, mapWidth, mapHeight, Vector2(lCenterX, lCenterY), Vector2(rCenterX, rCenterY), road, path);
		for (Node*& node : path)
		{
			map[node->y * mapWidth + node->x] = 7;

			delete node;
			node = nullptr;
		}
		path.clear();
	}
	
	
	
}

void DungeonMaker::MakeHorizontalRoad(const RoomNode* node)
{
	//Todo : 문설치 - lCenterX-2 lCenterY 

	//왼쪽노드 우측 중간
	int lCenterY = node->leftNode->room.position.y + node->leftNode->room.height/2;
	int lCenterX = node->leftNode->room.position.x + node->leftNode->room.width - 2;
	//오른쪽노드 좌측 중간
	int rCenterY = node->rightNode->room.position.y + node->rightNode->room.height / 2;
	int rCenterX = node->rightNode->room.position.x;
	//중간값
	int middleX = node->leftNode->rect.position.x + node->leftNode->rect.width;//lCenterX + static_cast<int>((rCenterX - lCenterX) * node->currentRatio);
	//우측 이동
	for (int x = lCenterX; x <= middleX; ++x)
		map[lCenterY * mapWidth + x] = 7;

	int from = lCenterY <= rCenterY ? lCenterY : rCenterY;
	int to = lCenterY <= rCenterY ? rCenterY : lCenterY;
	//위/아래 이동
	for (int y = from; y <= to; ++y)
		map[y * mapWidth + middleX] = 7;
	//우측 이어서 이동
	for (int x = middleX; x <= rCenterX; ++x)
		map[rCenterY * mapWidth + x] = 7;
}

void DungeonMaker::MakeVerticalRoad(const RoomNode* node)
{
	//Todo : 문설치 - lCenterX lCenterY-1

	//왼쪽노드 하단 중간
	int lCenterY = node->leftNode->room.position.y + node->leftNode->room.height - 1;
	int lCenterX = node->leftNode->room.position.x + (node->leftNode->room.width / 2);
	//오른쪽노드 상단 중간
	int rCenterY = node->rightNode->room.position.y;
	int rCenterX = node->rightNode->room.position.x + (node->rightNode->room.width / 2);
	//중간에 거쳐야 할 위치
	int middleY = node->leftNode->rect.position.y + node->leftNode->rect.height;//lCenterY + static_cast<int>((rCenterY - lCenterY) * node->currentRatio);

	//아래 이동
	for (int y = lCenterY; y < middleY; ++y)
		map[y * mapWidth + lCenterX] = 7;

	int from = lCenterX <= rCenterX ? lCenterX : rCenterX;
	int to = lCenterX <= rCenterX ? rCenterX : lCenterX;
	//좌/우 이동
	for (int x = from; x <= to; x++)
		map[middleY * mapWidth + x] = 7;

	//이어서 아래 이동
	for (int y = middleY; y <= rCenterY; ++y)
		map[y * mapWidth + rCenterX] = 7;

	

	
}
//방의 타입을 정해진 값에 따라 생성 후 랜덤 정렬
void DungeonMaker::SetRandomRoomType(int maxRoomCount)
{
	// Todo : 비율로 수정해야함

	//맵 숫
	allRoomTypes.emplace_back(RoomType::Player);
	allRoomTypes.emplace_back(RoomType::MonsterBoss);
	for (int eliteMonsterRoomCount = 0; eliteMonsterRoomCount < MONSTER_ELITE_ROOMTYPE_COUNT; ++eliteMonsterRoomCount)
	{
		allRoomTypes.emplace_back(RoomType::MonsterElite);
	}
	for (int treasurechestRoomCount = 0; treasurechestRoomCount < TREASURECHEST_ROOMTYPE_COUNT; ++treasurechestRoomCount)
	{
		allRoomTypes.emplace_back(RoomType::Treasurechest);
	}
	for (int monsterRoomCount = 0; monsterRoomCount < maxRoomCount - MONSTER_ELITE_ROOMTYPE_COUNT - TREASURECHEST_ROOMTYPE_COUNT - MONSTER_BOSS_ROOMTYPE_COUNT - PLAYER_ROOMTYPE_COUNT; ++monsterRoomCount)
	{
		allRoomTypes.emplace_back(RoomType::Monster);
	}
	//랜덤정렬
	std::mt19937 generator(static_cast<unsigned int>(time(nullptr)));
	std::shuffle(allRoomTypes.begin(), allRoomTypes.end(), generator);
}

void DungeonMaker::MakeRoomTypeLeftNode(RoomNode*& node, Vector2 inPosition, int inWidth, int inHeight, float inRatio)
{
	switch (allRoomTypes[roomIdx])
	{
	case RoomType::Monster:
		node->leftNode = 
			new MonsterRoom(inPosition, inWidth, inHeight, inRatio);
		break;
	case RoomType::MonsterElite:
		node->leftNode =
			new MonsterElite(inPosition, inWidth, inHeight, inRatio);
		break;
	case RoomType::MonsterBoss:
		node->leftNode =
			new MonsterBossRoom(inPosition, inWidth, inHeight, inRatio);

		break;
	case RoomType::Treasurechest:
		node->leftNode =
			new TreasurechestRoom(inPosition, inWidth, inHeight, inRatio);
		break;
	case RoomType::Player:
		node->leftNode =
			new PlayerRoom(inPosition, inWidth, inHeight, inRatio);
		break;
	}

	allRooms.emplace_back(node->leftNode);
	++roomIdx;
}
void DungeonMaker::MakeRoomTypeRightNode(RoomNode*& node, Vector2 inPosition, int inWidth, int inHeight, float inRatio)
{
	switch (allRoomTypes[roomIdx])
	{
	case RoomType::Monster:
		node->rightNode = 
			new MonsterRoom(inPosition, inWidth, inHeight, inRatio);
		break;
	case RoomType::MonsterElite:
		node->rightNode = 
			new MonsterElite(inPosition, inWidth, inHeight, inRatio);
		break;
	case RoomType::MonsterBoss:
		node->rightNode = 
			new MonsterBossRoom(inPosition, inWidth, inHeight, inRatio);
		break;
	case RoomType::Treasurechest:
		node->rightNode = 
			new TreasurechestRoom(inPosition, inWidth, inHeight, inRatio);
		break;
	case RoomType::Player:
		node->rightNode = 
			new PlayerRoom(inPosition, inWidth, inHeight, inRatio);
		break;
	}
	allRooms.emplace_back(node->rightNode);
	++roomIdx;
}

void DungeonMaker::PrintMap()
{
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			// int index = (position.y * (settings.width)) + position.x + ix;
			std::cout << map[(i * mapWidth)+j];
			//printf("%c", map[(i * mapWidth) + j]);
		}
		std::cout << '\n';
	}
}


