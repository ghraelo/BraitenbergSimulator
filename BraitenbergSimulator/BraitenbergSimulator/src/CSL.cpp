#include "CSL.h"


CSL::CSL()
	:coeff_gf(0), coeff_gi(0)
{
}

CSL::CSL(float gf, float gi)
	:coeff_gf(gf), coeff_gi(gi)
{
}

float CSL::Update(float input)
{
	float output = input* -coeff_gi + m_delay;
	m_last_delay = m_delay;
	m_delay = Clamp(output * coeff_gf);
	m_last_input = input;
	m_last_output = output;

	return Clamp(output);
}

void CSL::SetCoefficients(float gf, float gi)
{
	coeff_gf = gf;
	coeff_gi = gi;
}

CSLInternalData CSL::GetInternalData()
{
	CSLInternalData cid;
	cid.m_delay = m_last_delay;
	cid.m_input = m_last_input;
	cid.m_output = m_last_output;

	return cid;
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