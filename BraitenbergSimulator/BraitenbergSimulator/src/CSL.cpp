#include "CSL.h"

CSL::CSL()
	:coeff_gf(1.0f),coeff_gi(1.0f), m_delay(0.0f)
{
}

CSL::CSL(float gi, float gf)
	: coeff_gf(gf), coeff_gi(gi), m_delay(0.0f)
{

}

float CSL::Update(float input)
{
	float output = input* -coeff_gi + m_delay;
	m_last_delay = m_delay;
	m_delay = Clamp(output * coeff_gf);
	m_last_input = input;
	m_last_output = output;

	return output;
}

void CSL::UpdateCoefficients(float gi, float gf)
{
	coeff_gf = gf;
	coeff_gi = gi;
}

float CSL::Clamp(float input)
{
	if (input < -1.0f)
		return -1.0f;
	else if (input > 1.0f)
		return 1.0f;
	else
		return input;
}

CSLInternalState CSL::GetInternalState()
{
	CSLInternalState cis;
	cis.m_delay = m_last_delay;
	cis.m_input = m_last_input;
	cis.m_output = m_last_output;

	return cis;
}