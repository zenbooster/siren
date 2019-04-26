#pragma once
#include "cFxButton.h"
#include "cHiLo.h"

class cFxButtonsContainer;

class cFxHiLoButton :
	public cFxButton
{
	cHiLo hilo;
	cFxButtonsContainer* pfxButtonsContainer;

public:
	cFxHiLoButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer);
	virtual ~cFxHiLoButton();

	virtual void on_highlight();
	virtual void on_unhighlight();

	void stop();
	bool is_playing();
	void mute(bool m);
};

