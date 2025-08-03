#pragma once
#include "RoomNode.h"
#include <vector>

/*
	 https://ajdxjdrnfld.tistory.com/4
	 https://nowitzki.tistory.com/10 
*/



enum class Direction
{
	Left = 0,
	Right,
	Up,
	Down
};

struct DirectionMap
{
	Direction arr[4] = { Direction::Left,Direction::Right,Direction::Up,Direction::Down };
};


class DungeonMaker
{
public:
	// 맵 크기 , 방 분할 최소/최대 비율
	DungeonMaker(int inMapWidth, int inMapHeight, float inMinRatio, float inMaxRatio, int inMaxDepth);
	~DungeonMaker();
private:
	// 맵 분할 함수 : 구역 나누기
	void DivideMap(RoomNode* node, int depth);
	void MakeRoomTypeRightNode(RoomNode*& node, Vector2 inPosition, int inWidth, int inHeight, float inRatio);
	void MakeRoomTypeLeftNode(RoomNode*& node, Vector2 inPosition, int inWidth, int inHeight, float inRatio);
	// 맵 출력
	void PrintMap();

	//방만들기
	void MakeRoom(RoomNode* node);

	//길만들기
	// 
	void MakeRoad();
	void MakeHorizontalRoad(const RoomNode* node);
	void MakeVerticalRoad(const RoomNode* node);

	//방 타입 랜덤지정
	void SetRandomRoomType(int maxRoomCount);
	//방 타입 생성
	
private:
	//분할 최소 비율
	float minRatio = 0.0f;
	//분할 최대 비율
	float maxRatio = 0.0f;
	int mapWidth;
	int mapHeight;
	int maxDepth = 0;
	int* map = nullptr;
	RoomNode* rootNode = nullptr;
	int roomIdx = 0;
	std::vector<RoomType> allRoomTypes;
	std::vector<RoomNode*> allRooms;

};

