#include "Monitor.h"

#include <ctime>
#include <cassert>
#include <sstream>

Monitor::Monitor()
	:m_directoryPath("")
{
}

Monitor::Monitor(SimObject * simObject, std::string directoryPath)
	:m_simObject(simObject), m_directoryPath(directoryPath), m_timeStamp(GetTimeStamp())
{
	//open csv
	std::string fileName = m_directoryPath + "/" + simObject->GetName() + m_timeStamp + ".csv";
	m_csvStream.open(fileName);
}

Monitor::~Monitor()
{
	m_csvStream.close();
}

std::string Monitor::GetName()
{
	return m_simObject->GetName();
}

SimObject * Monitor::GetPointer()
{
	return m_simObject;
}

std::string Monitor::GetTimeStamp()
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
