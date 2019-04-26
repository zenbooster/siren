#include "stdafx.h"
#include "common.h"
#ifdef RESTRICTED_VERSION
#include "cRestrictedVersion.h"
#include "tstring.h"

int cRestrictedVersion::i_button_presses_left = MAX_BUTTON_PRESSES;

cRestrictedVersion::cRestrictedVersion()
{
}

cRestrictedVersion::~cRestrictedVersion()
{
}

void cRestrictedVersion::init(HWND hwnd)
{
	int sz_caption = GetWindowTextLength(hwnd) + 1;
	LPTSTR p_caption = new TCHAR[sz_caption];
	GetWindowText(hwnd, p_caption, sz_caption);
	std::tstring ts_caption(p_caption);
	ts_caption += _T(" (trial version)");
	SetWindowText(hwnd, ts_caption.c_str());

	cStatusBar::DoCreateStatusBar(hwnd);
	what_button_presses_left();
}

void cRestrictedVersion::on_highlight()
{
	if (!i_button_presses_left--)
	{
		std::stringstream ss_msg;
		ss_msg << "In the trial version, the number of button presses is limited to " << MAX_BUTTON_PRESSES;
		std::string s_msg = ss_msg.str();
		std::wstring ws_msg(s_msg.begin(), s_msg.end());
		ERRMSG(ws_msg.c_str());
		exit(0);
	}
	what_button_presses_left();
}

void cRestrictedVersion::what_button_presses_left()
{
	std::stringstream ss_msg;
	ss_msg << i_button_presses_left << " button presses left.";
	cStatusBar::SetText(ss_msg);
}
#endif