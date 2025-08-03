#include "MonsterElite.h"
#include <iostream>
MonsterElite::MonsterElite()
	: RoomNode(Vector2(0, 0), 0, 0, 0.f)
{

}

MonsterElite::MonsterElite(Vector2 inPosition, int inWidth, int inHeight, float inRato)
	:RoomNode(inPosition, inWidth, inHeight, inRato)
{

}

MonsterElite::~MonsterElite()
{

}

void MonsterElite::SetActors()
{
}
