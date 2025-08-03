#pragma once
#include "RoomNode.h"

class TreasurechestRoom : public RoomNode
{
    friend class DungeonMaker;
public:
    TreasurechestRoom();
    TreasurechestRoom(Vector2 inPosition, int inWidth, int inHeight, float inRato);
    ~TreasurechestRoom() {}

protected:
    virtual void SetActors() override;
};

