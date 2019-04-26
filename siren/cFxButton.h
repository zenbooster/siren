#pragma once
#include <map>

class cFxButton
{
protected:
	HWND h_dlg;
	int i_control_id;
	HWND h_wnd;
	WNDPROC p_window_proc;
	BOOL is_on_unhighlight_called;
	
	static std::map<HWND, cFxButton*> buttons;

	static LRESULT CALLBACK WindowProc(
		_In_ HWND   hwnd,
		_In_ UINT   uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

public:
	cFxButton(HWND h_dlg, int i_control_id);
	virtual ~cFxButton();

	virtual void on_highlight() = 0;
	virtual void on_unhighlight() = 0;
	virtual void stop() {};
	//virtual void play() {};
	virtual void mute(bool m) {};
};

