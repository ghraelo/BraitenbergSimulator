#include "FixedWatcher.h"

FixedWatcher::FixedWatcher(std::string name, int sampleRate, int maxStorage, std::function<float()> func, float min, float max)
	:Watcher(name,sampleRate,maxStorage,func),m_min(min),m_max(max)
{
}

float FixedWatcher::GetMin()
{
	return m_min;
}

float FixedWatcher::GetMax()
{
	return m_max;
}
