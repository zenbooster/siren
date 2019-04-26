#include "stdafx.h"
#include "cMeander.h"
#include "common.h"

void cMeander::_set_value(byte v)
{
	value = v;
}

cMeander::cMeander(char *p): p(p), value(0)
{}

void cMeander::reset(char *p)
{
	this->p = p;
}

void cMeander::delay(int us)
{
	int i_cnt = US2SIZE(us);
	for (int i = 0; i < i_cnt; i++)
		*p++ = value;
}