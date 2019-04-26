#pragma once
#include "cFxButton.h"
#include "cFast.h"

class cFxButtonsContainer;

class cFxFastButton :
	public cFxButton
{
	static int i_refs_cnt;
	cFast fx;
	cFxButtonsContainer* pfxButtonsContainer;

public:
	cFxFastButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer);
	virtual ~cFxFastButton();

	void play();
	virtual void on_highlight();
	virtual void on_unhighlight();
	void stop();
	bool is_playing();
	void mute(bool m);
};

