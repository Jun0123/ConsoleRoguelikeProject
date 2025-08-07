#pragma once

#include "Math/Vector2.h"
#include "Actors/DGCharacter.h"

// 타겟을 제공할때 사용할 인터페이스
class ITargetable
{
public:
	virtual bool CanFindPlayer(DGCharacter*& playerCharacter) = 0;
	virtual Vector2 GetPositionWindowToMap(Vector2 inScreenPosition) = 0;
	virtual Vector2 GetPositionMapToWindow(Vector2 inWindowPosition) = 0;
	virtual std::vector<Vector2> GetFindPathToTarget(Vector2 startMapPosition, DGCharacter* target) = 0;
	//이동 가능한 위치인지
	virtual bool CanMoveEnemy(DGCharacter* enmey, Vector2 moveToMapPosition,bool& bIsVisible) = 0;
	//화면 안에 있는지
	virtual bool InCameraEnemy(Vector2 enemyMapPosition)=0;
};
