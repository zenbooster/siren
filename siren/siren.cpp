// siren.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "siren.h"
#include "cFxButtonsContainer.h"
#include "common.h"
#ifdef RESTRICTED_VERSION
#include "cRestrictedVersion.h"
#endif

cFxButtonsContainer *pfxButtonsContainer = NULL;

// Message handler for dialog box.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	#ifdef RESTRICTED_VERSION
		cRestrictedVersion::init(hDlg);
	#endif

		if (!InitializeOpenAL())
		{
			MessageBox(hDlg, _T("InitializeOpenAL() failed!"), _T("Error:"), MB_OK | MB_ICONERROR);
			ExitProcess(1);
		}
		{
			pfxButtonsContainer = new cFxButtonsContainer(hDlg);
		}
		return (INT_PTR)TRUE;

	case WM_DESTROY:
		pfxButtonsContainer->stop();
		delete pfxButtonsContainer;
		DestroyOpenAL();

#ifdef RESTRICTED_VERSION
	case WM_SIZE:
		cStatusBar::Resize();
#endif

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);

	return 0;
}