#pragma once

#include "SimObject.h"
#include "Vehicle.h"
#include  "Rectangle.h"
class UIManager
{
public:
	UIManager();
	void DrawUI();
	void SelectVehicle(Vehicle* obj);
	void DeselectVehicle();
	void DrawVehicleUI(Vehicle* veh);
	void DrawBarElement(float x, float y, LightSensor& sensor);
	bool InRegion(const b2Vec2& point);
private:
	 Vehicle* m_selectedVehicle;
	 std::vector<Rectangle> m_regions;
};