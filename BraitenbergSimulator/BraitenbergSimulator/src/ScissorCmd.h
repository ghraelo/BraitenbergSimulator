#pragma once
#include "RenderCmd.h"

//GLEW
#include <glew\glew.h>
//GLFW
#include <glfw\glfw3.h>

#include <string>

class OpenGLRenderer;

class ScissorCmd : public RenderCmd
{
public:
	ScissorCmd();
	void Execute(OpenGLRenderer& renderer) override;
	float x;
	float y;
	float w;
	float h;
	bool enable;
};