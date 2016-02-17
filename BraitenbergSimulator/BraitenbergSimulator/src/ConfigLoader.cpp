#include "ConfigLoader.h"


#include <iostream>
#include <fstream>
#include <Windows.h>

#include "YAMLConverters.h"
#include "Vehicle.h"

ConfigLoader::ConfigLoader()
{

}

ConfigLoader::ConfigLoader(b2World* world)
	:world_ptr(world)
{

}

bool ConfigLoader::OpenFile(std::string fileName)
{
	m_fileBaseNode = YAML::LoadFile(fileName);

	if (m_fileBaseNode.IsNull())
	{
		return false;
	}
	else
	{
		return true;
	}
}

Vehicle ConfigLoader::LoadVehicle()
{
	b2Vec2 vehicle_pos;

	YAML::Node vehicleNode = m_fileBaseNode["Vehicle"];
	if (vehicleNode.IsNull())
		throw std::exception("YAML:Vehicle node not found");

	//get position
	vehicle_pos = vehicleNode["Position"].as<b2Vec2>();

	//get left sensor
	YAML::Node leftSensorNode = vehicleNode["LeftSensor"];
	if (leftSensorNode.IsNull())
		throw std::exception("YAML:Left sensor node not found");

	sensorInfo leftInfo;
	if (!LoadSensor(leftSensorNode, leftInfo))
		throw std::exception("YAML:Left sensor load failed");


	//get right sensor
	YAML::Node rightSensorNode = vehicleNode["RightSensor"];
	if (rightSensorNode.IsNull())
		throw std::exception("YAML:Right sensor load failed");

	sensorInfo rightInfo;
	if (!LoadSensor(rightSensorNode, rightInfo))
	{
		return false; //left load failed
	}

	//got all information, create vehicle
	return Vehicle(world_ptr, leftInfo, rightInfo);
}

bool ConfigLoader::LoadSensor(YAML::Node& sensorNode, sensorInfo& info)
{
	if (sensorNode.IsNull() || sensorNode["Offset"].IsNull() || sensorNode["ApertureAngle"].IsNull() || sensorNode["Direction"].IsNull())
		return false; //left sensor info

					  //create sensorInfo struct
	info.m_offset = sensorNode["Offset"].as<b2Vec2>();
	info.m_aperture = sensorNode["ApertureAngle"].as<float>();
	info.m_direction = sensorNode["Direction"].as<b2Vec2>();

	return true;
}