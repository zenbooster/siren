#pragma once
#include "cFxButton.h"
#include "cHorn.h"

class cFxButtonsContainer;

class cFxHornButton :
	public cFxButton
{
	cHorn horn;
	cFxButtonsContainer* pfxButtonsContainer;
	cFxButton *pFxButtonPlayed;

public:
	cFxHornButton(HWND h_dlg, int i_control_id, cFxButtonsContainer* pfxButtonsContainer);
	virtual ~cFxHornButton();

	virtual void on_highlight();
	virtual void on_unhighlight();
};

