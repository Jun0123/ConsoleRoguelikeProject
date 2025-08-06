#pragma once

#include "Actor/Actor.h"

class DGCharacter : public Actor
{
	RTTI_DECLARATIONS(DGCharacter, Actor)

public:
	DGCharacter();
	DGCharacter(
		const char* image = "",
		Color color = Color::White,
		const Vector2& position = Vector2::Zero
	);
	virtual void Tick(float deltaTime) override;
protected:
	int attackCurrentCount = 0;
	int attackMaxCount=0;
	int attackDamage=0;
	int moveCurrentCount=0;
	int maxDefensive=0;
	int currentDefensive=0;
	int maxHp=0;
	int currentHp=0;
	int maxMP=0;
	int currentMP=0;
	int currentTurnPoint=0;
	bool bTurnEnd;
};