#include "MonitorManager.h"

#include <yaml-cpp\yaml.h>
#include <ctime>
#include "VehicleMonitor.h"
#include "LightMonitor.h"
#include "Monitor.h"

MonitorManager::MonitorManager()
{
}

MonitorManager::MonitorManager(std::string directoryPath)
	:m_directoryPath(directoryPath)
{
}

void MonitorManager::Init(const Scene * theScene)
{
	m_scene = theScene;

	m_sceneFile = theScene->m_fileName;
	for (auto& vehicle : theScene->m_vehicles)
	{
		MonitorPtr monitor = std::make_unique<VehicleMonitor>(vehicle.get(), m_directoryPath);
		m_monitors.push_back(std::move(monitor));
	}

	for (auto& light : m_scene->m_lights)
	{
		MonitorPtr monitor = std::make_unique<LightMonitor>(light.get(), m_directoryPath);
		m_monitors.push_back(std::move(monitor));
	}
}

void MonitorManager::RecordCSV(double elapsedTime)
{
	for (auto& monitor : m_monitors)
	{
		monitor->WriteCSV(elapsedTime);
	}
}

void MonitorManager::SaveYAML()
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "scene-file";
	out << YAML::Value << m_sceneFile;
	out << YAML::Key << "vehicle-data";
	out << YAML::Value << YAML::BeginSeq;

	for (auto& monitor : m_monitors)
	{
		if (dynamic_cast<VehicleMonitor*>(monitor.get()) != nullptr)
		{
			out << monitor->GetYAML();
		}
	}
	out << YAML::EndSeq;

	out << YAML::Key << "light-data";
	out << YAML::Value << YAML::BeginSeq;

	for (auto& monitor : m_monitors)
	{
		if (dynamic_cast<LightMonitor*>(monitor.get()) != nullptr)
		{
			out << monitor->GetYAML();
		}
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	std::string x = m_sceneFile;
	x = x.substr(0,x.find_last_of('.'));

	std::ofstream yamlFile;
	yamlFile.open(m_directoryPath + "/" + x + "-" + GetTimeStamp() + ".yaml");
	yamlFile << "---\n";
	yamlFile << out.c_str();
	yamlFile << "\n...\n";
	yamlFile.close();
}

std::string MonitorManager::GetTimeStamp()
{
	time_t now = time(0);
	tm* dt = localtime(&now);

	std::ostringstream oss;

	oss << 1900 + dt->tm_year;
	oss << 1 + dt->tm_mon;
	oss << dt->tm_mday;
	oss << "-";
	oss << dt->tm_hour;
	oss << dt->tm_min;

	return oss.str();
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
	std::vector<MonitorPtr>::iterator collided = std::find_if(m_monitors.begin(), m_monitors.end(),
		[&](MonitorPtr& monitor_ptr) {
			return (monitor_ptr->GetName() == vehicle->GetName());
		}
	);

	VehicleMonitor* vehMon = dynamic_cast<VehicleMonitor*>((*collided).get());
	if (vehMon != nullptr)
	{
		if (collided != m_monitors.end())
		{
			vehMon->AddCollision(type, position, time);
		}
		else
		{
			printf("error: vehicle not found\n");
		}
	}
}
