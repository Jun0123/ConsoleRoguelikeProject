#include "DGPlayer.h"
#include "Input.h"

DGPlayer::DGPlayer(Vector2 position)
	:Actor("P",Color::Green,position)
{
	// keydown 콜백 등록.
	
}

void DGPlayer::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 키 눌림 반복 이벤트 발행.
	if (PlayerMoveCallbacks.size() > 0)
	{
		// 키 눌림 반복 이벤트 확인.
			// 방향키 입력.
		if (Input::Get().GetKeyDown(VK_LEFT))
		{
			Vector2 position = Position();
			position.x -= 1;
			for (auto& playerMoveDelete : PlayerMoveCallbacks)
			{
				playerMoveDelete(position.x,position.y);
			}
		}

		if (Input::Get().GetKeyDown(VK_RIGHT))
		{
			Vector2 position = Position();
			position.x += 1;
			for (auto& playerMoveDelete : PlayerMoveCallbacks)
			{
				playerMoveDelete(position.x, position.y);
			}
		}

		if (Input::Get().GetKeyDown(VK_UP))
		{
			Vector2 position = Position();
			position.y -= 1;
			for (auto& playerMoveDelete : PlayerMoveCallbacks)
			{
				playerMoveDelete(position.x, position.y);
			}
		}

		if (Input::Get().GetKeyDown(VK_DOWN))
		{
			Vector2 position = Position();
			position.y += 1;
			for (auto& playerMoveDelete : PlayerMoveCallbacks)
			{
				playerMoveDelete(position.x, position.y);
			}
		}
		
	}

	

	// 입력 처리.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 게임 종료.
		QuitGame();
		return;
	}

	
}

void DGPlayer::Move(int x,int y)
{
	position.x = x;
	position.y = y;
}
