#include "GUIRenderer.h"
#include "GfxCmds.h"

GUIRenderer::GUIRenderer()
{
}


GUIRenderer::GUIRenderer(float width, float height, float pixelRatio)
	:m_width(width),m_height(height),m_pixelRatio(pixelRatio)
{
	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

GUIRenderer::~GUIRenderer()
{
	nvgDeleteGL3(vg);
}

void GUIRenderer::SetFont(std::string file)
{
	fontHandle = nvgCreateFont(vg, "droidsans", file.c_str());
	nvgFontSize(vg, 12);
	nvgFontFace(vg,"droidsans");
}

void GUIRenderer::SetWindowDimensions(float width, float height, float pixelRatio)
{
	m_width = width;
	m_height = height;
	m_pixelRatio = pixelRatio;
}

void GUIRenderer::AddGfxCmdLine(float x0, float y0, float x1, float y1, float size, NVGcolor color)
{
	GfxCmdPtr p = std::make_unique<GfxCmdLine>(x0, y0, x1, y1, size, color);
	m_cmdList.push_back(std::move(p));

}

void GUIRenderer::AddGfxCmdRect(float x, float y, float w, float h, NVGcolor color)
{
	GfxCmdPtr p = std::make_unique<GfxCmdRect>(x, y, w, h, color);
	m_cmdList.push_back(std::move(p));
}

void GUIRenderer::AddGfxCmdRoundedRect(float x, float y, float w, float h, float r, NVGcolor color)
{
	GfxCmdPtr p = std::make_unique<GfxCmdRoundedRect>(x, y, w, h, r, color);
	m_cmdList.push_back(std::move(p));
}

void GUIRenderer::AddGfxCmdText(float x, float y, const char * text, NVGcolor color)
{
	GfxCmdPtr p = std::make_unique<GfxCmdText>(x, y, (NVGalign)(NVG_ALIGN_CENTER | NVG_ALIGN_TOP), std::string(text), color);
	m_cmdList.push_back(std::move(p));
}

void GUIRenderer::AddGfxCmdTriangle(float x, float y, float w, float h, float r, NVGcolor color)
{
}

void GUIRenderer::AddGfxCmdScissor(float x, float y, float w, float h)
{
	GfxCmdPtr p = std::make_unique<GfxCmdScissor>(x, y, w, h);
	m_cmdList.push_back(std::move(p));
}

void GUIRenderer::Flush()
{
	nvgBeginFrame(vg,m_width, m_height, m_pixelRatio);

	for (auto& cmd : m_cmdList)
	{
		cmd->Execute(vg);
	}
	nvgEndFrame(vg);
}
