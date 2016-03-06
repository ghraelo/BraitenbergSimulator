#pragma once

#include "Processor.h"

class AutocorrelationProcessor : public Processor
{
public:
	AutocorrelationProcessor(std::string name, Watcher* watcherToProcess);
	void Update() override;
private:
	std::vector<float> FFTAutocorrelation(const boost::circular_buffer<float>& x);
};