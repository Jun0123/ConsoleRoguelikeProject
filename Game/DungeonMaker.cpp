#include "DungeonMaker.h"
#include "Utils/Utils.h"

DungeonMaker::DungeonMaker(int inMapWidth, int inMapHeight, float inMinRatio, float inMaxRatio, int inMaxDepth)
	: mapWidth(inMapWidth), mapHeight(inMapHeight), minRatio(inMinRatio), maxRatio(inMaxRatio), maxDepth(inMaxDepth)
{

}

void DungeonMaker::DivideMap(RoomNode* node, int depth)
{
	if (depth == maxDepth)
	{
		return;
	}
	// 최대 길이를 구해서 나눌 기준을 정한다.
	int maxLen = node->GetWidth() > node->GetHeight() ? node->GetWidth() : node->GetHeight();
	int split = Utils::RandomFloat(maxLen * minRatio, maxLen * maxRatio);

	if (node->GetHeight() >= static_cast<int>(split))
	{

	}
	else if (node->GetWidth() >= static_cast<int>(split))
	{

	}
	

}

void DungeonMaker::PrintMap()
{
}
