// ------------------------------------------------------------
// DynaScape v1.0
// Copyright 1998 (c) Leprechaun Software.  All Rigthts Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "WinApp.h"
#include "DynaScape.rh"

//
// Variables
//
WINAPP			WinApp;
HMENU          hMenuInit;

//
// Procedure Prototypes
//
int _export FAR PASCAL AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

// ------------------------------------------------------------
// int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR nCmdLine, int cmdShow)
// ------------------------------------------------------------
// Description
//  Window entry point
//
// Parameters
//  hInstance		- Handle of instance
//  hPrevInst		- Handle of previous instance
//  nCmdLine		- Command line string
//  cmdShow			- Window view
//
// Returns
//  Status code
// ------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR cmdString, int cmdShow)
{
	HWND        MainWnd;
	MSG         Msg;
	WNDCLASS    WndClass;

	WinApp.hInstance = hInstance;
	WinApp.hPrevInst = hPrevInst;
	WinApp.cmdShow   = cmdShow;

	if(!WinApp.hPrevInst)
	{
		WndClass.style          = CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc    = WndProc;
		WndClass.cbClsExtra     = 0;
		WndClass.cbWndExtra     = 0;
		WndClass.hInstance      = hInstance;
		WndClass.hIcon          = LoadIcon(WinApp.hInstance, MAKEINTRESOURCE(IDI_ICON1));
		WndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
		WndClass.hbrBackground  = (HBRUSH)(GetStockObject(LTGRAY_BRUSH));
		WndClass.lpszMenuName   = NULL;
		WndClass.lpszClassName  = "MAINWND";

		RegisterClass(&WndClass);
	}

	hMenuInit = LoadMenu(WinApp.hInstance, MAKEINTRESOURCE(IDM_MENU1));
	MainWnd   = CreateWindow("MAINWND",
					"Dynamic Landscape Modeller v1.0",
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, CW_USEDEFAULT,
					CW_USEDEFAULT, CW_USEDEFAULT,
					NULL, hMenuInit, hInstance, NULL);

	ShowWindow(MainWnd, SW_NORMAL);
	UpdateWindow(MainWnd);

	while(GetMessage(&Msg, 0, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
};

// ------------------------------------------------------------
// LRESULT CALLBACK _export WndProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Window message handler
//
// Parameters
//  hDlg				- Handle of window
//  Message			- Message code
//  wParam			- Word parameter
//  lParam			- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
LRESULT CALLBACK _export WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch(message)
	{
		case WM_CREATE:
			return 0;

		case WM_PAINT:
			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case CM_EXIT:
					PostQuitMessage(0);
            	return 0;
               
				case CM_ABOUT:
            	DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_ABOUT), hWindow, (DLGPROC)AboutDlgProc);
					return 0;            	
         }
         return 0;

		default:
			return(DefWindowProc(hWindow, message, wParam, lParam));         
   }
};

// ##################################
// #<<<<<<< About Dlg Proc >>>>>>>>>#
// ##################################
int _export FAR PASCAL AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
   	case WM_COMMAND:
      	switch(wParam)
         {
         	case IDOK:
            	EndDialog(hDlg, TRUE);
               break;
         }
         return 0;
	}
	return 0;
};