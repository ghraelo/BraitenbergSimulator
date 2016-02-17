#pragma once

#include <string>
#include "Vehicle.h"
#include <yaml-cpp\yaml.h>

class ConfigLoader
{
public:
	ConfigLoader();
	ConfigLoader(b2World* world);
	bool OpenFile(std::string fileName);
	Vehicle LoadVehicle();

protected:
	bool ConfigLoader::LoadSensor(YAML::Node& sensorNode, sensorInfo& info);

private:
	YAML::Node m_fileBaseNode;
	b2World* world_ptr;
};