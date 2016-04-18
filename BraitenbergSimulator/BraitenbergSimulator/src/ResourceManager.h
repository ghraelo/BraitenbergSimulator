#pragma once

#include "Scene.h"
#include <string>
#include <yaml-cpp\yaml.h>
#include <map>

class Shader;

class ResourceManager
{
public:
	static ScenePtr LoadScene(std::string fileName);
	static void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory);
private:
	static VehiclePtr LoadVehicle(YAML::Node vehicleNode);
	static LightSourcePtr LoadLight(YAML::Node lightNode);
	static ObstaclePtr LoadObstacle(YAML::Node obstacleNode);
	static bool LoadSensor(YAML::Node& sensorNode, sensorInfo& info);

	ResourceManager();
};