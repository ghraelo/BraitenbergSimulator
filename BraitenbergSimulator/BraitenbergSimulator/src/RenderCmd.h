#pragma once

class OpenGLRenderer;

class RenderCmd
{
public:
	RenderCmd()
	{

	};

	virtual void Execute(OpenGLRenderer& renderer) = 0;
};