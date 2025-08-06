#pragma once
#include "Level/Level.h"
#include "Window/DGMapWindow.h"
#include "Interface/ITargetable.h"

/* ECS - 서로 모름.
* 게임 레벨의 책임.
* [x] 적생성: 약간의 랜덤성을 가미해서 일정 시간마다 적 생성.
* [x] 충돌처리: 플레이어-적 탄약 사이의 충돌/적-플레이어 탄약의 충돌 처리(AABB).
* [x] 점수 관리: 플레이어가 적을 없애면 1점씩 획득.
* [x] 게임 판정: 플레이어가 죽으면 게임 종료.
*/

enum class RoomType : unsigned char;

class DungeonLevel : public Level, public ITargetable
{
	RTTI_DECLARATIONS(DungeonLevel, Level)

public:
	DungeonLevel();
	~DungeonLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Render() override;
	virtual bool CanFindPlayer(DGCharacter*& playerCharacter) override;
	virtual Vector2 GetPositionWindowToMap(Vector2 inScreenPosition) override;
	virtual Vector2 GetPositionMapToWindow(Vector2 inMapPosition) override;
private:
	void SpawnPlayer();
	void SpawnEnemies();
	void LoadMap();
	void CanMoveActor(int x,int y);

private:
	std::vector< enum class RoomType> allRoomTypes;
	std::vector< class RoomNode> allRooms;
	char* map=nullptr;
	class DGPlayer* player = nullptr;
	DGMapWindow mapWindow;
	//Vector2 
	//Vector2 cameraPosition;
};