#include "PeriodicityDetectionProcessor.h"

#include <fftw3.h>

#include <numeric>
#include <algorithm>


PeriodicityDetectionProcessor::PeriodicityDetectionProcessor(std::string name, Watcher * watcherToProcess)
	:Processor(name, watcherToProcess)
{
}

void PeriodicityDetectionProcessor::Update()
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

	std::vector<std::vector<float>::iterator> iterators;

	for (int i = 1; i < dct.size()-4; i++)
	{
		if (open == false)
		{
			//get start point
			if (dct[i] != 0 && dct[i - 1] == 0)
			{
				iterators.push_back(std::next(dct.begin(), i));
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
					iterators.push_back(std::next(dct.begin(), i));
					open = false;
				}
			}
		}
	}
	std::vector<std::vector<float>>ranges;
	if (iterators.size() >= 2)
	{
		for (int i = 0; i < iterators.size(); i++)
		{
			if ((i + 1) % 2 == 0)
			{
				std::vector<float> temp(iterators[i-1], iterators[i]);
				ranges.push_back(temp);
			}
		}
	}
	//pad
	for (int i = 0; i < ranges.size(); i++)
	{
		while (ranges[i].size() < dct.size())
		{
			ranges[i].push_back(0.0f);
			m_internal_data = InverseDiscreteCosine(ranges[i]);
		}

		//inverse discrete cosine
	}
	//m_internal_data = dct;
}

std::vector<float> PeriodicityDetectionProcessor::DiscreteCosine(const boost::circular_buffer<float>& x)
{
	int N = x.size();
	double* in = new double[N];
	double* out = new double[N];

	std::vector<double> x_cent(x.size());
	std::copy(x.begin(), x.end(), x_cent.begin());

	//float mean = std::accumulate(x.begin(), x.end(), 0.0f) / (float)x.size();

	////centre x
	//std::transform(x.begin(), x.end(), x_cent.begin(),
	//	[&](float a) {
	//	return a - mean;
	//}
	//);

	//create plan
	fftw_plan dct_plan;
	dct_plan = fftw_plan_r2r_1d(N, in, out, FFTW_REDFT10, FFTW_MEASURE);

	//initialise in (as fftw plan overwrites it)
	std::copy(x_cent.begin(), x_cent.end(), stdext::checked_array_iterator<double*>(in,N));

	//for (int i = 0; i < N; i++)
	//{
	//	in[i] = x_cent[i];
	//}

	fftw_execute(dct_plan);

	std::vector<float> result;
	result.reserve(N);

	result.assign(out, out + N);

	/*
	for (int i = 0; i < N; i++)
	{

		float temp = (float)out[i];
		result.push_back(temp);
	}*/

	//calculate std deviation

	//normalise#

	/*double max = abs(*std::max_element(result.begin(), result.end()));
	double min = abs(*std::max_element(result.begin(), result.end()));

	std::transform(result.begin(), result.end(), result.begin(),
	[&](float a) {
	return a / fmax(max,min);
	}
	);*/

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

	//float mean = std::accumulate(x.begin(), x.end(), 0.0f) / (float)x.size();

	////centre x
	//std::transform(x.begin(), x.end(), x_cent.begin(),
	//	[&](float a) {
	//	return a - mean;
	//}
	//);

	//create plan
	fftw_plan dct_plan;
	dct_plan = fftw_plan_r2r_1d(N, in, out, FFTW_REDFT01, FFTW_MEASURE);

	//initialise in (as fftw plan overwrites it)
	std::copy(x_cent.begin(), x_cent.end(), stdext::checked_array_iterator<double*>(in, N));
	/*for (int i = 0; i < N; i++)
	{
		in[i] = x_cent[i];
	}*/

	fftw_execute(dct_plan);

	std::vector<float> result;
	result.reserve(N);
	result.assign(out, out + N);

	//calculate std deviation

	//normalise#

	/*double max = abs(*std::max_element(result.begin(), result.end()));
	double min = abs(*std::max_element(result.begin(), result.end()));

	std::transform(result.begin(), result.end(), result.begin(),
	[&](float a) {
	return a / fmax(max,min);
	}
	);*/

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
