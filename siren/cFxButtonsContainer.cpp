#include "stdafx.h"
#include "cFxButtonsContainer.h"
#include "resource.h"


cFxButtonsContainer::cFxButtonsContainer(HWND h_dlg):
	fxbHorn(h_dlg, IDC_BUTTON_HORN, this),
	fxbStart(h_dlg, IDC_BUTTON_START, this),
	fxbWail(h_dlg, IDC_BUTTON_WAIL, this),
	fxbHiLo(h_dlg, IDC_BUTTON_HILO, this),
	fxbManual(h_dlg, IDC_BUTTON_MANUAL, this),
	fxbFast(h_dlg, IDC_BUTTON_FAST, this),
	fxbMedium(h_dlg, IDC_BUTTON_MEDIUM, this)
{
}

cFxButtonsContainer::~cFxButtonsContainer(void)
{
}

void cFxButtonsContainer::stop()
{
	cFxButton *pFxButtonPlayed;
	if (fxbStart.is_playing())
		pFxButtonPlayed = &fxbStart;
	else
		if (fxbWail.is_playing())
			pFxButtonPlayed = &fxbWail;
		else
			if (fxbFast.is_playing())
				pFxButtonPlayed = &fxbFast;
			else
				if (fxbMedium.is_playing())
					pFxButtonPlayed = &fxbMedium;
				else
					if (fxbHiLo.is_playing())
						pFxButtonPlayed = &fxbHiLo;
					else
						if (fxbManual.is_playing())
							pFxButtonPlayed = &fxbManual;
						else
							pFxButtonPlayed = NULL;

	if (pFxButtonPlayed)
		pFxButtonPlayed->stop();
}