#pragma once

#include "DGCharacter.h"
#include "Utils/Timer.h"
#include "Delegate.h"
#include <vector>


enum class EnemyState
{
	Idle = 0,
	RandomMove,
	TrackingTarget,
	AttackTarget
};

class DGEnemy : public DGCharacter
{
	friend class DungeonLevelLevel;

	RTTI_DECLARATIONS(DGEnemy, DGCharacter)

public:
	// 적 캐릭터가 보여줄 이미지(문자열)와 생성 y위치.
	DGEnemy(const char* image, Color color, const Vector2& windowPosition, const Vector2& mapPosition, DGCharacterAbility ability, class DungeonLevel* owner);
	virtual void Tick(float deltaTime) override;

	void SetState(EnemyState inState);

	void Idle();
	void RandomMove();
	bool FindTarget();
	void TrackingTarget();
	void MovePath();
	void EndMove();
	void AttackTarget();

	int GetDistanceToTarget();
	virtual void ReadyTurn();
	
	//어떻게 턴을 소모할 것인가?> 이동 카운트 및 공격 카운트를 전부 소모하면 턴종료
	// 1순위 : 타겟 탐색
	// 2순위 : 타겟 공격
	// 3순위 : 타겟 추격
	// 4순위 : 이동 할지 가만히 있을지 랜덤 결정
	virtual void UseTurn() override;
	

protected:
	EnemyState currentState;
	DGCharacter* target;
	int findTargetDistance;

	//한칸씩 이동으로 보이기 위해 만든 변수
	std::vector<Vector2> path;
	size_t pathIdx = 0;
	//한칸씩 이동하게 만들기 위해 타이머 사용
	Timer moveTimer;
	bool bRandomMove;

	//공격
	Timer attackTimer;
	bool bOnAttack;

};

