#include "stdafx.h"
#include "cFxStartButton.h"
#include "tstring.h"
#include "cFxButtonsContainer.h"
#include "cYelp1.h"
#include "cYelp2.h"
#include "cShortEnd.h"
#include "cHiLo.h"

int cFxStartButton::i_refs_cnt = 0;
remSnd *cFxStartButton::fx[4];

cFxStartButton::cFxStartButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer):
	cFxButton(h_dlg, i_control_id),
	i_mode(0),
	pfxButtonsContainer(pfxButtonsContainer)
{
	if (!i_refs_cnt++)
	{
		fx[0] = new cYelp1();
		fx[1] = fx[3] = new cShortEnd();
		fx[2] = new cYelp2();

		for (int i = 0; i < fx_size; i++)
			if ((i != 3 ) && !fx[i]->init())
			{
				std::tstring s_err = _T("cFxStartButton: fx[");
				s_err += std::to_tstring(i);
				s_err += _T("]->init() failed!");
				ERRMSG(s_err.c_str());
			}
	}

	SetWindowText(h_wnd, _T("start 1"));
}

cFxStartButton::~cFxStartButton()
{
	if (!--i_refs_cnt)
	{
		for (int i = 0; i < fx_size - 1; i++) // последний элемент не трогаем т.к. fx[3] == fx[1]
		{
			if (fx[i])
			{
				delete fx[i];
				fx[i] = NULL;
			}
		}
	}
}

int cFxStartButton::_prev_mode()
{
	return i_prev_mode;// (!i_mode ? fx_size : i_mode) - 1;
}

void cFxStartButton::_next_mode()
{
	i_prev_mode = i_mode;
	i_mode = (i_mode + 1) % fx_size;
}

void cFxStartButton::play()
{
	pfxButtonsContainer->stop();
	fx[_prev_mode()]->Stop();
	fx[i_mode]->Play();
}

VOID CALLBACK cFxStartButton::MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	cFxStartButton *p_btn = (cFxStartButton*)buttons[hwnd];
	int i_prev_mode = p_btn->i_prev_mode;
	p_btn->_next_mode();
	p_btn->_next_mode();
	p_btn->i_prev_mode = i_prev_mode;

	int i_len = GetWindowTextLength(hwnd) - 2;
	std::tstringstream ss;
	std::tstring s_text;
	s_text.resize(i_len);
	int i = GetWindowText(hwnd, &(s_text[0]), i_len + 1);
	ss << s_text << _T(" ") << (p_btn->i_mode >> 1) + 1;
	s_text = ss.str();
	SetWindowText(hwnd, s_text.c_str());

	KillTimer(hwnd, 1);
}

void cFxStartButton::on_highlight()
{
	if (!(i_mode & 1))
		SetTimer(h_wnd, 1, 1000, (TIMERPROC)MyTimerProc);
}

void cFxStartButton::on_unhighlight()
{
	KillTimer(h_wnd, 1);

	play();
	_next_mode();

	int i_len = GetWindowTextLength(h_wnd) - 2;
	std::tstringstream ss;
	std::tstring s_text;
	s_text.resize(i_len);
	int i = GetWindowText(h_wnd, &(s_text[0]), i_len + 1);
	ss << s_text << _T(" ") << (i_mode >> 1) + 1;
	s_text = ss.str();
	SetWindowText(h_wnd, s_text.c_str());
}

void cFxStartButton::reset()
{
	i_mode = 0;
}

void cFxStartButton::stop()
{
	int t = i_mode;
	i_mode = _prev_mode();
	fx[i_mode]->Stop();
	i_mode = t;
}

bool cFxStartButton::is_playing()
{
	bool res = false;

	res = fx[_prev_mode()]->IsPlaying();

	return res;
}

void cFxStartButton::mute(bool m)
{
	int i = _prev_mode();
	fx[i]->mute(m);
}