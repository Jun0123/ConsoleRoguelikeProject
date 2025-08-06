#pragma once

#include "Math/Vector2.h"

class DGWindow
{
	friend class DungeonLevel;
public:
	DGWindow();
	DGWindow(Vector2 windowPosition, Vector2 windowSize,Vector2 imagePosition,Vector2 imageSize);
	virtual void RenderWindow();
	virtual void Render();
	inline Vector2 GetWindowPosition() const { return windowPosition; }
	inline Vector2 GetWindowSize() const { return windowSize; }
protected:
	Vector2 windowPosition;
	Vector2 windowSize;
	Vector2 imagePosition;
	char* image;
	Vector2 imageSize;
};
