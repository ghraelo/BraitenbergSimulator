#include "MonitorManager.h"

MonitorManager::MonitorManager()
{
}

MonitorManager::MonitorManager(std::string directoryPath)
	:m_directoryPath(directoryPath)
{
}

void MonitorManager::Init(const Scene * theScene)
{
	for (auto& vehicle : theScene->m_vehicles)
	{
		VehicleMonitorPtr monitor = std::make_unique<VehicleMonitor>(vehicle.get(), m_directoryPath,theScene->m_fileName);
		m_monitors.push_back(std::move(monitor));
	}
}

void MonitorManager::RecordCSV()
{
	for (auto& monitor : m_monitors)
	{
		monitor->WriteCSV();
	}
}

void MonitorManager::SaveYAML()
{
	for (auto& monitor : m_monitors)
	{
		monitor->WriteYAML();
	}
}

void MonitorManager::Close()
{
	for (auto& monitor : m_monitors)
	{
		//destroy monitors, closing streams
		monitor.reset();
	}
}

void MonitorManager::AddBoundaryCollision(BoundaryType type, b2Vec2 position, double time, Vehicle * vehicle)
{
	std::vector<VehicleMonitorPtr>::iterator collided = std::find_if(m_monitors.begin(), m_monitors.end(),
		[&](VehicleMonitorPtr& monitor_ptr) {
			return (monitor_ptr->GetVehicleName() == vehicle->GetName());
		}
	);

	if (collided != m_monitors.end())
		(*collided)->AddCollision(type, position, time);
	else
		printf("error: vehicle not found\n");
}
