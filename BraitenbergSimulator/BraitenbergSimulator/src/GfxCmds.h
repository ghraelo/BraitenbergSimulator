#pragma once

#include "Rectangle.h"

#include <nanovg\nanovg.h>
#include <string>
#include <memory>
#include <vector>

class GfxCmd {
public:
	GfxCmd();
	virtual void Execute(NVGcontext* vg) = 0;
};

typedef std::unique_ptr<GfxCmd> GfxCmdPtr;

class GfxCmdRect : public GfxCmd
{
public:
	GfxCmdRect(float x, float y, float w, float h, NVGcolor color);
	void Execute(NVGcontext* vg) override;
private:
	NVGcolor m_color;
	float m_x;
	float m_y;
	float m_w;
	float m_h;
};

class GfxCmdRoundedRect : public GfxCmd
{
public:
	GfxCmdRoundedRect(float x, float y, float w, float h, float rounding, NVGcolor color);
	void Execute(NVGcontext* vg) override;
private:
	NVGcolor m_color;
	float m_x;
	float m_y;
	float m_w;
	float m_h;
	float m_rounding;
};

class GfxCmdText : public GfxCmd
{
public:
	GfxCmdText(float x, float y, NVGalign align, std::string text, NVGcolor color);
	void Execute(NVGcontext* vg) override;
private:
	float m_x;
	float m_y;
	NVGcolor m_color;
	NVGalign m_align;
	std::string m_text;
};

class GfxCmdLine : public GfxCmd
{
public:
	GfxCmdLine(float x0, float y0, float x1, float y1, float r, NVGcolor color);
	void Execute(NVGcontext* vg) override;
private:
	float m_x0;
	float m_y0;
	float m_x1;
	float m_y1;
	float m_r;
	NVGcolor m_color;
};

class GfxCmdPolyLine : public GfxCmd
{
public:
	GfxCmdPolyLine(std::vector<std::pair<float,float>> points, float r, NVGcolor color);
	void Execute(NVGcontext* vg) override;
private:
	std::vector<std::pair<float, float>> m_points;
	float m_r;
	NVGcolor m_color;
};

class GfxCmdScissor : public GfxCmd
{
public:
	GfxCmdScissor(float x, float y, float w, float h);
	void Execute(NVGcontext* vg) override;
private:
	float m_x;
	float m_y;
	float m_w;
	float m_h;
};