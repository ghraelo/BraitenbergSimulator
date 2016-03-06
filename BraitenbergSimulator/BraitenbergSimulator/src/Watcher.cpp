#include "Watcher.h"


Watcher::Watcher(std::string name, int sampleRate, int maxStorage, std::function<float()> func)
	:Statistic(name, sampleRate), m_func(func), m_internal_data(maxStorage)
{
	for (int i = 0; i < m_internal_data.capacity(); i++)
	{
		m_internal_data.push_back(0);
	}
}

void Watcher::Update()
{
	m_internal_data.push_back(m_func());
}

const boost::circular_buffer<float>& Watcher::GetData()
{
	return m_internal_data;
}


void Watcher::Clear()
{
	m_internal_data.clear();
}
