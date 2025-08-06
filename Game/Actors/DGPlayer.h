#pragma once
#include "DGCharacter.h"
#include "Delegate.h"
#include <vector>

class DGPlayer :  public DGCharacter
{
	RTTI_DECLARATIONS(DGPlayer, DGCharacter)
public:
	DGPlayer(Vector2 position);

	virtual void Tick(float deltaTime) override;

	// 키 눌림 이벤트에 리스너 함수를 등록할 때 사용할 함수.
	template<typename T, void (T::* Method)(int ,int)>
	void TestEvent(T* object)
	{
		// 델리게이트를 전달한 객체가 Actor의 자손인지 확인.
		if (!object)
		{
			// Visual Studio 출력(Output) 창에 오류 메시지 출력.
			OutputDebugStringA("일반 함수는 메소드 등록이 불가능합니다.");

			// Debug 모드에서 일시정지시킬 때 사용하는 함수.
			__debugbreak();
		}

		// 델리게이트 객체 생성.
		Delegate delegate(object, [](void* instance,int x,int y)
			{
				// 형변환(Down-Casting) 후 메소드 호출.
				T* derived = static_cast<T*>(instance);
				(derived->*Method)(x,y);
			});

		// 콜백에 델리게이트 추가.
		PlayerMoveCallbacks.emplace_back(delegate);
	}


	// 오른쪽 이동 함수.
	void Move(int x, int y);
private:
	//플레이어가 움직을 때 position값을 level에게 전달
	

private:
	// 키입력 관련 이벤트.
	std::vector<Delegate> PlayerMoveCallbacks;
};

