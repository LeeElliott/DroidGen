#include "Button.h"

Button::Button(float xPos, float yPos, float bWdth, float bHgt, float scrWdth, float scrHgt)
{
	screenWidth = scrWdth;
	screenHeight = scrHgt;

	width = bWdth;
	height = bHgt;

	xPosition = xPos * 2 / screenWidth - 1;
	yPosition = yPos * (-2/screenHeight) +1;

	left = xPosition - (width / screenWidth);
	right = xPosition + (width / screenWidth);
	top = yPosition + (height / screenHeight);
	bottom = yPosition - (height / screenHeight);
}

Button::~Button()
{
}

void Button::Pressed()
{
}
