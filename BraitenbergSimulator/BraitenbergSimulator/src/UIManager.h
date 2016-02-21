#pragma once

#include "SimObject.h"
#include "Vehicle.h"

class UIManager
{
public:
	UIManager();
	void DrawUI();
	void SelectVehicle(Vehicle* obj);
	void DeselectVehicle();
	void DrawVehicleUI(Vehicle* veh);
	void DrawBarElement(float x, float y, LightSensor& sensor);
private:
	 Vehicle* m_selectedVehicle;
};