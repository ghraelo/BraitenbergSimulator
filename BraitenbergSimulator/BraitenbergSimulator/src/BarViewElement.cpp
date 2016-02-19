#include "BarViewElement.h"
#include "imgui.h"
#include "Renderer.h"
#include <algorithm>

BarViewElement::BarViewElement()
{
}

BarViewElement::BarViewElement(int backgroundColor, int foregroundColor, int length)
	:m_backColor(backgroundColor), m_foreColor(foregroundColor), m_length(length)
{

}

void BarViewElement::Render(Renderer& r)
{
	r.RenderBarViewElement(*this);
}
