#pragma once

#include "GfxCmds.h"

#include <nanovg\nanovg.h>
#include <nanovg\nanovg_gl.h>

class GUIRenderer
{
public:
	GUIRenderer();
	GUIRenderer(float width, float height, float pixelRatio);
	~GUIRenderer();
	void SetFont(std::string file);
	void SetWindowDimensions(float width, float height, float pixelRatio);
	void AddGfxCmdLine(float x0, float y0, float x1, float y1, float size, NVGcolor color);
	void AddGfxCmdRect(float x, float y, float w, float h, NVGcolor color);
	void AddGfxCmdRoundedRect(float x, float y, float w, float h, float r, NVGcolor color);
	void AddGfxCmdText(float x, float y, const char* text, NVGcolor color);
	void AddGfxCmdTriangle(float x, float y, float w, float h, float r, NVGcolor color);
	void AddGfxCmdScissor(float x, float y, float w, float h);
	void Flush();
private:
	std::vector<GfxCmdPtr> m_cmdList;
	NVGcontext* vg;
	float m_width;
	float m_height;
	float m_pixelRatio;
	int fontHandle;
};

typedef std::unique_ptr<GUIRenderer> GUIRendererPtr;