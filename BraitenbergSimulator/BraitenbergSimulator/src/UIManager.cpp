#include "UIManager.h"

#include "imgui.h"

#include <iostream>
#include <Box2D\Common\b2Math.h>
#include <exception>
#include "Processor.h"
#include "ResourceManager.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>

UIManager::UIManager()
{
}

void UIManager::DrawUI(const StatisticsManager& sm)
{
	if (m_selectedVehicle != nullptr)
	{
		DrawVehicleUI(m_selectedVehicle);
		printf("selected!\n");
	}
//	DrawStatsPane(sm);
}

void UIManager::DrawVehicleUI(Vehicle* veh)
{
	/*m_regions.clear();


	b2Vec2 vehScreenCoords = g_camera.ConvertWorldToScreen(m_selectedVehicle->GetPosition());
	//std::cout << "vehicle screen coords: (" << vehScreenCoords.x << ", " << vehScreenCoords.y << ")" << std::endl;
	int x = (int)vehScreenCoords.x + 20;
	int y = (int)g_camera.m_height - vehScreenCoords.y + 20;

	int position = y + 85;

	std::string text = veh->GetName() + ":";
	int scroll = 0;
	
	b2Vec2 topRight(x + 150, g_camera.m_height - (y + 150));
	b2Vec2 bottomLeft(x, g_camera.m_height - y);
	topRight = g_camera.ConvertScreenToWorld(topRight);
	bottomLeft = g_camera.ConvertScreenToWorld(bottomLeft);
	//printf("rectangle: topRight:(%f,%f), bottomLeft:(%f,%f)\n", topRight.x, topRight.y, bottomLeft.x, bottomLeft.y);
	m_regions.push_back(Rectangle(topRight,bottomLeft));

	imguiBeginScrollArea(veh->GetName().c_str(), x, y, 150, 150, &scroll);
	char* btext;
	if (veh->ControlStatus())
		btext = "Disable controller";
	else
		btext = "Enable controller";

	if (imguiButton(btext,true))
	{
		if (veh->ControlStatus())
			veh->DisableControl();
		else
			veh->EnableControl();
	}

	imguiLabel("Left sensor:");
	imguiBarDisplay(veh->leftSensor.m_intervals);
	imguiLabel("Right sensor:");
	imguiBarDisplay(veh->rightSensor.m_intervals);
	imguiEndScrollArea();*/
}

void UIManager::DrawBaseUI(BaseUISettings& settings, const WindowState& ws)
{
	imguiBeginScrollArea("Simulator v0.1a", 0, 0, ws.width*0.2, ws.height, &settings.scroll);
	
	std::stringstream sStream;
	sStream << "Frame rate: " << std::setprecision(1) << std::fixed << 1/(settings.frameTime) << " hz";
	imguiLabel(sStream.str().c_str());
	std::stringstream sStream2;

	sStream2 << "Elapsed time: " << std::setprecision(2) << std::fixed << (glfwGetTime() - settings.startTime) << " s";
	imguiLabel(sStream2.str().c_str());

	imguiLabel("Select scene: ");
	if (imguiButton(settings.activeSceneFilename.c_str(), true))
	{
		settings.selectPanelOpen = true;
	}
	if (settings.paused == false)
	{
		if (imguiButton("Pause", true))
		{
			settings.paused = true;
		}
	}
	else
	{
		if (imguiButton("Unpause", true))
		{
			settings.paused = false;
		}
	}
	if (imguiCheck("Enable dragging", settings.mouseDrag, true))
	{
		settings.mouseDrag = !settings.mouseDrag;
	}
	if (imguiCheck("Show stats pane", settings.showStatsPane, true))
	{
		settings.showStatsPane = !settings.showStatsPane;
	}
	settings.shouldExit = imguiButton("Exit to menu", true);
	imguiEndScrollArea();

	//draw scene select panel
	if (settings.selectPanelOpen == true)
	{
		imguiBeginScrollArea("Select scene", ws.width*0.2, 0, ws.width*0.2, ws.height, &settings.scroll2);

		std::vector<std::string> files;
		ResourceManager::GetFilesInDirectory(files, "yaml");
		for (auto& file : files)
		{
			if (imguiButton(file.c_str(), true))
			{
				settings.activeSceneFilename = file.c_str();
				settings.selectPanelOpen = false;
			}
		}

		imguiEndScrollArea();
	}
}

