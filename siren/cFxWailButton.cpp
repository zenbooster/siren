#include "stdafx.h"
#include "cFxWailButton.h"
#include "cFxButtonsContainer.h"

cFxWailButton::cFxWailButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer):
	cFxButton(h_dlg, i_control_id),
	pfxButtonsContainer(pfxButtonsContainer)
{
	if (!wail.init())
		ERRMSG(_T("wail.init() failed!"));
}

cFxWailButton::~cFxWailButton()
{
}

void cFxWailButton::on_highlight()
{
}

void cFxWailButton::on_unhighlight()
{
	if (wail.IsPlaying())
		wail.Stop();
	else
	{
		pfxButtonsContainer->stop();
		wail.Play();
	}
}

void cFxWailButton::stop()
{
	wail.Stop();
}

bool cFxWailButton::is_playing()
{
	return wail.IsPlaying();
}

void cFxWailButton::mute(bool m)
{
	wail.mute(m);
}