#pragma once

#include "UIElement.h"
#include <vector>

class BarViewElement : UIElement
{
public:
	BarViewElement();
	BarViewElement(int backgroundColor, int foregroundColor, int length);
	std::vector<float> m_points;
	void Draw(int xPos, int yPos);
protected:
	int m_backColor;
	int m_foreColor;
	int m_length;
};