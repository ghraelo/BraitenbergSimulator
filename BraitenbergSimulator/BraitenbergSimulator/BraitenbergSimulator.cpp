// BraitenbergSimulator.cpp : Defines the entry point for the console application.
//

#include <Box2D\Box2D.h>

//GLEW
#define GLEW_STATIC
#include <glew\glew.h>
//GLFW
#define _GLFW_USE_DWM_SWAP_INTERVAL 1
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

	glfwSwapInterval(1);

	//measure frame time
	double time1 = glfwGetTime();
	double frameTime = 0.0;

	while (!glfwWindowShouldClose(se.GetWindow()))
	{
		se.HandleEvents();
		se.Update(frameTime);
		se.Render();

		double time2 = glfwGetTime();
		double alpha = 0.9f;
		frameTime = alpha * frameTime + (1.0 - alpha) * (time2 - time1);
		time1 = time2;

		glfwPollEvents();
		glfwSwapBuffers(se.GetWindow());
	}

	se.Cleanup();
	return 0;
}