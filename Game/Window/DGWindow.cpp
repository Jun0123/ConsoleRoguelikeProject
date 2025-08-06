#include "DGWindow.h"
#include "Engine.h"

DGWindow::DGWindow()
{
	image = nullptr;
}

DGWindow::DGWindow(Vector2 windowPosition, Vector2 windowSize, Vector2 imagePosition,Vector2 imageSize)
	:windowPosition(windowPosition), windowSize(windowSize), imagePosition(imagePosition), imageSize(imageSize)
{
	image = nullptr;
}

void DGWindow::RenderWindow()
{
	if (image == nullptr)
	{
		return;
	}
	for (int edgeX = windowPosition.x; edgeX < windowPosition.x+ windowSize.x; ++edgeX)
	{
		Engine::Get().WriteValueToBuffer(Vector2(edgeX, windowPosition.y), ' ',Color::WhiteBackground);
		Engine::Get().WriteValueToBuffer(Vector2(edgeX, windowPosition.y + windowSize.y-1), ' ', Color::WhiteBackground);
	}
	for (int edgeY = windowPosition.y; edgeY < windowPosition.y+windowSize.y; ++edgeY)
	{
		Engine::Get().WriteValueToBuffer(Vector2(windowPosition.x, edgeY), ' ', Color::WhiteBackground);
		Engine::Get().WriteValueToBuffer(Vector2(windowPosition.x + windowSize.x-1, edgeY), ' ', Color::WhiteBackground);
	}

	for (int y = 1; y < windowSize.y-1; ++y)
	{
		for (int x = 1; x < windowSize.x-1; ++x)
		{
			Engine::Get().WriteValueToBuffer(windowPosition + Vector2(x, y), (char)image[(imagePosition.y + y) * imageSize.x + imagePosition.x + x]);
		}
	}
}

void DGWindow::Render()
{
	RenderWindow();
}

