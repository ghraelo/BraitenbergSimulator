#pragma once

#include <algorithm>

#include <fftw3.h>

void DiscreteCosine(double* input, double* output, int size, bool forward)
{
	double* in = new double[size];

	//create plan
	fftw_plan dct_plan;
	dct_plan = fftw_plan_r2r_1d(size, in, output, forward ? FFTW_REDFT10 : FFTW_REDFT01, FFTW_MEASURE);

	//initialise in (as fftw plan overwrites it)
	memcpy(in, input, size*sizeof(double));

	fftw_execute(dct_plan);

	fftw_destroy_plan(dct_plan);

	delete[] in;
}