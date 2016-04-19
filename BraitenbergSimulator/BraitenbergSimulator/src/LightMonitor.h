#pragma once

#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include <yaml-cpp\yaml.h>

#include "LightSource.h"
#include "Monitor.h"

class LightMonitor : public Monitor
{
public:
	LightMonitor();
	LightMonitor(LightSource* light, std::string directoryPath);
	void WriteCSV(double elapsedTime) override;
	YAML::Node GetYAML() override;
private:
};

typedef std::unique_ptr<LightMonitor> LightMonitorPtr;