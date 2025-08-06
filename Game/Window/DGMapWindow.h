#pragma once
#include "DGWindow.h"

class DGMapWindow : public DGWindow
{
	friend class DungeonLevel;
public:
	DGMapWindow();
	void SetMapWindow(Vector2 windowPosition, Vector2 windowSize, Vector2 imagePosition, char*& image, Vector2 imageSize);
	virtual void Render();

	void MoveCamera(Vector2 direction);
	Vector2 GetPositionWindowToMap(Vector2 inScreenPosition);
	Vector2 GetPositionMapToWindow(Vector2 inWindowPosition);

private:

};