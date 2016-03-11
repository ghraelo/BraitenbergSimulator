#include "GUIRenderer.h"
#include "GfxCmds.h"

GUIRenderer::GUIRenderer()
{
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

void GUIRenderer::AddGfxCmdText(float x, float y, const char * text, NVGalign align, NVGcolor color)
{
	GfxCmdPtr p = std::make_unique<GfxCmdText>(x, y, (NVGalign)(align | NVG_ALIGN_TOP), std::string(text), color);
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

void GUIRenderer::Flush(NVGcontext* vg)
{
	for (auto& cmd : m_cmdList)
	{
		cmd->Execute(vg);
	}
}
