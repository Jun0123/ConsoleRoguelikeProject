#pragma once
#include "RoomNode.h"

/*
	 https://ajdxjdrnfld.tistory.com/4
	 https://nowitzki.tistory.com/10 
*/

class DungeonMaker
{
public:
	// 맵 크기 , 방 분할 최소/최대 비율
	DungeonMaker(int inMapWidth, int inMapHeight, float inMinRatio, float inMaxRatio, int inMaxDepth);

private:
	// 맵 분할 함수 : 방생성
	void DivideMap(RoomNode* node, int depth);
	// 맵 출력
	void PrintMap();

private:
	//분할 최소 비율
	float minRatio = 0;
	//분할 최대 비율
	float maxRatio = 0;
	int mapWidth;
	int mapHeight;
	int maxDepth = 0;
	int* map = nullptr;
};

