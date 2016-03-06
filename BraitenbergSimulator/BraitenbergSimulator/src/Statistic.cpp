#include "Statistic.h"

Statistic::Statistic()
{
}

Statistic::Statistic(std::string name, int sampleRate)
	:m_name(name), m_sampleRate(sampleRate)
{
}

int Statistic::GetSampleRate()
{
	return m_sampleRate;
}

std::string Statistic::GetName()
{
	return m_name;
}
