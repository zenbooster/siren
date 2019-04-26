#pragma once
#include "common.h"

typedef unsigned char byte;

class cMeander
{
private:
	char *p;
	byte value;

	void _set_value(byte v);

public:
	cMeander(char *p = NULL);

	void reset(char *p);

	void high()
	{_set_value(MAX_SAMPLE_VAL);}
	void low()
	{_set_value(0);}

	void delay(int us);
};