#pragma once
#include "cFxHornButton.h"
#include "cFxStartButton.h"
#include "cFxWailButton.h"
#include "cFxHiLoButton.h"
#include "cFxManualButton.h"
#include "cFxFastButton.h"
#include "cFxMediumButton.h"

class cFxButtonsContainer
{
public:
	cFxHornButton fxbHorn;
	cFxStartButton fxbStart;
	cFxWailButton fxbWail;
	cFxHiLoButton fxbHiLo;
	cFxManualButton fxbManual;
	cFxFastButton fxbFast;
	cFxMediumButton fxbMedium;
public:
	cFxButtonsContainer(HWND h_dlg);
	~cFxButtonsContainer(void);

	void stop();
};

