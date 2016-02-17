#pragma once

struct CSLInternalState
{
	float m_input;
	float m_delay;
	float m_output;
};

class CSL
{
public:
	CSL();
	CSL(float gi, float gf);
	float Update(float input);
	void UpdateCoefficients(float gi, float gf);
	CSLInternalState GetInternalState();
private:
	float coeff_gi;
	float coeff_gf;
	float m_delay; //delay element
	float m_last_input;
	float m_last_output;
	float m_last_delay;
	float Clamp(float input);
};