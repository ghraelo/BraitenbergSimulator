#include "DiscreteCosineProcessor.h"

#include <fftw3.h>

#include <numeric>
#include <algorithm>

DiscreteCosineProcessor::DiscreteCosineProcessor(std::string name, Watcher * watcherToProcess)
	:Processor(name,watcherToProcess)
{
}

void DiscreteCosineProcessor::Update()
{
	m_internal_data = DiscreteCosine(m_watcher->GetData());
}

std::vector<float> DiscreteCosineProcessor::DiscreteCosine(const boost::circular_buffer<float>& x)
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

	double std_dev = StandardDeviation(x_cent);

	/*printf("3&std_dev:  %0.2f\n", 3*std_dev);
	std::transform(x_cent.begin(), x_cent.end(), x_cent.begin(),
		[&](double a) {
		if (fabs(a) >= 3 * std_dev)
		{
			return a;
		}
		else
		{
			return 0.0;
		}
	}
	);*/

	//create plan
	fftw_plan dct_plan;
	dct_plan = fftw_plan_r2r_1d(N, in, out, FFTW_REDFT10, FFTW_MEASURE);

	//initialise in (as fftw plan overwrites it)
	for (int i = 0; i < N; i++)
	{
		in[i] = x_cent[i];
	}

	fftw_execute(dct_plan);

	std::vector<float> result;
	result.reserve(N);

	for (int i = 0; i < N; i++)
	{
		
		float temp = (float)out[i];
		result.push_back(temp);
	}

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

double DiscreteCosineProcessor::StandardDeviation(const std::vector<double> x)
{
	std::vector<double> x2(x.size());

	double mean = std::accumulate(x.begin(), x.end(), 0.0) / (double)x.size();
	std::transform(x.begin(),x.end(),x2.begin(),
		[&](double a) {
		return (a-mean)*(a-mean);
	}
	);
	double variance = std::accumulate(x2.begin(), x2.end(), 0.0)/((double)x.size()-1);

	return sqrtf(variance);
}
