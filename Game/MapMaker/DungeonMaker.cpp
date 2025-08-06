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
#define MONSTER_ROOMTYPE_RATE 0.5
#define MONSTER_ELITE_ROOMTYPE_RATE 0.334
#define MONSTER_BOSS_ROOMTYPE_COUNT 1
#define TREASURECHEST_ROOMTYPE_RATE 0.167
#define PLAYER_ROOMTYPE_COUNT 1
#define WAY '#'
#define INSIDE '+'
#define OUTSIDE '\0'

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
	map = new char[mapHeight * mapWidth];
	memset(map, 0, sizeof(char) * mapHeight * mapWidth);

	// maxDepth로 방이 몇개 만들어지는지 계산
	// 방 개수만큼 방 타입 지정 및 랜덤 정렬
	SetRandomRoomType(static_cast<int>(std::pow(2, maxDepth)));

	rootNode = new RoomNode(Vector2(MAPBASEPOINTX, MAPBASEPOINTY), inMapWidth, inMapHeight, 0.0f);
	DivideMap(rootNode, STARTDEPTH);
	MakeRoad(rootNode);
	PrintMap();
}

DungeonMaker::DungeonMaker(char*& map, int inMapWidth, int inMapHeight, float inMinRatio, float inMaxRatio, int inMaxDepth)
	: map(map), mapWidth(inMapWidth), mapHeight(inMapHeight), minRatio(inMinRatio), maxRatio(inMaxRatio), maxDepth(inMaxDepth)
{
	memset(map, 0, sizeof(char) * mapHeight * mapWidth);

	// maxDepth로 방이 몇개 만들어지는지 계산
	// 방 개수만큼 방 타입 지정 및 랜덤 정렬
	SetRandomRoomType(static_cast<int>(std::pow(2, maxDepth)));

	rootNode = new RoomNode(Vector2(MAPBASEPOINTX, MAPBASEPOINTY), inMapWidth, inMapHeight, 0.0f);
	DivideMap(rootNode, STARTDEPTH);
	MakeRoad(rootNode);
	//PrintMap();
}

DungeonMaker::~DungeonMaker()
{
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
			map[y * mapWidth + node->rightNode->GetPosition().x] = OUTSIDE;
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
			map[node->rightNode->GetPosition().y* mapWidth + x] = OUTSIDE;
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
		node->room.width =(static_cast<int>(node->GetWidth() * node->GetRatio()));
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
		std::fill(map + h * mapWidth + node->room.position.x, map + h * mapWidth + node->room.position.x + node->room.width - 1, INSIDE);
	}
}

void DungeonMaker::GetAllRoomTypes(std::vector<RoomType>& outRoomTypes)
{
	for (const RoomType roomType : allRoomTypes) {
		outRoomTypes.emplace_back(roomType);
	}
}

void DungeonMaker::GetAllRooms(std::vector<RoomNode>& outRooms)
{
	for (const RoomNode* ptr : allRooms) {
         if (ptr != nullptr) {
			 outRooms.emplace_back(*ptr);
		}
	}
}


