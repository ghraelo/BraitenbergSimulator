#include "UIManager.h"

#include "imgui.h"
#include "RenderGL3.h"

#include <iostream>
#include <Box2D\Common\b2Math.h>

UIManager::UIManager()
{
}

void UIManager::DrawUI()
{
	if (m_selectedVehicle != nullptr)
	{
		DrawVehicleUI(m_selectedVehicle);
	}
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
	if (imguiButton("Control",true))
	{
		printf("CLICKED!!!!\n");
	}
	//imguiDrawText(x + 5, position, TEXT_ALIGN_LEFT,text.c_str(), SetRGBA(255, 255, 255, 128));
	//position -= 20;
	imguiLabel("Left sensor:");
	imguiBarDisplay(veh->leftSensor.m_intervals);
	imguiLabel("Right sensor:");
	imguiBarDisplay(veh->rightSensor.m_intervals);
	//imguiDrawText(x + 5, position, TEXT_ALIGN_LEFT, "Left Sensor:", SetRGBA(255, 255, 255, 128));
	//position -= 10;
	//DrawBarElement(x + 10, position, veh->leftSensor);
	//position -= 20;
	//imguiDrawText(x + 5, position, TEXT_ALIGN_LEFT, "Right Sensor:", SetRGBA(255, 255, 255, 128));
	//position -= 10;
	//DrawBarElement(x + 10, position, veh->rightSensor);	
	imguiEndScrollArea();
}

void UIManager::DrawBarElement(float x, float y, LightSensor& sensor)
{
	int length = 80;
	//draw white line
	imguiDrawLine(x, y, x + length, y, 2, SetRGBA(255, 255, 255, 128));

	for (auto& interval : sensor.m_intervals)
	{
		if (interval.first != interval.second)
		{
			int xStart = x + interval.first * length;
			int xEnd = x + interval.second * length;

			imguiDrawLine(xStart, y, xEnd, y, 2, SetRGBA(15, 255, 199, 192));
		}
	}
}

bool UIManager::InRegion(const b2Vec2 & point)
{
	printf("point: (%f,%f)\n", point.x, point.y);
	for (auto& r : m_regions)
	{
		printf("rectangle: topRight:(%f,%f), bottomLeft:(%f,%f)\n", r.m_topRight.x, r.m_topRight.y, r.m_bottomLeft.x, r.m_bottomLeft.y);
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
