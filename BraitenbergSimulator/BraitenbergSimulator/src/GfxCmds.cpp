#include "GfxCmds.h"

GfxCmd::GfxCmd()
{
}

GfxCmdRect::GfxCmdRect(float x, float y, float w, float h, NVGcolor color)
	:GfxCmd()
{
	m_color = color;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;

}

void GfxCmdRect::Execute(NVGcontext* vg)
{
	nvgBeginPath(vg);
	nvgRect(vg, m_x, m_y, m_w, m_h);
	nvgFillColor(vg, m_color);
	nvgFill(vg);
}

GfxCmdRoundedRect::GfxCmdRoundedRect(float x, float y, float w, float h, float rounding, NVGcolor color)
	:GfxCmd()
{
	m_color = color;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
	m_rounding = rounding;
}

void GfxCmdRoundedRect::Execute(NVGcontext * vg)
{
	nvgBeginPath(vg);
	nvgRoundedRect(vg, m_x, m_y, m_w, m_h, m_rounding);
	nvgFillColor(vg, m_color);
	nvgFill(vg);
}

GfxCmdText::GfxCmdText(float x, float y, NVGalign align, std::string text, NVGcolor color)
	:GfxCmd()
{
	m_x = x;
	m_y = y;
	m_align = align;
	m_text = text;
	m_color = color;
}

void GfxCmdText::Execute(NVGcontext * vg)
{
	nvgBeginPath(vg);
	nvgFillColor(vg, m_color);
	nvgTextAlign(vg, m_align);
	nvgText(vg, m_x, m_y, m_text.c_str(), NULL);
	nvgFill(vg);
}

GfxCmdLine::GfxCmdLine(float x0, float y0, float x1, float y1, float r, NVGcolor color)
	:GfxCmd()
{
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;
	m_r = r;
	m_color = color;
}

void GfxCmdLine::Execute(NVGcontext * vg)
{
	nvgBeginPath(vg);
	nvgMoveTo(vg, m_x0, m_y0);
	nvgLineTo(vg, m_x1, m_y1);
	nvgStrokeWidth(vg, 1);
	nvgStrokeColor(vg, m_color);
	nvgStroke(vg);
}

GfxCmdScissor::GfxCmdScissor(float x, float y, float w, float h)
{
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
}

void GfxCmdScissor::Execute(NVGcontext * vg)
{
	if (m_x == -1 && m_y == -1 && m_w == -1 && m_h == -1)
	{
		nvgResetScissor(vg);
	}
	else
	{
		nvgScissor(vg, m_x, m_y, m_w, m_h);
	}
}
