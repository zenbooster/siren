#pragma once
#include "remSnd.h"
#include "cSettings.h"

class cShortEnd :
	public remSnd
{
	double dt_i;
	double fmin, fmax;

public:
	cShortEnd(double dt = NAN);
	~cShortEnd();

	bool init();
};

