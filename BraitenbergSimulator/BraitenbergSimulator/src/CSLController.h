#pragma once
#include "Controller.h"
class CSLController : public Controller
{
public:
	CSLController();
	CSLController(float gf, float gi);
	float Update(float input) override;
	void UpdateCoefficients(float gf, float gi);
private:
	float coeff_gi;
	float coeff_gf;
	float m_delay; //delay element
	float m_last_input;
	float m_last_output;
	float m_last_delay;
	float Clamp(float input);
};

