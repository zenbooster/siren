#pragma once
#include "cFxButton.h"
#include "cYelp1.h"

class cFxButtonsContainer;

class cFxMediumButton :
	public cFxButton
{
	static int i_refs_cnt;
	cYelp1 fx;
	cFxButtonsContainer* pfxButtonsContainer;
public:
	cFxMediumButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer);
	virtual ~cFxMediumButton();

	void play();
	virtual void on_highlight();
	virtual void on_unhighlight();
	void stop();
	bool is_playing();
	void mute(bool m);
};

