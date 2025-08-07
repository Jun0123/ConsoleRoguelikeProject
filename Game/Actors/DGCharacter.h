#pragma once

#include "Actor/Actor.h"

struct DGCharacterAbility
{

	DGCharacterAbility(
		int moveMaxCount = 0,
		int moveCurrentCount = 0,
		int attackCurrentCount = 0,
		int attackDistance= 0,
		int attackMaxCount = 0,
		int attackDamage = 0,
		int maxDefensive = 0,
		int currentDefensive = 0,
		int maxHp = 0,
		int currentHp = 0,
		int maxMP = 0,
		int currentMP = 0,
		int currentTurnPoint = 0,
		int sortTurnPoint = 0,
		bool bTurnEnd = false
	);


	DGCharacterAbility& operator=(const DGCharacterAbility ability);
	int moveCurrentCount = 0;
	int attackDistance = 0;
	int attackCurrentCount = 0;
	int attackMaxCount = 0;
	int attackDamage = 0;
	int moveMaxCount = 0;
	int maxDefensive = 0;
	int currentDefensive = 0;
	int maxHp = 0;
	int currentHp = 0;
	int maxMP = 0;
	int currentMP = 0;
	int currentTurnPoint = 0;
	int sortTurnPoint = 0;
	bool bTurnEnd;
};

class DGCharacter : public Actor
{
	friend class DungeonLevel;

	RTTI_DECLARATIONS(DGCharacter, Actor)

public:
	DGCharacter();
	DGCharacter(
		const char* image = "",
		Color color = Color::White,
		const Vector2& position = Vector2::Zero, const Vector2& mapPosition = Vector2::Zero
	);
	virtual void TakeDamage(DGCharacter* attacker, int damage);
	virtual void Tick(float deltaTime) override;
	virtual void ReadyTurn();
	virtual void UseTurn();
protected:
	DGCharacterAbility ability;
	Vector2 mapPosition;
};