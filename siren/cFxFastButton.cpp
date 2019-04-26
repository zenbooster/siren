#include "stdafx.h"
#include "cFxFastButton.h"
#include "tstring.h"
#include "cFxButtonsContainer.h"
#include "cShortEnd.h"
#include "cSettings.h"

int cFxFastButton::i_refs_cnt = 0;

cFxFastButton::cFxFastButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer):
	cFxButton(h_dlg, i_control_id),
	pfxButtonsContainer(pfxButtonsContainer)
{
	if (!i_refs_cnt++)
	{
		if (!fx.init())
		{
			std::tstring s_err = _T("cFxFastButton: fx");
			s_err += _T(".init() failed!");
			ERRMSG(s_err.c_str());
		}
	}
}


cFxFastButton::~cFxFastButton()
{
	if (!--i_refs_cnt)
	{
	}
}

void cFxFastButton::play()
{
	pfxButtonsContainer->stop();
	fx.Play();
}

void cFxFastButton::on_highlight()
{
}

void cFxFastButton::on_unhighlight()
{
	if (fx.IsPlaying())
		fx.Stop();
	else
	{
		pfxButtonsContainer->stop();
		fx.Play();
	}
}

void cFxFastButton::stop()
{
	fx.Stop();
}

bool cFxFastButton::is_playing()
{
	bool res = false;

	res = fx.IsPlaying();

	return res;
}

void cFxFastButton::mute(bool m)
{
	fx.mute(m);
}