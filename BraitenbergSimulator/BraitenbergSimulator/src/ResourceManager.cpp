#include "ResourceManager.h"
#include "YAMLConverters.h"
#include <iostream>

#include "Shader.h"
#include "Strategies.h"	


ResourceManager::ResourceManager()
{
}
ScenePtr ResourceManager::LoadScene(std::string fileName)
{
	ScenePtr theScene = std::make_unique<Scene>();
	YAML::Node baseNode = YAML::LoadFile(fileName);
	if (baseNode.IsNull())
		throw std::exception("yaml file failed to load\n");
	printf("file loaded\n");
	
	if (baseNode["name"].IsNull())
		throw std::exception("name node is null\n");

	const std::string theString = baseNode["name"].as<std::string>();

	theScene->m_name = theString;
	printf("Scene name: %s\n",theString.c_str());

	
	//get vehicles
	int vehicleNum = baseNode["vehicles"].size();
	printf("Attempting to load %d vehicles...\n", vehicleNum);
	for (int i = 0; i < vehicleNum; i++)
	{
		std::string vehicleName = baseNode["vehicles"][i].as<std::string>();
		YAML::Node vehicleNode = baseNode[vehicleName];
		VehiclePtr v = LoadVehicle(vehicleNode, vehicleName);
		theScene->m_vehicles.push_back(std::move(v));
	}
	printf("Done!\n");
	//get lights
	int lightNum = baseNode["lights"].size();
	printf("Attempting to load %d lights...\n", lightNum);
	for (int i = 0; i < lightNum; i++)
	{
		std::string lightName = baseNode["lights"][i].as<std::string>();
		YAML::Node lightNode = baseNode[lightName];
		theScene->m_lights.push_back(LoadLight(lightNode));
	}
	printf("Done!\n");
	return theScene;
}

VehiclePtr ResourceManager::LoadVehicle(YAML::Node vehicleNode, std::string vehicleName)
{
	b2Vec2 vehicle_pos;

	if (vehicleNode.IsNull())
		throw std::exception("YAML:Vehicle node not found");

	//get position
	vehicle_pos = vehicleNode["position"].as<b2Vec2>();

	//get left sensor
	YAML::Node leftSensorNode = vehicleNode["leftsensor"];
	sensorInfo leftInfo;
	LoadSensor(leftSensorNode, leftInfo);


	//get right sensor
	YAML::Node rightSensorNode = vehicleNode["rightsensor"];
	sensorInfo rightInfo;
	LoadSensor(rightSensorNode, rightInfo);

	//get strategy type
	YAML::Node stratNode = vehicleNode["strategy"];
	std::string strat_type = stratNode["type"].as<std::string>();

	ControlStrategyPtr cs_ptr;

	if (strat_type == "csl")
	{
		float gi = stratNode["gi"].as<float>();
		float gf = stratNode["gf"].as<float>();
		cs_ptr = std::make_unique<CSLStrategy>(gi, gf);
	}
	else if (strat_type == "2a")
	{
		cs_ptr = std::make_unique<TwoAStrategy>();
	}
	else if (strat_type == "2b")
	{
		cs_ptr = std::make_unique<TwoBStrategy>();
	}

	//got all information, create vehicle
	return std::make_unique<Vehicle>(leftInfo, rightInfo, cs_ptr, vehicleName);
}

LightSource ResourceManager::LoadLight(YAML::Node lightNode)
{
	b2Vec2 light_pos;
	float light_radius;

	if (lightNode.IsNull())
		throw std::exception("YAML:Vehicle node not found");

	//get position,radius
	light_pos = lightNode["position"].as<b2Vec2>();
	light_radius = lightNode["radius"].as<float>();

	//got all information, create light
	return LightSource(light_pos, light_radius);
}

bool ResourceManager::LoadSensor(YAML::Node& sensorNode, sensorInfo& info)
{
	if (sensorNode.IsNull() || sensorNode["offset"].IsNull() || sensorNode["apertureangle"].IsNull() || sensorNode["direction"].IsNull())
		return false; //left sensor info

					  //create sensorInfo struct
	info.m_offset = sensorNode["offset"].as<b2Vec2>();
	info.m_aperture = sensorNode["apertureAngle"].as<float>();
	info.m_direction = sensorNode["direction"].as<b2Vec2>();

	return true;
}

std::map<std::string, Shader> ResourceManager::LoadShaders()
{
	std::map<std::string, Shader> temp;
	YAML::Node baseNode = YAML::LoadFile("yaml/config.yaml");

	//get shaders
	int shaderNum = baseNode["Shaders"].size();
	printf("Attempting to load %d shaders...\n", shaderNum);
	for (int i = 0; i < shaderNum; i++)
	{
		std::string shaderId = baseNode["Shaders"][i].as<std::string>();
		YAML::Node shader = baseNode[shaderId];
		std::string fragPath = "shaders/" + shader["FragmentName"].as<std::string>();
		std::string vertexPath = "shaders/" + shader["VertexName"].as<std::string>();

		Shader theShader(vertexPath.c_str(), fragPath.c_str());
		temp.insert(std::pair<std::string, Shader>(shaderId, theShader));
	}
	printf("Done!\n");
	return temp;
}