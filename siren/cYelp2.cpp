#include "stdafx.h"
#include "cYelp2.h"

cYelp2::cYelp2()
{
	double fmin = (float)(cSettings::cfg.getRoot()["fx"]["yelp2"]["fmin"]);
	double fmax = (float)(cSettings::cfg.getRoot()["fx"]["yelp2"]["fmax"]);
	init(
		SEC2DELTA((float)(cSettings::cfg.getRoot()["fx"]["yelp2"]["fsec"]), fmin, fmax),
		fmin,
		fmax,
		NULL);
}


cYelp2::~cYelp2()
{
}
