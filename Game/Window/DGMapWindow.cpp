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
//화면->지도 좌표변환
Vector2 DGMapWindow::GetPositionWindowToMap(Vector2 inWindowPosition)
{
	//화면기준 좌표로부터 임의 좌표의 길이 + 카메라 기준 좌표
	return inWindowPosition - windowPosition + imagePosition;
}
//지도->화면 좌표변환
Vector2 DGMapWindow::GetPositionMapToWindow(Vector2 inMapPosition)
{
	//길이 = 카메라에서의 임의 좌표 - 카메라 기준 좌표
	Vector2 size = inMapPosition - imagePosition;
	//화면 좌표 = 화면 기준 좌표 + 길이
	Vector2 currentPosition = windowPosition + size;
	return  currentPosition;
}