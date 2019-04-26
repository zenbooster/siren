#include "stdafx.h"
#include "cFxHornButton.h"
#include "cFxButtonsContainer.h"
#include "common.h"


cFxHornButton::cFxHornButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer):
	cFxButton(h_dlg, i_control_id),
	pfxButtonsContainer(pfxButtonsContainer),
	pFxButtonPlayed(NULL)
{
	if (!horn.init())
		ERRMSG(_T("horn.init() failed!"));
}


cFxHornButton::~cFxHornButton()
{
}

void cFxHornButton::on_highlight()
{
	if (pfxButtonsContainer->fxbStart.is_playing())
		pFxButtonPlayed = &pfxButtonsContainer->fxbStart;
	else
		if (pfxButtonsContainer->fxbManual.is_playing())
			pFxButtonPlayed = &pfxButtonsContainer->fxbManual;
		else
			if (pfxButtonsContainer->fxbHiLo.is_playing())
				pFxButtonPlayed = &pfxButtonsContainer->fxbHiLo;
			else
				pFxButtonPlayed = NULL;

	if (pFxButtonPlayed)
	{
		pFxButtonPlayed->mute(true);
	}
	horn.Play();
}

void cFxHornButton::on_unhighlight()
{
	horn.Stop();
	if (pFxButtonPlayed)
		pFxButtonPlayed->mute(false);
}
