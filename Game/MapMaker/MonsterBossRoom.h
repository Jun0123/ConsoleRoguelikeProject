#pragma once
#include "RoomNode.h"
class MonsterBossRoom :
    public RoomNode
{
    friend class DungeonMaker;
public:
    MonsterBossRoom();
    MonsterBossRoom(Vector2 inPosition, int inWidth, int inHeight, float inRato);

    ~MonsterBossRoom();

protected:
    virtual void SetActors() override;
};

