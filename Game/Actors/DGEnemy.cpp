#include "DGEnemy.h"
#include "Level/DungeonLevel.h"

DGEnemy::DGEnemy(const char* image, Color color, const Vector2& position)
	: DGCharacter(image,color,position)
{
	currentState = EnemyState::Idle;
}

void DGEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void DGEnemy::SetState(EnemyState inState)
{
	switch (inState)
	{
	case EnemyState::Idle:
		Idle();
		break;
	case EnemyState::RandomMove:
		RandomMove();
		break;
	case EnemyState::TrackingTarget:
		TrackingTarget();
		break;
	case EnemyState::AttackTarget:
		AttackTarget();
		break;
	}
}

void DGEnemy::Idle()
{

}

void DGEnemy::RandomMove()
{
}

void DGEnemy::FindTarget()
{
	if (target != nullptr)
	{
		return;
	}
	GetDistanceToTarget();
}

void DGEnemy::TrackingTarget()
{
}

void DGEnemy::AttackTarget()
{

}
//플레이어와 거리가 멀다면 비활성화 
void DGEnemy::GetDistanceToTarget()
{
	Vector2 playerWindowPosition(-1, -1);
	ITargetable* targetable = dynamic_cast<ITargetable*>(owner);
	if (targetable->CanFindPlayer(target))
	{
		
	}
}

void DGEnemy::UseTurn()
{
	while (!bTurnEnd)//(target != nullptr && attackCurrentCount) || moveCurrentCount > 0)
	{
		if (!isActive)
		{
			// Todo : level에서 턴시작할때 bTurnEnd = false로 바꿔주기
			bTurnEnd = true;
			return;
		}
		// 1순위 : 타겟 탐색
		// 2순위 : 타겟 공격
		// 3순위 : 타겟 추격
		// 4순위 : 이동 할지 가만히 있을지 랜덤 결정
		FindTarget();

	}
}
