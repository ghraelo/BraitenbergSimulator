#include "UIManager.h"

#include "imgui.h"
#include "RenderGL3.h"

#include <iostream>
#include <Box2D\Common\b2Math.h>
#include <exception>
#include "Processor.h"

UIManager::UIManager()
{
}

void UIManager::DrawUI(const StatisticsManager& sm)
{
	if (m_selectedVehicle != nullptr)
	{
		DrawVehicleUI(m_selectedVehicle);
	}
	DrawStatsPane(sm);
}

void UIManager::DrawVehicleUI(Vehicle* veh)
{
	m_regions.clear();


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
	imguiEndScrollArea();
}

void UIManager::DrawStatsPane(const StatisticsManager & sm)
{
	int testScroll = 0;
	bool over = imguiBeginScrollArea("Statistics", 0, 10, 200, g_camera.m_height - 20, &testScroll);

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
