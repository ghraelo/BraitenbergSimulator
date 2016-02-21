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
	b2Vec2 vehScreenCoords = g_camera.ConvertWorldToScreen(m_selectedVehicle->GetPosition());
	//std::cout << "vehicle screen coords: (" << vehScreenCoords.x << ", " << vehScreenCoords.y << ")" << std::endl;
	int x = (int)vehScreenCoords.x + 20;
	int y = (int)g_camera.m_height - vehScreenCoords.y + 20;

	int position = y + 85;

	imguiDrawRoundedRect(x, y, 100, 100, 6, imguiRGBA(0, 0, 0, 192));
	imguiDrawText(x + 5, position, TEXT_ALIGN_LEFT, "Vehicle:", SetRGBA(255, 255, 255, 128));
	position -= 20;
	imguiDrawText(x + 5, position, TEXT_ALIGN_LEFT, "Left Sensor:", SetRGBA(255, 255, 255, 128));
	position -= 10;
	DrawBarElement(x + 10, position, veh->leftSensor);
	position -= 20;
	imguiDrawText(x + 5, position, TEXT_ALIGN_LEFT, "Right Sensor:", SetRGBA(255, 255, 255, 128));
	position -= 10;
	DrawBarElement(x + 10, position, veh->rightSensor);
	
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

void UIManager::SelectVehicle(Vehicle* obj)
{
	m_selectedVehicle = obj;
}

void UIManager::DeselectVehicle()
{
	m_selectedVehicle = nullptr;
}
