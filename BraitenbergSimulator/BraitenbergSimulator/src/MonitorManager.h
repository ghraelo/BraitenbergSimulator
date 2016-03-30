#pragma once

#include "Scene.h"
#include "VehicleMonitor.h"
#include "Boundary.h"

class MonitorManager
{
public:
	MonitorManager();
	MonitorManager(std::string directoryPath);
	void Init(const Scene* theScene);
	void RecordCSV();
	void SaveYAML();
	void Close();
	void AddBoundaryCollision(BoundaryType type, b2Vec2 position, double time, Vehicle* vehicle);
private:
	std::string m_directoryPath;
	std::vector<VehicleMonitorPtr> m_monitors;
};