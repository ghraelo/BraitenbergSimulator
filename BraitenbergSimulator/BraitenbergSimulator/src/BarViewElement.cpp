#include "BarViewElement.h"
#include "imgui.h"
#include "Renderer.h"
#include <algorithm>
#include "LightSensor.h"
BarViewElement::BarViewElement()
{
}

BarViewElement::BarViewElement(int backgroundColor, int foregroundColor, int length)
	:m_backColor(backgroundColor), m_foreColor(foregroundColor), m_length(length)
{

}

