#include "DGCharacter.h"

DGCharacter::DGCharacter()
{
}

DGCharacter::DGCharacter(const char* image, Color color, const Vector2& position)
	:Actor(image,color,position)
{

}

void DGCharacter::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}
