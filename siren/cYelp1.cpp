#include "stdafx.h"
#include "cYelp1.h"

cYelp1::cYelp1()
{
	double fmin = (float)(cSettings::cfg.getRoot()["fx"]["yelp1"]["fmin"]);
	double fmax = (float)(cSettings::cfg.getRoot()["fx"]["yelp1"]["fmax"]);
	init(
		SEC2DELTA((float)(cSettings::cfg.getRoot()["fx"]["yelp1"]["fsec"]), fmin, fmax),
		fmin,
		fmax,
		NULL);
}


cYelp1::~cYelp1()
{
}
