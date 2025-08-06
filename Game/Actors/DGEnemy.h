#pragma once

#include "DGCharacter.h"

enum class EnemyState
{
	Idle = 0,
	RandomMove,
	TrackingTarget,
	AttackTarget
};

class DGEnemy : public DGCharacter
{
	RTTI_DECLARATIONS(DGEnemy, DGCharacter)

public:
	// 적 캐릭터가 보여줄 이미지(문자열)와 생성 y위치.
	DGEnemy(
		const char* image = "",
		Color color = Color::White,
		const Vector2& position = Vector2::Zero, const Vector2& mapPosition = Vector2::Zero, int findTargetDistance = 10, int attackDistance = 1
		);
	virtual void Tick(float deltaTime) override;

	void SetState(EnemyState inState);

	void Idle();
	void RandomMove();
	bool FindTarget();
	void TrackingTarget();
	void AttackTarget();
	

	int GetDistanceToTarget();
	
	
	//어떻게 턴을 소모할 것인가?> 이동 카운트 및 공격 카운트를 전부 소모하면 턴종료
	// 1순위 : 타겟 탐색
	// 2순위 : 타겟 공격
	// 3순위 : 타겟 추격
	// 4순위 : 이동 할지 가만히 있을지 랜덤 결정
	void UseTurn();
	

protected:
	Vector2 mapPosition;
	EnemyState currentState;
	DGCharacter* target;
	int findTargetDistance;
	int attackDistance;
};

