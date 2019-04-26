#include "stdafx.h"
#include "common.h"
#ifdef RESTRICTED_VERSION
#include "cStatusBar.h"
#include <CommCtrl.h>

#pragma comment (lib,  "comctl32.lib")

HWND cStatusBar::hwndStatus = NULL;

cStatusBar::cStatusBar()
{
}

cStatusBar::~cStatusBar()
{
}

// Description: 
//   Creates a status bar and divides it into the specified number of parts.
// Parameters:
//   hwndParent - parent window for the status bar.
//   idStatus - child window identifier of the status bar.
//   hinst - handle to the application instance.
//   cParts - number of parts into which to divide the status bar.
// Returns:
//   The handle to the status bar.
//
void cStatusBar::_DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts)
{
	RECT rcClient;
	HLOCAL hloc;
	PINT paParts;
	int i, nWidth;

	// Ensure that the common control DLL is loaded.
	InitCommonControls();

	// Create the status bar.
	hwndStatus = CreateWindowEx(
		0,                       // no extended styles
		STATUSCLASSNAME,         // name of status bar class
		(PCTSTR)NULL,           // no text when first created
														/*SBARS_SIZEGRIP |*/         // includes a sizing grip
		WS_CHILD | WS_VISIBLE,   // creates a visible child window
		0, 0, 0, 0,              // ignores size and position
		hwndParent,              // handle to parent window
		(HMENU)idStatus,       // child window identifier
		hinst,                   // handle to application instance
		NULL);                   // no window creation data

														 // Get the coordinates of the parent window's client area.
	GetClientRect(hwndParent, &rcClient);

	// Allocate an array for holding the right edge coordinates.
	hloc = LocalAlloc(LHND, sizeof(int) * cParts);
	paParts = (PINT)LocalLock(hloc);

	// Calculate the right edge coordinate for each part, and
	// copy the coordinates to the array.
	nWidth = rcClient.right / cParts;
	int rightEdge = nWidth;
	for (i = 0; i < cParts; i++) {
		paParts[i] = rightEdge;
		rightEdge += nWidth;
	}

	// Tell the status bar to create the window parts.
	SendMessage(hwndStatus, SB_SETPARTS, (WPARAM)cParts, (LPARAM)
		paParts);

	// Free the array, and return.
	LocalUnlock(hloc);
	LocalFree(hloc);
}

void cStatusBar::DoCreateStatusBar(HWND hwnd)
{
	if (!hwndStatus)
	{
		RECT rc;
		GetWindowRect(hwnd, &rc);
		_DoCreateStatusBar(hwnd, 0, 0, 1);
		RECT rc_status;
		GetWindowRect(hwndStatus, &rc_status);
		rc.bottom += rc_status.bottom - rc_status.top;
		MoveWindow(hwnd, rc.left, rc.top, rc.right - rc.left + 1, rc.bottom - rc.top + 1, false);
	}
}

void cStatusBar::Resize()
{
	if (hwndStatus)
		SendMessage(hwndStatus, WM_SIZE, 0, 0);
}

void cStatusBar::SetText(std::stringstream& ss_msg)
{
	std::string s_msg = ss_msg.str();
	std::wstring ws_msg(s_msg.begin(), s_msg.end());

	SendMessage(hwndStatus, SB_SETTEXT, SB_SIMPLEID & 0, (LPARAM)ws_msg.c_str());
}
#endif