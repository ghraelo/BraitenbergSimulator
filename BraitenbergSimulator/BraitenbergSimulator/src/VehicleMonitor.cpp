#include "VehicleMonitor.h"

#include <ctime>
#include <cassert>
#include <sstream>

VehicleMonitor::VehicleMonitor()
	:m_directoryPath("")
{
}

VehicleMonitor::VehicleMonitor(Vehicle * vehicle, std::string directoryPath, std::string sceneFileName)
	:m_vehicle(vehicle), m_directoryPath(directoryPath), m_fileName(sceneFileName)
{
	m_timeStamp = GetTimeStamp();
	//open csv
	std::string fileName = m_directoryPath + "/" + m_vehicle->GetName() + m_timeStamp + ".csv";
	m_csvStream.open(fileName);

	//write header row
	std::string headerRow = "events,x,y,angle,";

	for (auto& datum : m_vehicle->GetInternalData())
	{
		headerRow = headerRow + datum.first + ",";
	}
	//remove last comma
	headerRow.pop_back();

	m_csvStream << headerRow << "\n";
}

VehicleMonitor::~VehicleMonitor()
{
	m_csvStream.close();
}

void VehicleMonitor::WriteCSV()
{
	std::ostringstream row;
	row << ",";
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

void VehicleMonitor::WriteYAML()
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "scene";
	out << YAML::Value << m_fileName;
	out << YAML::Key << "vehicle";
	out << YAML::Value << m_vehicle->GetName();
	out << YAML::Key << "numericData";
	out << YAML::Value << (m_vehicle->GetName() + m_timeStamp + ".csv");
	out << YAML::Key << "boundaryCollisions" << YAML::Value << YAML::BeginSeq;

	for (auto& collision : boundaryCollisions)
	{
		out << YAML::BeginMap;

		out << YAML::Key << "type";
		out << YAML::Value << collision.m_type;

		out << YAML::Key << "position";
		out << YAML::Value << collision.m_position;

		out << YAML::Key << "time";
		out << YAML::Value << collision.m_time;

		out << YAML::EndMap;
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream yamlFile;
	yamlFile.open(m_directoryPath + "/" + m_vehicle->GetName() + m_timeStamp + ".yaml");
	yamlFile << out.c_str();
	yamlFile.close();
	
}

std::string VehicleMonitor::GetVehicleName()
{
	return m_vehicle->GetName();
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