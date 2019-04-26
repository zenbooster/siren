#pragma once
#include "cFxButton.h"
#include "cManual.h"

class cFxButtonsContainer;

class cFxManualButton :
	public cFxButton
{
	cManual manual;
	cFxButtonsContainer* pfxButtonsContainer;

public:
	cFxManualButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer);
	virtual ~cFxManualButton();

	void on_highlight();
	void on_unhighlight();

	void stop();
	bool is_playing();
	void mute(bool m);
};