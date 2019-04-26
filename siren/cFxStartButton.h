#pragma once
#include "cFxButton.h"
#include "remSnd.h"

class cFxButtonsContainer;

class cFxStartButton :
	public cFxButton
{
	static VOID CALLBACK cFxStartButton::MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);

	int i_prev_mode;
	int i_mode;
	static int i_refs_cnt;
	static remSnd *fx[4];
	static const size_t fx_size = sizeof(cFxStartButton::fx) / sizeof(remSnd*);
	cFxButtonsContainer* pfxButtonsContainer;

	int _prev_mode();
	void _next_mode();

public:
	cFxStartButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer);
	virtual ~cFxStartButton();

	void play();
	virtual void on_highlight();
	virtual void on_unhighlight();
	void reset();
	void stop();
	bool is_playing();
	void mute(bool m);
};

