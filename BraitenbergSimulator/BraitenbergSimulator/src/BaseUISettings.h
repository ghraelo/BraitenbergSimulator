#pragma once

#include <string>

//settings for base UI panel
struct BaseUISettings
{
	bool paused = false; //pause simulation
	std::string activeSceneFilename; //filename of active scene
	bool shouldExit = false; //exit to menu at end of current draw cycle
	bool mouseDrag = false; //enable mouse dragging of sim objects
	int scroll = 0; //scroll state
	int scroll2 = 0; //select panel scroll state
	bool selectPanelOpen = false; //scene selection panel open
	bool showStatsPane = true; //show stats pane
	double frameTime = 0.0; //frame time
	double startTime = 0.0;
};