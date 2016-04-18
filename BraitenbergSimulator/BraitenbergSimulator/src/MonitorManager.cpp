#include "MonitorManager.h"

#include <yaml-cpp\yaml.h>
#include <ctime>

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
		VehicleMonitorPtr monitor = std::make_unique<VehicleMonitor>(vehicle.get(), m_directoryPath);
		m_monitors.push_back(std::move(monitor));
	}

	for (auto& light : m_scene->m_lights)
	{
		MinDistData m;
		m.minDist = 200000.0f;
		m_lightMinDistances[light->GetName()] = m;
	}
}

void MonitorManager::RecordCSV(double elapsedTime)
{
	for (auto& monitor : m_monitors)
	{
		monitor->WriteCSV(elapsedTime);
	}

	for (auto& light : m_scene->m_lights)
	{
		float min_dist = m_lightMinDistances[light->GetName()].minDist;
		std::string vehicleName;
		for (auto& vehicle : m_scene->m_vehicles)
		{
			vehicleName = vehicle->GetName();
			float dist = b2Distance(light->GetPosition(), vehicle->GetPosition());
			//printf("dist: %f \n", dist);
			if (dist < min_dist)
			{
				min_dist = dist;

			}
		}
		MinDistData m;
		m.minDist = min_dist;
		m.vehicleName = vehicleName;

		m_lightMinDistances[light->GetName()] = m;

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
		out << monitor->GetYAML();
	}
	out << YAML::EndSeq;

	out << YAML::Key << "min-light-dists";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& light : m_scene->m_lights)
	{
		out << YAML::BeginMap;
		std::string name = light->GetName();
		out << YAML::Key << "light-name";
		out << YAML::Value << name;

		out << YAML::Key << "min-dist";
		out << YAML::Value << m_lightMinDistances[name].minDist;

		out << YAML::Key << "vehicle-name";
		out << YAML::Value << m_lightMinDistances[name].vehicleName;
		out << YAML::EndMap;
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
