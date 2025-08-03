#include "MonsterBossRoom.h"

MonsterBossRoom::MonsterBossRoom()
	: RoomNode(Vector2(0, 0), 0, 0, 0.f)
{
}

MonsterBossRoom::MonsterBossRoom(Vector2 inPosition, int inWidth, int inHeight, float inRato)
	:RoomNode(inPosition, inWidth, inHeight, inRato)
{
}

MonsterBossRoom::~MonsterBossRoom()
{
}

void MonsterBossRoom::SetActors()
{
}
