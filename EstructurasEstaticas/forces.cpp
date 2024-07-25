#include "forces.h"

#define M_PI 3.14159265358979323846


Force::Force(float x, float y, float z)
{
	this->vector[0] = x;
	this->vector[1] = y;
	this->vector[2] = z;
	mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Force::Force(float mag, float angulo)
{
	this->vector[0] = mag * cos(angulo * M_PI / 180);
	this->vector[1] = mag * sin(angulo * M_PI / 180);
	this->vector[2] = 0;
	this->mag = mag;
}


void Force::setDirection(float x, float y, float z)
{
	float magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	direction[0] = x/magnitude;
	direction[1] = y/magnitude;
	direction[2] = z/magnitude;
}