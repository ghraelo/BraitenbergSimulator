#include "Processor.h"

Processor::Processor(std::string name, Watcher * watcherToProcess)
	:m_watcher(watcherToProcess), Statistic(name, watcherToProcess->GetSampleRate()), m_internal_data(watcherToProcess->GetData().capacity())
{
}

const std::vector<float>& Processor::GetData()
{
	return m_internal_data;
}

void Processor::Clear()
{
	m_internal_data.clear();
}
