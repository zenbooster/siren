#pragma once
#include "remSnd.h"

class cHorn :
	public remSnd
{
private:
	static int refs;
	static short *p_horn;
	static int i_horn_len;

public:
	cHorn(void);
	~cHorn(void);

	bool init();
};

