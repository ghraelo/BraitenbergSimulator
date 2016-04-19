#include "LightMonitor.h"

LightMonitor::LightMonitor()
	:Monitor()
{
}

LightMonitor::LightMonitor(LightSource * light, std::string directoryPath)
	:Monitor(light,directoryPath)
{
	//write header row
	std::string headerRow = "time,x,y";

	m_csvStream << headerRow << "\n";
}

void LightMonitor::WriteCSV(double elapsedTime)
{
	std::ostringstream row;

	row << elapsedTime << ",";
	row << m_simObject->GetPosition().x << ",";
	row << m_simObject->GetPosition().y;
	m_csvStream << row.str() << "\n";
}

YAML::Node LightMonitor::GetYAML()
{
	YAML::Node node;
	node["name"] = m_simObject->GetName();
	node["numeric-data"] = m_simObject->GetName() + m_timeStamp + ".csv";

	return node;
}