void UIManager::DrawNoVisUI(NoVisUISettings & settings, const WindowState & ws)
{
	imguiBeginScrollArea("Simulator v0.1a", 0, 0, ws.width*0.2, ws.height, &settings.scroll);

	std::stringstream sStream2;

	sStream2 << "Current scene: " << std::setprecision(2) << std::fixed << settings.sceneName << " s";
	imguiLabel(sStream2.str().c_str());

	sStream2.str(std::string());
	sStream2.clear();

	sStream2 << "Elapsed sim time: " << std::setprecision(2) << std::fixed << settings.simTime << " s";
	imguiLabel(sStream2.str().c_str());

	sStream2.str(std::string());
	sStream2.clear();

	double accel = !(settings.simTime == 0.0 || settings.running == false) ? settings.simTime / (glfwGetTime() - settings.startTime) : 0.0;

	sStream2 << "Time acceleration: " << std::setprecision(2) << std::fixed << accel << "x";
	imguiLabel(sStream2.str().c_str());

	imguiLabel("Select scene: ");
	if (imguiButton(settings.activeSceneFilename.c_str(), true))
	{
		settings.selectPanelOpen = true;
	}
	if (settings.paused == false)
	{
		if (imguiButton("Pause", true))
		{
			settings.paused = true;
		}
	}
	else
	{
		if (imguiButton("Unpause", true))
		{
			settings.paused = false;
		}
	}
	if (imguiCheck("Enable dragging", settings.mouseDrag, true))
	{
		settings.mouseDrag = !settings.mouseDrag;
	}

	settings.shouldExit = imguiButton("Exit to menu", true);
	imguiEndScrollArea();

	if (settings.running == false)
	{
		imguiBeginScrollArea("Message", ws.width - ws.width*0.6, ws.height - ws.height*0.6, ws.width*0.2, ws.height*0.1, &settings.scroll);
		imguiLabel("Simulation complete!");
		imguiEndScrollArea();
	}

	//draw scene select panel
	if (settings.selectPanelOpen == true)
	{
		imguiBeginScrollArea("Select scene", ws.width*0.2, 0, ws.width*0.2, ws.height, &settings.scroll2);

		std::vector<std::string> files;
		ResourceManager::GetFilesInDirectory(files, "yaml");
		for (auto& file : files)
		{
			if (imguiButton(file.c_str(), true))
			{
				settings.activeSceneFilename = file.c_str();
				settings.selectPanelOpen = false;
			}
		}

		imguiEndScrollArea();
	}
}

void UIManager::DrawStatsPane(const StatisticsManager & sm, const WindowState& ws)
{
	int testScroll = 0;
	bool over = imguiBeginScrollArea("Statistics", ws.width - 200, 0, 200, ws.height, &testScroll);
	
	for (int i = 0; i < sm.GetStats().size(); i++)
	{
		imguiSeparatorLine();

		Processor* p = dynamic_cast<Processor*>(sm.GetStats()[i].get());
		if (p != nullptr)
		{
			imguiLabel(p->GetName().c_str());
			std::vector<float> test(p->GetData().size());
			std::copy(p->GetData().begin(), p->GetData().end(), test.begin());
			imguiGraphDisplay(test);
		}

		FixedWatcher* fw = dynamic_cast<FixedWatcher*>(sm.GetStats()[i].get());
		if (fw != nullptr) 
		{
			imguiLabel(fw->GetName().c_str());
			std::vector<float> test(fw->GetData().size());
			std::copy(fw->GetData().begin(), fw->GetData().end(), test.begin());
			imguiGraphDisplay(test,fw->GetMin(),fw->GetMax());
			continue;
		}

		Watcher* w = dynamic_cast<Watcher*>(sm.GetStats()[i].get());
		if (w != nullptr)
		{
			imguiLabel(w->GetName().c_str());
			std::vector<float> test(w->GetData().size());
			std::copy(w->GetData().begin(), w->GetData().end(), test.begin());
			imguiGraphDisplay(test);
		}


	}
	
	imguiEndScrollArea();

}

bool UIManager::InRegion(const b2Vec2 & point)
{
	//printf("point: (%f,%f)\n", point.x, point.y);
	for (auto& r : m_regions)
	{
		//printf("rectangle: topRight:(%f,%f), bottomLeft:(%f,%f)\n", r.m_topRight.x, r.m_topRight.y, r.m_bottomLeft.x, r.m_bottomLeft.y);
		if (r.Inside(point))
		{
			//printf("inside\n");
			return true;
		}
	}

	return false;
}

void UIManager::SelectVehicle(Vehicle* obj)
{
	m_selectedVehicle = obj;
}

void UIManager::DeselectVehicle()
{
	m_selectedVehicle = nullptr;
}
