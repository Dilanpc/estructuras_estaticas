#pragma once

#include "linearAlgebra.h"

class Force
{
public:
	float vector[3]{};
	float direction[3]{};
	float mag = 0;


public:
	Force() {}
	Force(float x, float y, float z);
	Force(float mag, float angle);

	void setDirection(float x, float y, float z = 0.f);


};