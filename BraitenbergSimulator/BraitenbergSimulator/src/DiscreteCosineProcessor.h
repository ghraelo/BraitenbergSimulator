#pragma once

#include "Processor.h"

class DiscreteCosineProcessor : public Processor
{
public:
	DiscreteCosineProcessor(std::string name, Watcher* watcherToProcess);
	void Update() override;
private:
	std::vector<float> DiscreteCosine(const boost::circular_buffer<float>& x);
	double StandardDeviation(const std::vector<double> x);
};