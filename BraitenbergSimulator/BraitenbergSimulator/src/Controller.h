#pragma once

class Controller
{
public:
	Controller();
	virtual float Update(float input) = 0;
};