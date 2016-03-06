#pragma once

#include <vector>

#include "Statistic.h"
#include "Watcher.h"

class Processor : public Statistic
{
public:
	Processor(std::string name, Watcher* watcherToProcess);
	virtual void Update() = 0;
	const std::vector<float>& GetData();
	void Clear();
protected:
	Watcher* m_watcher;
	std::vector<float> m_internal_data;
};