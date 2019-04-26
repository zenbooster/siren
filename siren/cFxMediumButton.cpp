#include "stdafx.h"
#include "cFxMediumButton.h"
#include "tstring.h"
#include "cFxButtonsContainer.h"

int cFxMediumButton::i_refs_cnt = 0;

cFxMediumButton::cFxMediumButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer) :
	cFxButton(h_dlg, i_control_id),
	pfxButtonsContainer(pfxButtonsContainer)
{
	if (!i_refs_cnt++)
	{
		if (!fx.init())
		{
			std::tstring s_err = _T("cFxMediumButton: fx");
			s_err += _T(".init() failed!");
			ERRMSG(s_err.c_str());
		}
	}
}


cFxMediumButton::~cFxMediumButton()
{
	if (!--i_refs_cnt)
	{
	}
}

void cFxMediumButton::play()
{
	pfxButtonsContainer->stop();
	fx.Play();
}

void cFxMediumButton::on_highlight()
{
}

void cFxMediumButton::on_unhighlight()
{
	if (fx.IsPlaying())
		fx.Stop();
	else
	{
		pfxButtonsContainer->stop();
		fx.Play();
	}
}

void cFxMediumButton::stop()
{
	fx.Stop();
}

bool cFxMediumButton::is_playing()
{
	bool res = false;

	res = fx.IsPlaying();

	return res;
}

void cFxMediumButton::mute(bool m)
{
	fx.mute(m);
}