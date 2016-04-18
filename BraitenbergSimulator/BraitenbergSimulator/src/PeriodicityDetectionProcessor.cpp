#include "PeriodicityDetectionProcessor.h"

#include <fftw3.h>

#include <numeric>
#include <algorithm>


PeriodicityDetectionProcessor::PeriodicityDetectionProcessor(std::string name, Watcher * watcherToProcess)
	:Processor(name, watcherToProcess)
{
}

PeriodicityDetectionProcessor::PeriodicityDetectionProcessor(std::string name, Watcher * watcherToProcess, OscDetectedCallback callback)
	: Processor(name, watcherToProcess), odc(callback)
{
}

void PeriodicityDetectionProcessor::Update()
{
	std::vector<float> data(m_watcher->GetData().begin(), m_watcher->GetData().end());

	float mean = 0.0f;
	for (auto& val : data)
	{
		mean += val;
	}
	mean = mean / data.size();

	//LOCK PHASE
	if((prevData0 > mean && data[0] < mean) || (prevData0 == data[0]))
	{
		std::vector<float> dct = DiscreteCosine(m_watcher->GetData());

		//remove points below std.deviation if enabled
		if (m_noiseReductionEnabled)
		{
			double std_dev = StandardDeviation(dct);

			std::transform(dct.begin(), dct.end(), dct.begin(),
				[&](double a) {
				if (fabs(a) >= 0.5 * std_dev)
				{
					return a;
				}
				else
				{
					return 0.0;
				}
			}
			);
		}

		//get frequency components
		bool open = false;

		std::vector<std::pair<int, int>> indexPairs;
		int startIndex = 0;
		for (int i = 1; i < dct.size() - 4; i++)
		{
			if (open == false)
			{
				//get start point
				if (dct[i] != 0 && dct[i - 1] == 0)
				{
					startIndex = i;
					open = true;
				}
			}
			else
			{
				//get end point
				if (dct[i] != 0)
				{
					bool ok = true;
					for (int k = 1; k <= 4; k++)
					{
						if (dct[i + k] != 0)
						{
							ok = false;
						}
					}
					if (ok)
					{
						indexPairs.push_back(std::pair<int, int>(startIndex, i));
						open = false;
					}
				}
			}
		}
		//pad
		std::vector<std::vector<float>> components;

		for (int i = 0; i < indexPairs.size(); i++)
		{
			std::vector<float> temp(dct);
			if (i < indexPairs[i].first || i > indexPairs[i].second)
			{
				temp[i] = 0;
			}
			temp = InverseDiscreteCosine(temp);
			components.push_back(temp);
		}

		if (components.size() > 0)
			m_internal_data = components[0];

		std::vector<int> zcIndices;

		//find zero crossings
		for (int i = 1; i < m_internal_data.size(); i++)
		{
			float prev = m_internal_data[i - 1];
			float curr = m_internal_data[i];

			if (prev > 0 && curr < 0 || prev < 0 && curr > 0)
			{
				//zero crossing
				zcIndices.push_back(i - 1);
			}
		}
		//printf("zero crossings: %i\n", zcIndices.size());
		m_oscStatus = false;

		if (!zcIndices.empty())
		{
			std::vector<int> increments;

			for (int i = 1; i < zcIndices.size(); i++)
			{
				int prevIndex = zcIndices[i - 1];
				int index = zcIndices[i];
				increments.push_back(2 * (index - prevIndex));
			}

			float mean = 0.0f;
			for (int i = 0; i < increments.size(); i++)
			{
				mean += increments[i];
			}

			mean = mean / (zcIndices.size() - 1);

			float standardDeviation = 0.0f;
			for (int i = 0; i < increments.size(); i++)
			{
				//printf("increment: %d\n", increments[i]);
				standardDeviation += (increments[i] - mean) * (increments[i] - mean);
			}

			standardDeviation = sqrt(standardDeviation / (zcIndices.size() - 2));

			//printf("mean: %f\nstandard deviation: %f\n", mean, standardDeviation);

			if ((standardDeviation / mean) < 0.333f)
			{
				//printf("oscillation detected!\nperiod: %f s\n", mean/(60/m_sampleRate));
				m_count++;
				if (m_count >= 6)
				{
					m_oscStatus = true;
					m_oscPeriod = mean;
					if (odc)
					{
						odc(mean);

					}
					m_count = 0;
				}
			}
			else
			{
				m_count = 0;
			}
		}

	}
	prevData0 = data[0];
}

bool PeriodicityDetectionProcessor::GetOscillationStatus()
{
	return m_oscStatus;
}

float PeriodicityDetectionProcessor::GetOscillationPeriod()
{
	if (m_oscStatus)
		return m_oscPeriod;
	else
		return -1.0f;
}

std::vector<float> PeriodicityDetectionProcessor::DiscreteCosine(const boost::circular_buffer<float>& x)
{
	int N = x.size();
	double* in = new double[N];
	double* out = new double[N];

	std::vector<double> x_cent(x.size());
	std::copy(x.begin(), x.end(), x_cent.begin());

	//create plan
	fftw_plan dct_plan;
	dct_plan = fftw_plan_r2r_1d(N, in, out, FFTW_REDFT10, FFTW_MEASURE);

	//initialise in (as fftw plan overwrites it)
	std::copy(x_cent.begin(), x_cent.end(), stdext::checked_array_iterator<double*>(in,N));

	fftw_execute(dct_plan);

	std::vector<float> result;
	result.reserve(N);

	result.assign(out, out + N);

	fftw_destroy_plan(dct_plan);

	delete[] in;
	delete[] out;

	return result;
}

std::vector<float> PeriodicityDetectionProcessor::InverseDiscreteCosine(const std::vector<float>& x)
{
	int N = x.size();
	double* in = new double[N];
	double* out = new double[N];

	std::vector<double> x_cent(x.size());
	std::copy(x.begin(), x.end(), x_cent.begin());

	//create plan
	fftw_plan dct_plan;
	dct_plan = fftw_plan_r2r_1d(N, in, out, FFTW_REDFT01, FFTW_MEASURE);

	//initialise in (as fftw plan overwrites it)
	std::copy(x_cent.begin(), x_cent.end(), stdext::checked_array_iterator<double*>(in, N));

	fftw_execute(dct_plan);

	std::vector<float> result;
	result.reserve(N);
	result.assign(out, out + N);

	fftw_destroy_plan(dct_plan);

	delete[] in;
	delete[] out;

	return result;
}

float PeriodicityDetectionProcessor::StandardDeviation(const std::vector<float> x)
{
	std::vector<float> x2(x.size());

	float mean = std::accumulate(x.begin(), x.end(), 0.0) / (float)x.size();
	std::transform(x.begin(), x.end(), x2.begin(),
		[&](float a) {
		return (a - mean)*(a - mean);
	}
	);
	float variance = std::accumulate(x2.begin(), x2.end(), 0.0) / ((float)x.size() - 1);

	return sqrtf(variance);
}
