// DCTWrapper.h

#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

#include "Unmanaged.h"

namespace DCTWrapper {

	public ref class DiscreteCosineTransform
	{
		// TODO: Add your methods for this class here.
	public:
		static List<double>^ Forward(List<double>^ input)
		{
			int count = input->Count;
			IntPtr q = Marshal::AllocHGlobal(count*sizeof(double));

			double* inArr = (double*)(q.ToPointer());

			for (int i = 0; i < count; i++)
			{
				inArr[i] = input[i];
			}

			double* output = new double[count];

			DiscreteCosine(inArr, output, count, true);

			//free memory
			Marshal::FreeHGlobal(q);

			List<double>^ ls;

			for (int i = 0; i < count; i++)
			{
				ls->Add(output[i]);
			}

			delete[] output;
			return ls;
		};

		static List<double>^ Reverse(List<double>^ input)
		{
			int count = input->Count;
			IntPtr q = Marshal::AllocHGlobal(count*sizeof(double));

			double* inArr = (double*)(q.ToPointer());

			for (int i = 0; i < count; i++)
			{
				inArr[i] = input[i];
			}

			double* output = new double[count];

			DiscreteCosine(inArr, output, count, false);

			//free memory
			Marshal::FreeHGlobal(q);

			List<double>^ ls;

			for (int i = 0; i < count; i++)
			{
				ls->Add(output[i]);
			}

			delete[] output;
			return ls;
		};
	};
}
