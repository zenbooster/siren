#include "stdafx.h"
#include "cSine.h"
#include "common.h"
#include <math.h>

//#define PI 3.141592654

cSine::cSine() :
	phase(0.0)
{
}

cSine::~cSine()
{
}

double cSine::next(double freq, double pm)
{
	double res = sin(phase + pm);
	phase = fmod((phase + 2.0 * PI * freq / (double)FREQUENCY), (2 * PI));
	return res;
}

void cSine::reset()
{
	phase = 0.0;
}

double cSine::get_min_phase(double freq) const
{
	return 2.0 * PI * freq / (double)FREQUENCY;
}

double cSine::get_phase() const
{
	return phase;
}