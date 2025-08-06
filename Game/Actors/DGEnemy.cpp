#include "DGEnemy.h"
#include "Level/DungeonLevel.h"

DGEnemy::DGEnemy(const char* image, Color color, const Vector2& windowPosition, const Vector2& mapPosition, int findTargetDistance,int attackDistance)
	: DGCharacter(image,color, windowPosition), mapPosition(mapPosition),findTargetDistance(findTargetDistance), attackDistance(attackDistance)
{
	currentState = EnemyState::Idle;
}

void DGEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	TrackingTarget();
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

bool DGEnemy::FindTarget()
{
	if (target != nullptr)
	{
		return false;
	}
	int distance = GetDistanceToTarget();

	if (distance > findTargetDistance )
	{
		return false;
	}

	return true;
}

void DGEnemy::TrackingTarget()
{
	if (GetDistanceToTarget() <= attackDistance)
	{
		SetState(EnemyState::AttackTarget);
		return;
	}

	ITargetable* targetable = dynamic_cast<ITargetable*>(owner);
	if (targetable != nullptr && targetable->CanFindPlayer(target))
	{
		std::vector<Vector2> path = targetable->GetFindPathToTarget(mapPosition, target);

	}
}

void DGEnemy::AttackTarget()
{

}
//플레이어와 거리가 멀다면 비활성화 
int DGEnemy::GetDistanceToTarget()
{
	Vector2 playerWindowPosition(-1, -1);
	ITargetable* targetable = dynamic_cast<ITargetable*>(owner);
	if (targetable != nullptr && targetable->CanFindPlayer(target))
	{
		Vector2 playerMapPosition = targetable->GetPositionWindowToMap(target->Position());
		return std::abs(playerMapPosition.x - mapPosition.x) + std::abs(playerMapPosition.y - mapPosition.y);
	}

	return findTargetDistance + 1;
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
		if (FindTarget())
		{
			SetState(EnemyState::TrackingTarget);
		}

	}
}
