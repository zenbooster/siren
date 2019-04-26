#pragma once
#include "remSnd.h"
#include "cMeander.h"

class cWail :
	public remSnd
{
	double dt_i;
	double fmin;
	double fmax;

	char* fname;

public:
	cWail(double dt = NAN, double fmin = NAN, double fmax = NAN, char* fname = NULL);
	virtual ~cWail(void);

	//bool init(double dt = NAN, double fmin = NAN, double fmax = NAN, char* fname = NULL);
	bool init(double dt, double fmin, double fmax, char* fname);

	bool init(void);
};

