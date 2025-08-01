#pragma once
#include "Math/Vector2.h"

class RoomNode
{
public:
	//기준위치, 너비, 높이
	RoomNode(Vector2 inPosition, int inWidth, int inHeight);
	//분할 자식 객체
	RoomNode* leftNode = nullptr;
	RoomNode* rightNode = nullptr;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline Vector2 GetPosition() const { return position; }
private:
	Vector2 position;
	int width = 0;
	int height = 0;
};
