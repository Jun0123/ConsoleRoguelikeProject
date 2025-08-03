#include "MonsterRoom.h"

MonsterRoom::MonsterRoom()
	: RoomNode(Vector2(0, 0), 0, 0, 0.f)
{

}

MonsterRoom::MonsterRoom(Vector2 inPosition, int inWidth, int inHeight, float inRato)
	:RoomNode(inPosition, inWidth, inHeight, inRato)
{

}

void MonsterRoom::SetActors()
{
}
