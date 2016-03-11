// BraitenbergSimulator.cpp : Defines the entry point for the console application.
//

#include <Box2D\Box2D.h>

#include "main.h"

//GLEW
#define GLEW_STATIC
#include <glew\glew.h>
//GLFW
#include <glfw\glfw3.h>

//STL
#include <iostream>


//...
#include <SimEngine.h>

int main(int argc, char** argv)
{
	
	//FreeConsole();
	SimEngine se;

	se.Init();

	while (!glfwWindowShouldClose(se.GetWindow()))
	{
		se.HandleEvents();
		se.Update();
		se.Render();
	}

	se.Cleanup();
	return 0;
}