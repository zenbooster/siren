#include "stdafx.h"
#include "Windows.h"
#include "cFxButton.h"
#include "common.h"
#include <Commctrl.h>
#include <sstream>
#ifdef RESTRICTED_VERSION
#include "cRestrictedVersion.h"
#endif

std::map<HWND, cFxButton*> cFxButton::buttons;

LRESULT CALLBACK cFxButton::WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		break;
	case BM_SETSTATE:
	{
		cFxButton *p_fx_button = buttons[hwnd];
		if ((BOOL)wParam)
		{
			if (p_fx_button->is_on_unhighlight_called)
			{
			#ifdef RESTRICTED_VERSION
				cRestrictedVersion::on_highlight();
			#endif
				p_fx_button->on_highlight();
				p_fx_button->is_on_unhighlight_called = false;
			}
		}
		else
		{
			if (!p_fx_button->is_on_unhighlight_called)
			{
				p_fx_button->on_unhighlight();
				p_fx_button->is_on_unhighlight_called = true;
			}
		}
		break;
	}
	}

	LRESULT res = CallWindowProc(buttons[hwnd]->p_window_proc, hwnd, uMsg, wParam, lParam);

	if (uMsg == WM_NCDESTROY) // предполагается, что это последнее сообщение, которое получит окно.
	{
		buttons.erase(hwnd);
	}
	return res;
}

cFxButton::cFxButton(HWND h_dlg, int i_control_id):
	h_dlg(h_dlg),
	i_control_id(i_control_id),
	is_on_unhighlight_called(true)
{
	h_wnd = GetDlgItem(h_dlg, i_control_id);
#ifdef _WIN64
	p_window_proc = (WNDPROC)GetWindowLongPtr(h_wnd, GWLP_WNDPROC);
#else
	p_window_proc = (WNDPROC)GetWindowLongPtr(h_wnd, GWL_WNDPROC);
#endif
	buttons[h_wnd] = this;
#ifdef _WIN64
	SetWindowLongPtr(h_wnd, GWLP_WNDPROC, (LONG_PTR)&WindowProc);
#else
	SetWindowLongPtr(h_wnd, GWL_WNDPROC, (LONG_PTR)&WindowProc);
#endif
}


cFxButton::~cFxButton()
{
	if (buttons[h_wnd])
	{
	#ifdef _WIN64
		SetWindowLongPtr(h_wnd, GWLP_WNDPROC, (LONG_PTR)p_window_proc);
	#else
		SetWindowLongPtr(h_wnd, GWL_WNDPROC, (LONG_PTR)p_window_proc);
	#endif
		buttons.erase(h_wnd);
	}
}
