#include "stdafx.h"
#include "cFxManualButton.h"
#include "cFxButtonsContainer.h"
#include "common.h"

cFxManualButton::cFxManualButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer):
	cFxButton(h_dlg, i_control_id),
	pfxButtonsContainer(pfxButtonsContainer)
{
	if (!manual.init())
		ERRMSG(_T("manual.init() failed!"));
}

cFxManualButton::~cFxManualButton()
{
}

void cFxManualButton::on_highlight()
{
	pfxButtonsContainer->fxbHiLo.stop();
	pfxButtonsContainer->fxbStart.stop();
	pfxButtonsContainer->fxbWail.stop();
	manual.on_highlight();
}

void cFxManualButton::on_unhighlight()
{
	manual.on_unhighlight();
}

void cFxManualButton::stop()
{
	manual.Stop();
}

bool cFxManualButton::is_playing()
{
	return manual.IsPlaying();
}

void cFxManualButton::mute(bool m)
{
	manual.mute(m);
}