#pragma once

#include "Processor.h"

class PeriodicityDetectionProcessor : public Processor
{
public:
	PeriodicityDetectionProcessor(std::string name, Watcher* watcherToProcess);
	void Update() override;
private:
	std::vector<float> DiscreteCosine(const boost::circular_buffer<float>& x);
	std::vector<float> InverseDiscreteCosine(const std::vector<float>& x);
	float StandardDeviation(const std::vector<float> x);
	bool m_noiseReductionEnabled = true;
};