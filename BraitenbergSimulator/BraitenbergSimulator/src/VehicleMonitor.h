#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include <yaml-cpp\yaml.h>

#include "ControlStrategy.h"
#include "Vehicle.h"
#include "Boundary.h"

struct BoundaryCollisionData
{
	BoundaryType m_type;
	b2Vec2 m_position;
	double m_time;
};

struct VehicleData
{
	float m_x;
	float m_y;
	float m_angle;
	std::map<std::string, float> internalState;
};

class VehicleMonitor
{
public:
	VehicleMonitor();
	VehicleMonitor(Vehicle* vehicle, std::string directoryPath);
	~VehicleMonitor();
	void WriteCSV(double elapsedTime);
	void AddCollision(BoundaryType type, b2Vec2 position, double time);
	YAML::Node GetYAML();
	std::string GetVehicleName();
private:
	std::string GetTimeStamp();
	std::ofstream m_csvStream;
	std::vector<BoundaryCollisionData> boundaryCollisions;
	Vehicle* m_vehicle;
	std::string m_directoryPath;
	std::string m_timeStamp;
};

typedef std::unique_ptr<VehicleMonitor> VehicleMonitorPtr;