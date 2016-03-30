#pragma once

struct CSLInternalData
{
	float m_input;
	float m_output;
	float m_delay;
};

class CSL
{
public:
	CSL();
	CSL(float gf, float gi);
	float Update(float input);
	void SetCoefficients(float gf, float gi);
	CSLInternalData GetInternalData();
private:
	float coeff_gi;
	float coeff_gf;
	float m_delay = 0; //delay element
	float m_last_input = 0;
	float m_last_output = 0;
	float m_last_delay = 0;
	float Clamp(float input);
};

