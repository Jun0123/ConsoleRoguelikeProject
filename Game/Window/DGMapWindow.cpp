#include "DGMapWindow.h"
#include "Engine.h"

DGMapWindow::DGMapWindow()
	:DGWindow(Vector2::Zero, Vector2::Zero,Vector2::Zero,0)
{
}

void DGMapWindow::SetMapWindow(Vector2 windowPosition, Vector2 windowSize, Vector2 imagePosition, char*& image, Vector2 imageSize)
{
	this->windowPosition = windowPosition;
	this->windowSize = windowSize;
	this->imagePosition = imagePosition;
	this->image = image;
	this->imageSize = imageSize;
}



void DGMapWindow::Render()
{
	DGWindow::Render();
}

void DGMapWindow::MoveCamera(Vector2 direction)
{
	imagePosition.x += direction.x;
	imagePosition.y += direction.y;
}

Vector2 DGMapWindow::GetPositionWindowToMap(Vector2 inWindowPosition)
{
	return inWindowPosition - windowPosition + imagePosition;
}

Vector2 DGMapWindow::GetPositionMapToWindow(Vector2 inMapPosition)
{
	//현재 window 위치 + 길이
	Vector2 size = inMapPosition - imagePosition;
	Vector2 currentPosition = windowPosition + size;
	//Vector2 currentPosition = (windowPosition + inMapPosition - imagePosition);
	/*if (currentPosition.x < 0)
		currentPosition.x = inMapPosition.x;
	if (currentPosition.y < 0)
		currentPosition.y = inMapPosition.y;*/

	return  currentPosition;
}