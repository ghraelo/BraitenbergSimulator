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
	void RecordCSV(double elapsedTime);
	void SaveYAML();
	void Close();
	void AddBoundaryCollision(BoundaryType type, b2Vec2 position, double time, Vehicle* vehicle);
private:
	std::string GetTimeStamp();
	std::string m_directoryPath;
	std::string m_sceneFile;
	std::vector<VehicleMonitorPtr> m_monitors;
};