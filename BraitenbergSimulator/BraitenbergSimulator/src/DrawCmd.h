#pragma once
#include "RenderCmd.h"

//GLEW
#include <glew\glew.h>
//GLFW
#include <glfw\glfw3.h>

#include <string>

class OpenGLRenderer;

class DrawCmd : public RenderCmd
{
public:
	DrawCmd();
	void Execute(OpenGLRenderer& renderer) override;
	GLfloat* vertices;
	int verticesNum;
	std::string shaderID;
};