//left 노드의 edge와 right노드의 중심을 비교하여 가까운 방향을 시작지점
//길생성 방향과 right노드의 edge 중 가까운 것에 끝지점
//시작과 끝지점을 A*알고리즘으로 이어줌
void DungeonMaker::MakeRoad(const RoomNode* node)
{
	std::vector<char> road;
	road.emplace_back(OUTSIDE);
	road.emplace_back(WAY);

	
	// 4방향 비교
	// 각 중심비교
	for (int roomNum = 0; roomNum < (int)allRooms.size() - 1; ++roomNum)
	{
		std::vector < std::shared_ptr<Node>> path;

		int lCenterY = 0;
		int lCenterX = 0;
		int rCenterY = 0;
		int rCenterX = 0;

		std::vector<std::pair<Vector2, int>> edges;
		Vector2 downLeftEdge(allRooms[roomNum]->room.position.x, allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height);
		edges.push_back({ downLeftEdge , mapWidth * mapHeight });
		Vector2 downRightEdge(allRooms[roomNum]->room.position.x + allRooms[roomNum]->room.width, allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height);
		edges.push_back({ downRightEdge , mapWidth * mapHeight });
		Vector2 upRightEdge(allRooms[roomNum]->room.position.x + allRooms[roomNum]->room.width, allRooms[roomNum]->room.position.y);
		edges.push_back({ upRightEdge , mapWidth * mapHeight });
		Vector2 upLeftEdge(allRooms[roomNum]->room.position.x, allRooms[roomNum]->room.position.y);
		edges.push_back({ upLeftEdge , mapWidth * mapHeight });

		Vector2 minEdge1;
		Vector2 minEdge2;

		for (auto& edge : edges)
		{
			int distance = std::abs(allRooms[roomNum + 1]->room.position.y + allRooms[roomNum + 1]->room.height / 2 - edge.first.y) + std::abs(allRooms[roomNum + 1]->room.position.x + allRooms[roomNum + 1]->room.width / 2 - edge.first.x);
			edge.second = distance;
		}

		std::sort(edges.begin(), edges.end(), &Compare);
		minEdge1 = edges[0].first;
		minEdge2 = edges[1].first;
		// 아래
		if ((minEdge1 == downLeftEdge && minEdge2 == downRightEdge) || (minEdge1 == downRightEdge && minEdge2 == downLeftEdge))
		{
			lCenterY = allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height - 1;
			lCenterX = allRooms[roomNum]->room.position.x + (allRooms[roomNum]->room.width / 2);
		}
		// 위
		if ((minEdge1 == upLeftEdge && minEdge2 == upRightEdge) || (minEdge1 == upRightEdge && minEdge2 == upLeftEdge))
		{
			lCenterY = allRooms[roomNum]->room.position.y;
			lCenterX = allRooms[roomNum]->room.position.x + (allRooms[roomNum]->room.width / 2);
		}
		// 좌
		if ((minEdge1 == downLeftEdge && minEdge2 == upLeftEdge) || (minEdge1 == upLeftEdge && minEdge2 == downLeftEdge))
		{
			lCenterY = allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height / 2;
			lCenterX = allRooms[roomNum]->room.position.x;
		}
		// 우
		if ((minEdge1 == downRightEdge && minEdge2 == upRightEdge) || (minEdge1 == upRightEdge && minEdge2 == downRightEdge))
		{
			lCenterY = allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height / 2;
			lCenterX = allRooms[roomNum]->room.position.x + allRooms[roomNum]->room.width - 1;
		}

		edges.clear();


		Vector2 downLeftEdge2(allRooms[roomNum + 1]->room.position.x, allRooms[roomNum + 1]->room.position.y + allRooms[roomNum + 1]->room.height);
		edges.push_back({ downLeftEdge2, mapWidth * mapHeight });
		Vector2 downRightEdge2(allRooms[roomNum + 1]->room.position.x + allRooms[roomNum + 1]->room.width, allRooms[roomNum + 1]->room.position.y + allRooms[roomNum]->room.height);
		edges.push_back({ downRightEdge2, mapWidth * mapHeight });
		Vector2 upRightEdge2(allRooms[roomNum + 1]->room.position.x + allRooms[roomNum + 1]->room.width, allRooms[roomNum + 1]->room.position.y);
		edges.push_back({upRightEdge2, mapWidth * mapHeight});
		Vector2 upLeftEdge2(allRooms[roomNum + 1]->room.position.x, allRooms[roomNum + 1]->room.position.y);
		edges.push_back({ upLeftEdge2, mapWidth * mapHeight });


		for (auto& edge : edges)
		{
			int distance = std::abs(lCenterY - edge.first.y) + std::abs(lCenterX - edge.first.x);
			edge.second = distance;
		}

		std::sort(edges.begin(), edges.end(), &Compare);
		minEdge1 = edges[0].first;
		minEdge2 = edges[1].first;
		// 아래
		if ((minEdge1 == downLeftEdge2 && minEdge2 == downRightEdge2) || (minEdge1 == downRightEdge2 && minEdge2 == downLeftEdge2))
		{
			rCenterY = allRooms[roomNum+1]->room.position.y + allRooms[roomNum+1]->room.height - 1;
			rCenterX = allRooms[roomNum+1]->room.position.x + (allRooms[roomNum+1]->room.width / 2);
		}
		// 위
		if ((minEdge1 == upLeftEdge2 && minEdge2 == upRightEdge2) || (minEdge1 == upRightEdge2 && minEdge2 == upLeftEdge2))
		{
			rCenterY = allRooms[roomNum+1]->room.position.y;
			rCenterX = allRooms[roomNum+1]->room.position.x + (allRooms[roomNum+1]->room.width / 2);
		}
		// 좌
		if ((minEdge1 == downLeftEdge2 && minEdge2 == upLeftEdge2) || (minEdge1 == upLeftEdge2 && minEdge2 == downLeftEdge2))
		{
			rCenterY = allRooms[roomNum+1]->room.position.y + allRooms[roomNum+1]->room.height / 2;
			rCenterX = allRooms[roomNum+1]->room.position.x;
		}
		// 우
		if ((minEdge1 == downRightEdge2 && minEdge2 == upRightEdge2) || (minEdge1 == upRightEdge2 && minEdge2 == downRightEdge2))
		{
			rCenterY = allRooms[roomNum+1]->room.position.y + allRooms[roomNum+1]->room.height / 2;
			rCenterX = allRooms[roomNum+1]->room.position.x + allRooms[roomNum+1]->room.width - 1;
		}

		map[lCenterY * mapWidth + lCenterX] = WAY;
		map[rCenterY * mapWidth + rCenterX] = WAY;
		AStarPathfinder pathfinder(map, mapWidth, mapHeight, Vector2(lCenterX, lCenterY), Vector2(rCenterX, rCenterY), road, path);
		for (auto& node : path)
		{
			map[node->y * mapWidth + node->x] = WAY;
		}
		path.clear();

		
	}
	


	//for (int roomNum = 0; roomNum < (int)allRooms.size()-1; ++roomNum)
	//{
	//	std::vector < std::shared_ptr<Node>> path;
	//	int lCenterY=0;
	//	int lCenterX=0;
	//	int rCenterY=0;
	//	int rCenterX=0;
	//	// 왼쪽 노드의 중심 좌표와 오른쪽 노드 길 연결
	//	if (node->leftNode->rect.position.x != node->rightNode->rect.position.x)
	//	{
	//		//위 아래
	//		//왼쪽노드 하단 중간
	//		lCenterY = allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height - 1;
	//		lCenterX = allRooms[roomNum]->room.position.x + (allRooms[roomNum]->room.width / 2);
	//		//오른쪽노드 상단 중간
	//		rCenterY = allRooms[roomNum + 1]->room.position.y;
	//		rCenterX = allRooms[roomNum + 1]->room.position.x + (allRooms[roomNum + 1]->room.width / 2);
	//	}
	//	else
	//	{
	//		//좌 우
	//		//왼쪽노드 우측 중간
	//		lCenterY = allRooms[roomNum]->room.position.y + allRooms[roomNum]->room.height / 2;
	//		lCenterX = allRooms[roomNum]->room.position.x + allRooms[roomNum]->room.width - 2;
	//		//오른쪽노드 좌측 중간
	//		rCenterY = allRooms[roomNum + 1]->room.position.y + allRooms[roomNum + 1]->room.height / 2;
	//		rCenterX = allRooms[roomNum + 1]->room.position.x;
	//	}
	//	
	//	map[lCenterY * mapWidth + lCenterX] = 7;
	//	map[rCenterY * mapWidth + rCenterX] = 7;
	//	AStarPathfinder pathfinder(map, mapWidth, mapHeight, Vector2(lCenterX, lCenterY), Vector2(rCenterX, rCenterY), road, path);
	//	for (auto& node : path)
	//	{
	//		map[node->y * mapWidth + node->x] = 7;
	//	}
	//	path.clear();
	//}
	
	
	
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
	int makedRoomCount = 0;
	maxRoomCount -= MONSTER_BOSS_ROOMTYPE_COUNT - PLAYER_ROOMTYPE_COUNT;

	//맵 숫
	allRoomTypes.emplace_back(RoomType::Player);
	++makedRoomCount;
	allRoomTypes.emplace_back(RoomType::MonsterBoss);
	++makedRoomCount;
	for (int treasurechestRoomCount = 0; treasurechestRoomCount < maxRoomCount * TREASURECHEST_ROOMTYPE_RATE; ++treasurechestRoomCount)
	{
		allRoomTypes.emplace_back(RoomType::Treasurechest);
		++makedRoomCount;
	}
	for (int eliteMonsterRoomCount = 0; eliteMonsterRoomCount < maxRoomCount * MONSTER_ELITE_ROOMTYPE_RATE; ++eliteMonsterRoomCount)
	{
		allRoomTypes.emplace_back(RoomType::MonsterElite);
		++makedRoomCount;
	}

	for (int monsterRoomCount = 0; monsterRoomCount < maxRoomCount- makedRoomCount; ++monsterRoomCount)
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

bool Compare(std::pair<Vector2, int> a, std::pair<Vector2, int> b)
{
	return a.second < b.second;
}
