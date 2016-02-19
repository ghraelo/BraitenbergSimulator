#pragma once

#include "UIElement.h"
#include <vector>

class Renderer;

class BarViewElement : UIElement
{
public:
	BarViewElement();
	BarViewElement(int backgroundColor, int foregroundColor, int length);
	std::vector<float> m_points;
	void Render(Renderer& r);

	int x;
	int y;

	int m_backColor;
	int m_foreColor;
	int m_length;
};