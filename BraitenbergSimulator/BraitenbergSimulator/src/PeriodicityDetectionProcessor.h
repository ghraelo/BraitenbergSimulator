#pragma once

#include "Processor.h"

#include <functional>

typedef std::function<void(float)> OscDetectedCallback;

class PeriodicityDetectionProcessor : public Processor
{
public:
	PeriodicityDetectionProcessor(std::string name, Watcher* watcherToProcess);
	PeriodicityDetectionProcessor(std::string name, Watcher* watcherToProcess, OscDetectedCallback callback);
	void Update() override;
	bool GetOscillationStatus();
	float GetOscillationPeriod();
private:
	std::vector<float> DiscreteCosine(const boost::circular_buffer<float>& x);
	std::vector<float> InverseDiscreteCosine(const std::vector<float>& x);
	float StandardDeviation(const std::vector<float> x);
	bool m_noiseReductionEnabled = true;
	float prevData0 = 0.0f;
	bool m_oscStatus = false;
	float m_oscPeriod = 0.0f;
	int m_count = 0;
	OscDetectedCallback odc;
};