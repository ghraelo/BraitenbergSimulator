#pragma once

#include <string>

//settings for no visualisation UI panel
struct NoVisUISettings
{
	bool paused = false; //pause simulation
	std::string sceneName;
	std::string activeSceneFilename; //filename of active scene
	bool shouldExit = false; //exit to menu at end of current draw cycle
	bool mouseDrag = false; //enable mouse dragging of sim objects
	int scroll = 0; //scroll state
	int scroll2 = 0; //select panel scroll state
	bool selectPanelOpen = false; //scene selection panel open
	double simTime = 0.0; //time elapsed in simulation
	double frameTime = 0.0;
	double startTime = 0.0;
	bool running = false;
	int testNumber = -1;
	bool testsDone = false;
	double endTime = 0.0;
	int slider = 60000;
};