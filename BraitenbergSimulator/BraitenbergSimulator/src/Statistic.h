#pragma once

#include <string>
#include <vector>
#include <memory>

class Statistic
{
public:
	Statistic();
	Statistic(std::string name, int sampleRate);
	int GetSampleRate();
	std::string GetName();
	virtual void Update() = 0;
protected:
	int m_sampleRate = 1;
	std::string m_name;
};

typedef std::unique_ptr<Statistic> StatisticPtr;