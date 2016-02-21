#pragma once

#include "UIElement.h"
#include <vector>

class Renderer;

class BarViewElement : UIElement
{
public:
	BarViewElement();
	BarViewElement(int backgroundColor, int foregroundColor, int length);

	int m_length;
	int m_backColor;
	int m_foreColor;
};