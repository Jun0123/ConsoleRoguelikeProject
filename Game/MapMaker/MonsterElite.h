#pragma once
#include "RoomNode.h"
class MonsterElite :public RoomNode
{
    friend class DungeonMaker;
public:
    MonsterElite();
    MonsterElite(Vector2 inPosition, int inWidth, int inHeight, float inRato);
    ~MonsterElite();

protected:
    virtual void SetActors() override;
};

