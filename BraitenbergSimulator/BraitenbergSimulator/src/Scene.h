#pragma once

#include <vector>
#include <string>
#include "Vehicle.h"
#include "LightSource.h"
#include "Obstacle.h"

struct Scene
{
	std::string m_name;
	std::string m_fileName;
	std::vector<VehiclePtr> m_vehicles;
	std::vector<LightSourcePtr> m_lights;
	std::vector<ObstaclePtr> m_obstacles;
};

typedef std::unique_ptr<Scene> ScenePtr;