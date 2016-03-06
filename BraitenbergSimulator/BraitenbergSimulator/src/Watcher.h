#pragma once

//STL
#include <boost\circular_buffer.hpp>
#include <functional>

#include "Statistic.h"

class Watcher : public Statistic
{
public:
	Watcher(std::string name, int sampleRate, int maxStorage, std::function<float()> func);
	void Update() override;
	const boost::circular_buffer<float>& GetData();
	void Clear();
protected:
	std::function<float()> m_func;
	boost::circular_buffer<float> m_internal_data;
};