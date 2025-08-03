#pragma once
#include "RoomNode.h"

class MonsterRoom : public RoomNode
{
    friend class DungeonMaker;
public:
    MonsterRoom();
    MonsterRoom(Vector2 inPosition, int inWidth, int inHeight, float inRato);
    ~MonsterRoom() {}

protected:
    virtual void SetActors() override;
};

