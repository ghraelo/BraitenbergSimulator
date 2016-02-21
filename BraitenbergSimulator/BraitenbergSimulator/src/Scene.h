#pragma once

#include <vector>
#include <string>
#include "Vehicle.h"
#include "LightSource.h"

struct Scene
{
	std::string m_name;
	std::vector<Vehicle> m_vehicles;
	std::vector<LightSource> m_lights;
};

typedef std::unique_ptr<Scene> ScenePtr;