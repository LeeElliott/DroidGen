#pragma once
class Button
{
public:
	Button(float xPos, float yPos, float bWdth, float bHgt, float scrWdth, float scrHgt);
	~Button();

	void Pressed();

private:
	float screenWidth;
	float screenHeight;
	float width;
	float height;
	float xPosition;
	float yPosition;
	float left;
	float right;
	float top;
	float bottom;

	bool isPressed = false;
};

