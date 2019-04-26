#pragma once
#include "AL/al.h"
#include "cManualBufferCommon.h"

class cManualAscBuffer
{
	double dt_i;
	static ALsizei size;

public:
	cManualAscBuffer(double dt = MANUAL_ASC_SPEED);
	~cManualAscBuffer();

	bool init();
	ALsizei get_size() const;
};

