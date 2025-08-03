#pragma once
#include "RoomNode.h"
class PlayerRoom : public RoomNode
{
    friend class DungeonMaker;
public:
    PlayerRoom();
    PlayerRoom(Vector2 inPosition, int inWidth, int inHeight, float inRato);
    ~PlayerRoom() {}

protected:
    virtual void SetActors() override;
};

