#pragma once
#include "common.h"
#define LIBCONFIGXX_STATIC
#include <libconfig.h++>

using namespace libconfig;

class cSettings
{
public:
	cSettings();
	~cSettings();

	static Config cfg;
};

