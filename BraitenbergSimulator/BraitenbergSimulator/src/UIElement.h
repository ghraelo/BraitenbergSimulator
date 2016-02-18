#pragma once
class UIElement
{
public:
	UIElement();
	virtual void Draw(int xPos, int yPos) = 0;
	~UIElement();
};

