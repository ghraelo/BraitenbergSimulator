// Wrapper.cpp : Defines the exported functions for the DLL application.
//

#pragma once

#include "stdafx.h"

__declspec(dllexport)
void DiscreteCosine(double* input, double* output, int size, bool forward);
