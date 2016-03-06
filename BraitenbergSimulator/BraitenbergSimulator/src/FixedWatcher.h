#pragma once

#include "Watcher.h"

class FixedWatcher : public Watcher
{
public:
	FixedWatcher(std::string name, int sampleRate, int maxStorage, std::function<float()> func, float min, float max);
	float GetMin();
	float GetMax();
private:
	float m_min;
	float m_max;
};