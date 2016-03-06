#include "AutocorrelationProcessor.h"
#include <fftw3.h>
#include <algorithm>
#include <numeric>
#include <boost\circular_buffer.hpp>

AutocorrelationProcessor::AutocorrelationProcessor(std::string name, Watcher * watcherToProcess)
	:Processor(name,watcherToProcess)
{
}

void AutocorrelationProcessor::Update()
{
	std::vector<float> acor = FFTAutocorrelation(m_watcher->GetData());

	//remove padding and shift so zero is center frequency
	m_internal_data.clear();
	m_internal_data.resize(acor.size() / 2);
	std::copy(acor.begin() + acor.size() / 2, acor.end(), m_internal_data.begin());
	std::rotate(m_internal_data.begin(), m_internal_data.begin() + m_internal_data.size() / 2, m_internal_data.end());
}

std::vector<float> AutocorrelationProcessor::FFTAutocorrelation(const boost::circular_buffer<float>& x)
{
	std::vector<double> x_cent;
	x_cent.resize(x.size());

	float mean = std::accumulate(x.begin(), x.end(), 0) / (float)x.size();
	//centre x
	std::transform(x.begin(), x.end(), x_cent.begin(),
		[&](float a) {
		return a - mean;
	}
	);

	int N = x.size();
	double* in = new double[N];

	for (int i = 0; i < N; i++)
	{
		in[i] = x_cent[i];
	}

	fftw_complex *out;
	fftw_plan p, p2;
	int x2 = floor(N / 2 + 1);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * floor(N / 2 + 1));

	p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
	p2 = fftw_plan_dft_c2r_1d(N, out, in, FFTW_ESTIMATE);

	//std::copy(x_cent.begin(), x_cent.end(), in);
	fftw_execute(p); /* repeat as needed */

						//replace cplx nos with norms
	for (int i = 0; i < floor(N / 2 + 1); i++)
	{
		out[i][0] = out[i][0] * out[i][0] + out[i][1] * out[i][1];
		out[i][1] = 0;
	}

	fftw_execute(p2);

	fftw_destroy_plan(p);
	fftw_destroy_plan(p2);
	std::vector<float> acorr(N);


	for (int i = 0; i < N; i++)
	{
		acorr.push_back(in[i] / in[0]);
	}

	delete[] in;
	fftw_free(out);

	return acorr;
}
