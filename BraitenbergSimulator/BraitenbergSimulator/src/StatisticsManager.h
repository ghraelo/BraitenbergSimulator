#pragma once

#include <vector>
#include "Statistic.h"

class StatisticsManager
{
public:
	StatisticsManager();
	StatisticsManager(int sampleRate);
	void Update();
	void AddStat(StatisticPtr stat);
	const std::vector<StatisticPtr>& GetStats() const;
private:
	int m_sampleRate;
	int accumulator = 0;
	std::vector<StatisticPtr> stats;
};