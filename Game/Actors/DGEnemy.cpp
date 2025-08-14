#include "DGEnemy.h"
#include "Level/DungeonLevel.h"
#include "Utils/Utils.h"

DGEnemy::DGEnemy(const char* image, Color color, const Vector2& windowPosition, const Vector2& mapPosition, DGCharacterAbility ability, DungeonLevel* owner)
	: DGCharacter(image, color, windowPosition,mapPosition)
{
	currentState = EnemyState::Idle;
	this->ability = ability;
	target = nullptr;
	this->owner = owner;
	bOnAttack = false;
	findTargetDistance = 10;
	bRandomMove = false;

	attackTimer.SetTargetTime(0.2f);
	moveTimer.SetTargetTime(0.3f);
	isVisible = false;
}

void DGEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	
	
	//카메라 안에 있으면 활성화 //맵 위치를 window위치로 저장
	ITargetable* targetable = dynamic_cast<ITargetable*>(owner);
	if (targetable != nullptr && targetable->InCameraEnemy(mapPosition))
	{
		isVisible = true;
		position = targetable->GetPositionMapToWindow(mapPosition);
	}
	else
	{
		//비활성화 Enemy
		isVisible = false;
		// Todo : level에서 턴시작할때 bTurnEnd = false로 바꿔주기
		return;
	}

	if (ability.bTurnEnd)
	{
		return;
	}

	//이동 타이머
	if (path.size()>1 || bRandomMove)
	{
		moveTimer.Tick(deltaTime);
		if (moveTimer.IsTimeout())
		{
			if (currentState == EnemyState::TrackingTarget)
			{
				MovePath();
			}
			else if (currentState == EnemyState::RandomMove)
			{
				RandomMove();
			}
			moveTimer.Reset();
		}
	}



	// 공격 타이머
	if (bOnAttack)
	{
		attackTimer.Tick(deltaTime);
		if (attackTimer.IsTimeout())
		{
			AttackTarget();
			attackTimer.Reset();
		}
	}
}

void DGEnemy::SetState(EnemyState inState)
{
	currentState = inState;

	switch (inState)
	{
	case EnemyState::Idle:
		Idle();
		break;
	case EnemyState::RandomMove:
		bRandomMove = true;
		break;
	case EnemyState::TrackingTarget:
		TrackingTarget();
		break;
	case EnemyState::AttackTarget:
		bOnAttack = true;
		attackTimer.Reset();
		break;
	}
}

void DGEnemy::Idle()
{

}

void DGEnemy::RandomMove()
{
	if (ability.moveCurrentCount >= ability.moveMaxCount)
	{
		return;
	}

	//방향 
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, -1, 1 };
	// 좌 우 상 하 랜덤 이동 후 플레이어와 Tracking 확인
	ITargetable* targetable = dynamic_cast<ITargetable*>(owner);
	if (targetable != nullptr)
	{
		int dirIdx = Utils::Random(0, 3);
		Vector2 dir(dx[dirIdx], dy[dirIdx]);

		// 카메라를 벗어날때 처리 및 갈 수 있는 길인지 확인
		if (targetable->CanMoveEnemy(this, mapPosition + dir, isVisible))
		{
			mapPosition = mapPosition + dir;
			position = position + dir;
		}
		++ability.moveCurrentCount;
	}

	bRandomMove = false;
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
	//플레이어와의 거리 체크
	int distance = GetDistanceToTarget();
	//공격 사거리안에 플레이어가 있다면 멈춘 후 공격
	if (distance <= ability.attackDistance)
	{
		EndMove();
		return;
	}
	//탐색 사거리 밖에 있다면 랜덤이동
	if (distance > findTargetDistance)
	{
		SetState(EnemyState::RandomMove);
		return;
	}
	//플레이어를 찾을 수 있다면 레벨에 적의 지도좌표를 넘겨주고 경로를 가져오기
	ITargetable* targetable = dynamic_cast<ITargetable*>(owner);
	if (targetable != nullptr && (targetable->CanFindPlayer(target)))
	{
		//경로 찾기
		path = targetable->GetFindPathToTarget(mapPosition, target);
		//플레이어가 바로 앞에 있다면 이동 종료
		if (path.size() == 1)
		{
			return;
		}
		//움직임 타이머 리셋
		moveTimer.Reset();
	}
	else
	{
		//랜덤이동
		SetState(EnemyState::RandomMove);
	}

}

//경로를 전부 거쳐서 갔거나 moveCount를 전부 소진했다면 이동하지 않음
void DGEnemy::MovePath()
{
	if (pathIdx > path.size()-2)
	{
		return;
	}

	Vector2 dir = path[pathIdx+1] - path[pathIdx];
	if (ability.moveCurrentCount < ability.moveMaxCount)
	{
		//카메라 안에 있으면 활성화 //맵 위치를 window위치로 저장
		ITargetable* targetable = dynamic_cast<ITargetable*>(owner);
		if (targetable != nullptr && targetable->InCameraEnemy(mapPosition))
		{
			isVisible = true;
			mapPosition = mapPosition + dir;
			position = targetable->GetPositionMapToWindow(mapPosition);
		}
		else
		{
			//비활성화 Enemy
			isVisible = false;
			mapPosition = mapPosition + dir;
			// Todo : level에서 턴시작할때 bTurnEnd = false로 바꿔주기
			return;
		}
		++ability.moveCurrentCount;
		++pathIdx;
	}
	else
	{
		path.clear();
		pathIdx = 0;
	}

	
}

void DGEnemy::EndMove()
{
	path.clear();
	pathIdx = 0;

	SetState(EnemyState::AttackTarget);
}

void DGEnemy::AttackTarget()
{
	if (target == nullptr)
	{
		ability.attackCurrentCount = 0;
		bOnAttack = false;
		return;
	}

	target->TakeDamage(this, ability.attackDamage);
	++ability.attackCurrentCount;

	if (ability.attackCurrentCount >= ability.attackMaxCount)
	{
		ability.attackCurrentCount = 0;
		ability.moveCurrentCount = ability.moveMaxCount;
		bOnAttack = false;
	}
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

void DGEnemy::ReadyTurn()
{
	super::ReadyTurn();
}



void DGEnemy::UseTurn()
{
	super::UseTurn();
	// 턴 종료 조건

	if (path.size() > 1 || bOnAttack)
		return;

	if (ability.moveCurrentCount >= ability.moveMaxCount || !isVisible)
	{
		// Todo : level에서 턴시작할때 bTurnEnd = false로 바꿔주기
		ability.bTurnEnd = true;
		ability.currentTurnPoint += 8;
		ability.moveCurrentCount = 0;
		return;
	}

	

	// 1순위 : 타겟 탐색 TrackingTarget
	// 2순위 : 타겟 공격 TrackingTarget -> Attack
	// 3순위 : 타겟 추격 TrackingTarget
	// 4순위 : 이동 할지 가만히 있을지 랜덤 결정  TrackingTarget->RandomMove

	SetState(EnemyState::TrackingTarget);
}
