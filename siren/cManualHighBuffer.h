#pragma once
#include "AL/al.h"

class cManualHighBuffer
{
	static ALsizei size;
public:
	cManualHighBuffer();
	~cManualHighBuffer();

	bool init();
	ALsizei get_size() const;
};

