#include "stdafx.h"
#include "cFxHiLoButton.h"
#include "cFxButtonsContainer.h"

cFxHiLoButton::cFxHiLoButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer):
	cFxButton(h_dlg, i_control_id),
	pfxButtonsContainer(pfxButtonsContainer)
{
	if (!hilo.init())
		ERRMSG(_T("hilo.init() failed!"));
}


cFxHiLoButton::~cFxHiLoButton()
{
}

void cFxHiLoButton::on_highlight()
{
}

void cFxHiLoButton::on_unhighlight()
{
	cFxButton *pFxButtonPlayed = NULL;

	if (hilo.IsPlaying())
		hilo.Stop();
	else
	{
		pfxButtonsContainer->stop();
		hilo.Play();
	}
}

void cFxHiLoButton::stop()
{
	hilo.Stop();
}

bool cFxHiLoButton::is_playing()
{
	return hilo.IsPlaying();
}

void cFxHiLoButton::mute(bool m)
{
	hilo.mute(m);
}