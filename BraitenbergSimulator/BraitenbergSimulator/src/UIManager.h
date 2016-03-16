#pragma once

#include "SimObject.h"
#include "Vehicle.h"
#include  "Rectangle.h"
#include "BaseUISettings.h"
#include "WMState.h"

#include "StatisticsManager.h"
#include "Watcher.h"
#include "FixedWatcher.h"
class UIManager
{
public:
	UIManager();
	void DrawUI(const StatisticsManager& sm);
	void SelectVehicle(Vehicle* obj);
	void DeselectVehicle();
	void DrawVehicleUI(Vehicle* veh);
	void DrawBaseUI(BaseUISettings& settings, const WindowState& ws);
	void DrawStatsPane(const StatisticsManager& sm, const WindowState& ws);
	bool InRegion(const b2Vec2& point);
private:
	 Vehicle* m_selectedVehicle;
	 std::vector<Rectangle> m_regions;
};