#pragma once
#include "AL/al.h"
#include "cManualBufferCommon.h"

class cManualDescBuffer
{
	double dt_i;
	ALsizei size;

public:
	cManualDescBuffer(double dt = MANUAL_DESC_SPEED);
	~cManualDescBuffer();

	bool init();
	ALsizei get_size() const;
};