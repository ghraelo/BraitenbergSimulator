#include "ResourceManager.h"
#include "YAMLConverters.h"
#include <iostream>

//#include "Shader.h"
#include "Strategies.h"	

#include <Windows.h>

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
	theScene->m_fileName = fileName.substr(fileName.find_last_of('/')+1, std::string::npos);
	printf("Scene name: %s\n",theString.c_str());
	printf("File name: %s\n", theScene->m_fileName.c_str());
	
	//get vehicles
	YAML::Node vehicleNode = baseNode["vehicles"];
	assert(vehicleNode.IsSequence());

	printf("Attempting to load %d vehicles...\n", vehicleNode.size());
	for(auto& vehicle : vehicleNode)
	{
		VehiclePtr v = LoadVehicle(vehicle);
		theScene->m_vehicles.push_back(std::move(v));
	}
	printf("Done!\n");

	//get lights
	YAML::Node lightNode = baseNode["lights"];
	assert(lightNode.IsSequence());

	printf("Attempting to load %d lights...\n", lightNode.size());
	for(auto& light : lightNode)
	{
		theScene->m_lights.push_back(LoadLight(light));
	}
	printf("Done!\n");
	return theScene;
}

VehiclePtr ResourceManager::LoadVehicle(YAML::Node vehicleNode)
{
	b2Vec2 vehicle_pos;
	std::string name;
	if (vehicleNode.IsNull())
		throw std::exception("YAML:Vehicle node not found");

	assert(vehicleNode.IsMap());

	//get name
	name = vehicleNode["name"].as<std::string>();

	//get position
	vehicle_pos = vehicleNode["position"].as<b2Vec2>();

	//get left sensor
	YAML::Node leftSensorNode = vehicleNode["left-sensor"];
	sensorInfo leftInfo;
	LoadSensor(leftSensorNode, leftInfo);


	//get right sensor
	YAML::Node rightSensorNode = vehicleNode["right-sensor"];
	sensorInfo rightInfo;
	LoadSensor(rightSensorNode, rightInfo);

	//get strategy type
	YAML::Node stratNode = vehicleNode["strategy"];
	std::string strat_type = stratNode["type"].as<std::string>();

	ControlStrategyPtr cs_ptr;

	if (strat_type == "singlecsl")
	{
		float gi = stratNode["gi"].as<float>();
		float gf = stratNode["gf"].as<float>();
		cs_ptr = std::make_unique<SingleCSLStrategy>(gi, gf);
	}
	else if (strat_type == "csl")
	{
		float gi = stratNode["gi"].as<float>();
		float gf = stratNode["gf"].as<float>();
		cs_ptr = std::make_unique<CSLStrategy>(gi, gf);
	}
	else if (strat_type == "csl2")
	{
		float gi = stratNode["gi"].as<float>();
		float gf = stratNode["gf"].as<float>();
		cs_ptr = std::make_unique<CSL2Strategy>(gi, gf);
	}
	else if (strat_type == "crossedcsl")
	{
		float gi = stratNode["gi"].as<float>();
		float gf = stratNode["gf"].as<float>();
		cs_ptr = std::make_unique<CrossedCSLStrategy>(gi, gf);
	}
	else if (strat_type == "2a")
	{
		cs_ptr = std::make_unique<TwoAStrategy>();
	}
	else if (strat_type == "2b")
	{
		cs_ptr = std::make_unique<TwoBStrategy>();
	}
	else if (strat_type == "3a")
	{
		cs_ptr = std::make_unique<ThreeAStrategy>();
	}
	else if (strat_type == "3b")
	{
		cs_ptr = std::make_unique<ThreeBStrategy>();
	}

	//got all information, create vehicle
	return std::make_unique<Vehicle>(leftInfo, rightInfo, cs_ptr,name);
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
	info.m_aperture = sensorNode["aperture-angle"].as<float>();
	info.m_direction = sensorNode["direction"].as<b2Vec2>();

	return true;
}
/*
http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
*/
void ResourceManager::GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
} // GetFilesInDirectory