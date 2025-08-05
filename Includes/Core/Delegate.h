#pragma once

/*
* Delegate 클래스.
* - 클래스(또는 구조체) 메소드를 등록해 콜백 처리가 가능하도록 해주는 클래스.
* - 일반 함수는 메소드 등록 불가.
* - 일반 함수와/클래스(구조체)의 메소드를 모두 활용할 수 있게 하려면 구조가 복잡해져 제한을 둠.
*/

#include "Core.h"

// 델리게이트에서 사용할 함수 포인터 타입 선언.
using Function = void(*)(void*);
//함수선언 = (반환형) (함수인스턴스, 매개변수)
using FunctionPosition = void(*)(void* ,int x,int y);

// 델리게이트 클래스.
struct Engine_API Delegate
{
	// 델리게이트에서 참조할 인스턴스.
	void* instance = nullptr;

	// 실행할 메소드 포인터.
	Function function = nullptr;
	FunctionPosition functionPosition = nullptr;

	Delegate() = default;

	Delegate(void* const instance, Function function)
		: instance(instance), function(function)
	{
	}
	// 인스턴스 및 델리게이트 함수 등록
	Delegate(void* const instance, FunctionPosition function)
		: instance(instance), functionPosition(function)
	{
	}

	// 델리게이트 호출 함수.
	void Invoke()
	{
		// 함수 호출 가능 여부 확인.
		if (!instance || !function)
		{
			return;
		}

		// 콜백 함수 호출.
		function(instance);
	}
	void Invoke(int x,int y)
	{
		// 함수 호출 가능 여부 확인.
		if (!instance || !functionPosition)
		{
			return;
		}

		// 콜백 함수 호출.
		functionPosition(instance, x,y);
	}

	// 호출 연산자 오버로딩.
	void operator()()
	{
		Invoke();
	}
	// 등록된 델리게이트 실행
	void operator()(int x, int y)
	{
		Invoke(x,y);
	}
};