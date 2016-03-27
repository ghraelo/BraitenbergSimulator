#pragma once
#include <nanovg\nanovg.h>

class Renderer;

class IRenderable
{
public:
	virtual void Render(NVGcontext* vg, Renderer& r) = 0;
};