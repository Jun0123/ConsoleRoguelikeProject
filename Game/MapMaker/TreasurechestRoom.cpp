#include "TreasurechestRoom.h"

TreasurechestRoom::TreasurechestRoom()
	: RoomNode(Vector2(0,0),0,0,0.f)
{
}

TreasurechestRoom::TreasurechestRoom(Vector2 inPosition, int inWidth, int inHeight, float inRatio)
	: RoomNode(inPosition, inWidth, inHeight, inRatio)
{
}

void TreasurechestRoom::SetActors()
{
}


