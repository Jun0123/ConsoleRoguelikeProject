#include "PlayerRoom.h"

PlayerRoom::PlayerRoom()
	: RoomNode(Vector2(0, 0), 0, 0, 0.f)
{
}

PlayerRoom::PlayerRoom(Vector2 inPosition, int inWidth, int inHeight, float inRato)
	:RoomNode(inPosition, inWidth, inHeight, inRato)
{
}

void PlayerRoom::SetActors()
{
}
