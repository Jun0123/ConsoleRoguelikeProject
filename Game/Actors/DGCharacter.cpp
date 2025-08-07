#include "DGCharacter.h"

DGCharacter::DGCharacter()
{
}

DGCharacter::DGCharacter(const char* image, Color color, const Vector2& position, const Vector2& mapPosition)
	:Actor(image,color,position),mapPosition(mapPosition)
{

}

void DGCharacter::TakeDamage(DGCharacter* attacker, int damage)
{
	ability.currentHp = max(0, ability.currentHp - damage);
}

void DGCharacter::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void DGCharacter::ReadyTurn()
{
	ability.attackCurrentCount = 0;
	ability.moveCurrentCount = 0;
}

void DGCharacter::UseTurn()
{
	ability.bTurnEnd = false;
}

DGCharacterAbility::DGCharacterAbility(int moveMaxCount, int moveCurrentCount,int attackCurrentCount, int attackDistance, int attackMaxCount, int attackDamage, int maxDefensive, int currentDefensive, int maxHp, int currentHp, int maxMP, int currentMP, int currentTurnPoint, int sortTurnPoint, bool bTurnEnd)
	:moveCurrentCount(moveCurrentCount),attackDistance(attackDistance),attackCurrentCount(attackCurrentCount), attackMaxCount(attackMaxCount), attackDamage(attackDamage), moveMaxCount(moveMaxCount), maxDefensive(maxDefensive), sortTurnPoint(sortTurnPoint),
	currentDefensive(currentDefensive), maxHp(maxHp), currentHp(currentHp), maxMP(maxMP), currentMP(currentMP), currentTurnPoint(currentTurnPoint), bTurnEnd(bTurnEnd)
{

}

DGCharacterAbility& DGCharacterAbility::operator=(const DGCharacterAbility ability)
{
	attackCurrentCount = ability.attackCurrentCount;
	attackMaxCount = ability.attackMaxCount;
	attackDamage = ability.attackDamage;
	moveMaxCount = ability.moveMaxCount;
	maxDefensive = ability.maxDefensive;
	currentDefensive = ability.currentDefensive;
	maxHp = ability.maxHp;
	currentHp = ability.currentHp;
	maxMP = ability.maxMP;
	currentMP = ability.currentMP;
	currentTurnPoint = ability.currentTurnPoint;
	bTurnEnd = ability.bTurnEnd;
	attackDistance = ability.attackDistance;
	sortTurnPoint = ability.sortTurnPoint;
	return *this;
}