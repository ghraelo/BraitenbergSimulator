#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <yaml-cpp\yaml.h>

#include "SimObject.h"

class Monitor
{
public:
	Monitor();
	Monitor(SimObject* simObject, std::string directoryPath);
	~Monitor();
	virtual void WriteCSV(double elapsedTime) = 0;
	virtual YAML::Node GetYAML() = 0;
	std::string GetName();
	SimObject* GetPointer(); //get pointer to monitored object
protected:
	std::string GetTimeStamp();
	std::ofstream m_csvStream;
	SimObject* m_simObject;
	std::string m_directoryPath;
	std::string m_timeStamp;
};

typedef std::unique_ptr<Monitor> MonitorPtr;