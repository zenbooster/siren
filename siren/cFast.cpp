#include "stdafx.h"
#include "cFast.h"


cFast::cFast()
{
	double fmin = (float)(cSettings::cfg.getRoot()["fx"]["fast"]["fmin"]);
	double fmax = (float)(cSettings::cfg.getRoot()["fx"]["fast"]["fmax"]);
	init(
		SEC2DELTA((float)(cSettings::cfg.getRoot()["fx"]["fast"]["fsec"]), fmin, fmax),
		fmin,
		fmax,
		NULL);
}


cFast::~cFast()
{
}
