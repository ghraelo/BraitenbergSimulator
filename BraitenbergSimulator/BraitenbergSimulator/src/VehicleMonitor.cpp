#include "VehicleMonitor.h"

#include <ctime>
#include <cassert>
#include <sstream>
#include "YAMLConverters.h"	

VehicleMonitor::VehicleMonitor()
	:Monitor(), vcl(this)
{
}

VehicleMonitor::VehicleMonitor(Vehicle * vehicle, std::string directoryPath)
	:Monitor(vehicle,directoryPath), vcl(this)
{

	//write header row
	std::string headerRow = "events,time,x,y,angle,";

	for (auto& datum : ((Vehicle*)m_simObject)->GetInternalData())
	{
		headerRow = headerRow + datum.first + ",";
	}
	//remove last comma
	headerRow.pop_back();

	m_csvStream << headerRow << "\n";

	prevPos = ((Vehicle*)m_simObject)->GetPosition();

	((Vehicle*)m_simObject)->GetWorld()->SetContactListener(&vcl);
}

void VehicleMonitor::WriteCSV(double elapsedTime)
{
	std::ostringstream row;
	//events
	if (m_IsColliding == true)
		row << "C:" << m_obstacleName << ";";
	else
		row << ",";

	row << elapsedTime << ",";
	row << m_simObject->GetPosition().x << ",";
	row << m_simObject->GetPosition().y << ",";
	row << ((Vehicle*)m_simObject)->m_body->GetAngle() << ",";
	//internal data
	for (auto& datum : ((Vehicle*)m_simObject)->GetInternalData())
	{
		row << datum.second << ",";
	}
	row.str().pop_back();
	m_csvStream << row.str() << "\n";

	m_dist_travelled += b2Distance(prevPos, m_simObject->GetPosition());
	prevPos = m_simObject->GetPosition();
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
	node["name"] = m_simObject->GetName();
	node["distance"] = m_dist_travelled;
	node["numeric-data"] = m_simObject->GetName() + m_timeStamp + ".csv";
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

void VehicleMonitor::SetIsColliding(bool state)
{
	m_IsColliding = state;
}

void VehicleMonitor::SetObstacleName(std::string name)
{
	m_obstacleName = name;
}