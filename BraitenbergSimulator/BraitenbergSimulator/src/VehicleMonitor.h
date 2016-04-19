#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include <yaml-cpp\yaml.h>

#include "ControlStrategy.h"
#include "Vehicle.h"
#include "Boundary.h"
#include "VehicleContactListener.h"
#include "Monitor.h"

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

class VehicleMonitor : public Monitor
{
public:
	VehicleMonitor();
	VehicleMonitor(Vehicle* vehicle, std::string directoryPath);
	void WriteCSV(double elapsedTime) override;
	void AddCollision(BoundaryType type, b2Vec2 position, double time);
	YAML::Node GetYAML() override;
	void SetIsColliding(bool state);
	void SetObstacleName(std::string name);
private:
	std::vector<BoundaryCollisionData> boundaryCollisions;
	float m_dist_travelled = 0.0f;
	b2Vec2 prevPos;
	bool m_IsColliding = false;
	bool m_prevIsColliding = false;
	std::string m_obstacleName;
	VehicleContactListener vcl;
};

typedef std::unique_ptr<VehicleMonitor> VehicleMonitorPtr;