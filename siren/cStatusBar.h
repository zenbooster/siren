#pragma once
#ifdef RESTRICTED_VERSION
#include <sstream>

class cStatusBar
{
	static void _DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);
public:
	cStatusBar();
	~cStatusBar();

	static void DoCreateStatusBar(HWND hwnd);
	static void Resize();
	static void SetText(std::stringstream& ss);

	static HWND hwndStatus;
};
#endif