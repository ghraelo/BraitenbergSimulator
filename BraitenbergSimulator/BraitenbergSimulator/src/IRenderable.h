#pragma once

class Renderer;

class IRenderable
{
public:
	virtual void Render(Renderer& r) = 0;
};