#include "StatisticsManager.h"

#include <algorithm>

const std::vector<StatisticPtr>& StatisticsManager::GetStats() const
{
	return stats;
}

StatisticsManager::StatisticsManager()
	:m_sampleRate(1)
{
}

StatisticsManager::StatisticsManager(int sampleRate)
	:m_sampleRate(sampleRate)
{
}

void StatisticsManager::Update()
{
	if (stats.size() > 0)
	{
		std::vector<StatisticPtr>::const_iterator max_it = (std::max_element(stats.begin(), stats.end(),
			[&](StatisticPtr& a, StatisticPtr& b) {
			return a->GetSampleRate() < b->GetSampleRate();
		}
		));

		int maxSampleRate = (*max_it)->GetSampleRate();

		accumulator++;

		for (auto& sp : stats)
		{
			if (accumulator % sp->GetSampleRate() == 0)
				sp->Update();
		}

		if (accumulator > maxSampleRate)
		{
			accumulator = 0;
		}
	}
}

void StatisticsManager::AddStat(StatisticPtr stat)
{
	stats.push_back(std::move(stat));
}
