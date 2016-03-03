#include "ResourceManager.h"
#include "YAMLConverters.h"
#include <iostream>

#include "Shader.h"

ResourceManager::ResourceManager()
{
}
ScenePtr ResourceManager::LoadScene(std::string fileName)
{
	Scene theScene;
	YAML::Node baseNode = YAML::LoadFile(fileName);
	if (baseNode.IsNull())
		throw std::exception("yaml file failed to load\n");
	printf("file loaded\n");
	
	if (baseNode["Name"].IsNull())
		throw std::exception("name node is null\n");

	const std::string theString = baseNode["Name"].as<std::string>();

	theScene.m_name = theString;
	printf("Scene name: %s\n",theString.c_str());

	
	//get vehicles
	int vehicleNum = baseNode["Vehicles"].size();
	printf("Attempting to load %d vehicles...\n", vehicleNum);
	for (int i = 0; i < vehicleNum; i++)
	{
		std::string vehicleName = baseNode["Vehicles"][i].as<std::string>();
		YAML::Node vehicleNode = baseNode[vehicleName];
		Vehicle v = LoadVehicle(vehicleNode, vehicleName);
		theScene.m_vehicles.push_back(v);
	}
	printf("Done!\n");
	//get lights
	int lightNum = baseNode["Lights"].size();
	printf("Attempting to load %d lights...\n", lightNum);
	for (int i = 0; i < lightNum; i++)
	{
		std::string lightName = baseNode["Lights"][i].as<std::string>();
		YAML::Node lightNode = baseNode[lightName];
		theScene.m_lights.push_back(LoadLight(lightNode));
	}
	printf("Done!\n");
	return ScenePtr(std::make_unique<Scene>(theScene));
}

Vehicle ResourceManager::LoadVehicle(YAML::Node vehicleNode, std::string vehicleName)
{
	b2Vec2 vehicle_pos;

	if (vehicleNode.IsNull())
		throw std::exception("YAML:Vehicle node not found");

	//get position
	vehicle_pos = vehicleNode["Position"].as<b2Vec2>();

	//get left sensor
	YAML::Node leftSensorNode = vehicleNode["LeftSensor"];
	sensorInfo leftInfo;
	LoadSensor(leftSensorNode, leftInfo);


	//get right sensor
	YAML::Node rightSensorNode = vehicleNode["RightSensor"];
	sensorInfo rightInfo;
	LoadSensor(rightSensorNode, rightInfo);

	float gi = vehicleNode["GI"].as<float>();
	float gf = vehicleNode["GF"].as<float>();

	//got all information, create vehicle
	return Vehicle(leftInfo, rightInfo,gf,gi,vehicleName,vehicle_pos);
}

LightSource ResourceManager::LoadLight(YAML::Node lightNode)
{
	b2Vec2 light_pos;
	float light_radius;

	if (lightNode.IsNull())
		throw std::exception("YAML:Vehicle node not found");

	//get position,radius
	light_pos = lightNode["Position"].as<b2Vec2>();
	light_radius = lightNode["Radius"].as<float>();

	//got all information, create vehicle
	return LightSource(light_pos, light_radius);
}

bool ResourceManager::LoadSensor(YAML::Node& sensorNode, sensorInfo& info)
{
	if (sensorNode.IsNull() || sensorNode["Offset"].IsNull() || sensorNode["ApertureAngle"].IsNull() || sensorNode["Direction"].IsNull())
		return false; //left sensor info

					  //create sensorInfo struct
	info.m_offset = sensorNode["Offset"].as<b2Vec2>();
	info.m_aperture = sensorNode["ApertureAngle"].as<float>();
	info.m_direction = sensorNode["Direction"].as<b2Vec2>();

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