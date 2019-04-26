#pragma once
#include "common.h"
#ifdef RESTRICTED_VERSION
#include "cStatusBar.h"

class cRestrictedVersion
{
public:
	cRestrictedVersion();
	~cRestrictedVersion();

	static void init(HWND hwnd);
	static void on_highlight();
	static void what_button_presses_left();

	static int i_button_presses_left;
};
#endif