#pragma once
#include "Math/Vector2.h"

enum class RoomType
{
	Monster,
	MonsterElite,
	MonsterBoss,
	Player,
	Treasurechest
};

struct Rect
{
public:
	Rect()
	{

	}
	Rect(Vector2 inPosition, int inWidth, int inHeight)
		: position(inPosition), width(inWidth), height(inHeight)
	{

	}

	Rect& operator=(const Rect& inRect);

	Vector2 position;
	int width = 0;
	int height = 0;
};

class RoomNode
{
	friend class DungeonMaker;
public:
	RoomNode();
	//기준위치, 너비, 높이
	RoomNode(Vector2 inPosition, int inWidth, int inHeight, float inRatio);
	virtual ~RoomNode();


	inline int GetWidth() const { return rect.width; }
	inline int GetHeight() const { return rect.height; }
	inline void SetWidth(const int inWidth) { rect.width = inWidth; }
	inline void SetHeight(const int inHeight) { rect.height = inHeight; }
	inline Vector2 GetPosition() const { return rect.position; }
	inline void SetPosition(const Vector2 inPosition) {  rect.position = inPosition; }

	void DeleteNode(RoomNode*& nodeToDelete);

	inline float GetRatio() const { return currentRatio; }

protected:
	virtual void SetActors();

protected:
	//분할 자식 객체
	RoomNode* leftNode = nullptr;
	RoomNode* rightNode = nullptr;
	Rect rect;
	Rect room;
	float currentRatio;
};
