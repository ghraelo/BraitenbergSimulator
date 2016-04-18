#include "VehicleMonitor.h"

#include <ctime>
#include <cassert>
#include <sstream>
#include "YAMLConverters.h"	

VehicleMonitor::VehicleMonitor()
	:m_directoryPath("")
{
}

VehicleMonitor::VehicleMonitor(Vehicle * vehicle, std::string directoryPath)
	:m_vehicle(vehicle), m_directoryPath(directoryPath)
{
	m_timeStamp = GetTimeStamp();
	//open csv
	std::string fileName = m_directoryPath + "/" + m_vehicle->GetName() + m_timeStamp + ".csv";
	m_csvStream.open(fileName);

	//write header row
	std::string headerRow = "events,time,x,y,angle,";

	for (auto& datum : m_vehicle->GetInternalData())
	{
		headerRow = headerRow + datum.first + ",";
	}
	//remove last comma
	headerRow.pop_back();

	m_csvStream << headerRow << "\n";

	prevPos = vehicle->GetPosition();
}

VehicleMonitor::~VehicleMonitor()
{
	m_csvStream.close();
}

void VehicleMonitor::WriteCSV(double elapsedTime)
{
	std::ostringstream row;
	row << ",";
	row << elapsedTime << ",";
	row << m_vehicle->GetPosition().x << ",";
	row << m_vehicle->GetPosition().y << ",";
	row << m_vehicle->m_body->GetAngle() << ",";
	//internal data
	for (auto& datum : m_vehicle->GetInternalData())
	{
		row << datum.second << ",";
	}
	row.str().pop_back();
	m_csvStream << row.str() << "\n";

	m_dist_travelled += b2Distance(prevPos, m_vehicle->GetPosition());
	prevPos = m_vehicle->GetPosition();
}

void VehicleMonitor::AddCollision(BoundaryType type, b2Vec2 position, double time)
{
	BoundaryCollisionData bcd;
	bcd.m_type = type;
	bcd.m_position = position;
	bcd.m_time = time;

	boundaryCollisions.push_back(bcd);
}

YAML::Emitter& operator << (YAML::Emitter& out, const b2Vec2& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
};

YAML::Node VehicleMonitor::GetYAML()
{
	YAML::Node node;
	node["name"] = m_vehicle->GetName();
	node["distance"] = m_dist_travelled;
	node["numeric-data"] = m_vehicle->GetName() + m_timeStamp + ".csv";
	if (boundaryCollisions.size() > 0)
	{

		for (int i = 0; i < (boundaryCollisions.size() < 10000 ? boundaryCollisions.size() : 10000);i++)
		{
			BoundaryCollisionData& collision = boundaryCollisions[i];
			YAML::Node col;
			col["type"] = collision.m_type;
			col["position"] = collision.m_position;
			col["time"] = collision.m_time;

			node["boundary-collisions"].push_back(col);
		}
	}

	return node;
}

std::string VehicleMonitor::GetVehicleName()
{
	return m_vehicle->GetName();
}

void VehicleMonitor::SetIsColliding(bool state)
{
	m_IsColliding = state;
}

Vehicle * VehicleMonitor::GetVehiclePointer()
{
	return m_vehicle;
}

std::string VehicleMonitor::GetTimeStamp()
{
	time_t now = time(0);
	tm* dt = localtime(&now);

	std::ostringstream oss;

	oss << 1900 + dt->tm_year;
	oss << 1 + dt->tm_mon;
	oss << dt->tm_mday;
	oss << "-";
	oss << dt->tm_hour;
	oss << dt->tm_min;

	return oss.str();
}