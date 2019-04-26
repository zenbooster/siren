#pragma once
#include "cFxButton.h"
#include "cWail.h"

class cFxButtonsContainer;

class cFxWailButton :
	public cFxButton
{
	cWail wail;
	cFxButtonsContainer* pfxButtonsContainer;

public:
	cFxWailButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer);
	virtual ~cFxWailButton();

	virtual void on_highlight();
	virtual void on_unhighlight();

	void stop();
	bool is_playing();
	void mute(bool m);
};

