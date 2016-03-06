#include "CSL2.h"


CSL2::CSL2()
	:coeff_gf(0), coeff_gi(0)
{
}

CSL2::CSL2(float gf, float gi)
	: coeff_gf(gf), coeff_gi(gi)
{
}

float CSL2::Update(float input)
{
	float output = input * -coeff_gi + m_delay;
	m_last_delay = m_delay;
	m_delay = Clamp(output * coeff_gf + input * coeff_gi);
	m_last_input = input;
	m_last_output = output;

	return Clamp(output);
}

void CSL2::SetCoefficients(float gf, float gi)
{
	coeff_gf = gf;
	coeff_gi = gi;
}

float CSL2::Clamp(float input)
{
	if (input < -1.0f)
		return -1.0f;
	else if (input > 1.0f)
		return 1.0f;
	else
		return input;
}