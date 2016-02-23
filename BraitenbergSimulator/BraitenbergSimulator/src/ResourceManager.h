#pragma once

#include "Scene.h"
#include <string>
#include <yaml-cpp\yaml.h>

class ResourceManager
{
public:
	static ScenePtr LoadScene(std::string fileName);
private:
	static Vehicle LoadVehicle(YAML::Node vehicleNode, std::string vehicleName);
	static LightSource LoadLight(YAML::Node lightNode);
	static bool LoadSensor(YAML::Node& sensorNode, sensorInfo& info);
	ResourceManager();
};