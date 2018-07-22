// ------------------------------------------------------------
// Tab Info Window
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\WinApp.h"
#include "Include\World.h"
#include "Include\Immersion.h"
#include "Include\Immersion.rh"
#include "Include\CommonCtrls.h"

//
// Local Function Prototypes
//
void	LocateMusic(HWND hWindow);
void	LocatePalette(HWND hWindow);

// ------------------------------------------------------------
// int _export FAR PASCAL WorldDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  About dialog box procedure handler
//
// Parameters
//  hDlg			- Handle of dialog
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
int _export FAR PASCAL WorldDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:
			RECT Rect;

			GetClientRect(GetParent(hDlg), &Rect);
			MoveWindow(hDlg, Rect.right-196, 32, 192, Rect.bottom-42, false);
			ShowWindow(hDlg, SW_NORMAL);

			SetWindowText(GetDlgItem(hDlg, IDC_DESCRIPTION), World->Description);
			SetDlgItemText(hDlg, IDC_DESCRIPTION, World->Description);
			SetDlgItemInt(hDlg, IDC_NUMOBJECTS, World->NumObjects, false);
			SetDlgItemInt(hDlg, IDC_NUMCAMERAS, World->NumCameras, false);
			SetDlgItemInt(hDlg, IDC_NUMLIGHTS, World->NumLights, false);
			return 0;

		case WM_COMMAND:
      	switch(wParam)
         {
				case IDC_MODIFY:
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_WLD), hDlg, (DLGPROC)EditWorldDlgProc);
					break;
			}
			return 0;

		case WM_DESTROY:
			return 0;
	}
	return 0;
};

// ------------------------------------------------------------
// int _export FAR PASCAL EditWorldDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  World information dialog box procedure handler
//
// Parameters
//  hDlg			- Handle of dialog
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
int _export FAR PASCAL EditWorldDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char	Buff[256];

	switch(Message)
	{
		case WM_INITDIALOG:
			// Set Description
			SetDlgItemText(hDlg, IDC_DESCRIPTION, World->Description);

			// Set Filename
         SetDlgItemText(hDlg, IDC_FILENAME, World->Filename);

			// Set Interface
			SetDlgItemText(hDlg, IDC_DLL, World->Interface);

			// Set Music
			SetDlgItemText(hDlg, IDC_MUSIC, World->MusicFile);

			// Set Palette
			SetDlgItemText(hDlg, IDC_PALETTE, World->Palette);

			// Set Color Components of Background
			SetDlgItemInt(hDlg, IDC_RED, World->BGColor.rgbRed, false);
			SetDlgItemInt(hDlg, IDC_GREEN, World->BGColor.rgbGreen, false);
			SetDlgItemInt(hDlg, IDC_BLUE, World->BGColor.rgbBlue, false);

			// Set Gravity
			sprintf(Buff, "%.3f", World->Gravity);
			SetDlgItemText(hDlg, IDC_GRAVITY, Buff);
			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_OK:
					// Get Alias
					GetDlgItemText(hDlg, IDC_DESCRIPTION, World->Description, sizeof(World->Description));

					// Get Interface
					GetDlgItemText(hDlg, IDC_DLL, Buff, sizeof(Buff));
					World->SetInterface(Buff);

					// Get Music
					GetDlgItemText(hDlg, IDC_MUSIC, World->MusicFile, sizeof(World->MusicFile));
					World->SetMusic(World->MusicFile);

					// Get Palette
					GetDlgItemText(hDlg, IDC_PALETTE, Buff, sizeof(Buff));
               World->SetPalette(Buff);

					// Get Color Components of Background
					World->BGColor.rgbRed	= GetDlgItemInt(hDlg, IDC_RED, NULL, false);
					World->BGColor.rgbGreen	= GetDlgItemInt(hDlg, IDC_GREEN, NULL, false);
					World->BGColor.rgbBlue	= GetDlgItemInt(hDlg, IDC_BLUE, NULL, false);

					// Get Gravity
					GetDlgItemText(hDlg, IDC_GRAVITY, Buff, sizeof(Buff));
					World->SetGravity(atof(Buff));

					Modified = true;
					EndDialog(hDlg, true);
					break;

				case IDC_CANCEL:
					EndDialog(hDlg, true);
					break;

				case IDC_SEARCHDLL:
					LocateInterface(hDlg);
					break;

				case IDC_SEARCHMUSIC:
					LocateMusic(hDlg);
					break;

				case IDC_SEARCHPALETTE:
					LocatePalette(hDlg);
					break;
			}
			return 0;

		case WM_DESTROY:
			return 0;
	}
	return 0;
};

// ------------------------------------------------------------
// void LocateMusic(HWND hWindow)
// ------------------------------------------------------------
// Description
//  Allows the user to locate the desired music file
//
// Parameters
//  hWindow		- Parent window
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LocateMusic(HWND hWindow)
{
	OPENFILENAME ofn;
	char 	szFile[80], szDirName[80];
	char	szFileTitle[16];
	char	szFilter[80] = "Music file (*.mid)\0*.mid\0";

	szFile[0] = '\0';
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize 		= sizeof(OPENFILENAME);
	ofn.hwndOwner 			= hWindow;
	ofn.lpstrFilter 		= szFilter;
	ofn.nFilterIndex 		= 0;
	ofn.lpstrFile			= szFile;
	ofn.nMaxFile 			= sizeof(szFile);
	ofn.lpstrFileTitle 	= szFileTitle;
	ofn.nMaxFileTitle 	= sizeof(szFileTitle);
	ofn.lpstrInitialDir 	= szDirName;
	ofn.lpstrTitle 		= "Specify background music";
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		SetDlgItemText(hWindow, IDC_MUSIC, szFile);
	}
};

// ------------------------------------------------------------
// void LocatePalette(HWND hWindow)
// ------------------------------------------------------------
// Description
//  Allows the user to locate the desired palette file
//
// Parameters
//  hWindow		- Parent window
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LocatePalette(HWND hWindow)
{
	OPENFILENAME ofn;
	char 	szFile[80], szDirName[80];
	char	szFileTitle[16];
	char	szFilter[80] = "Palette file (*.pal)\0*.pal\0";

	szFile[0] = '\0';
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize 		= sizeof(OPENFILENAME);
	ofn.hwndOwner 			= hWindow;
	ofn.lpstrFilter 		= szFilter;
	ofn.nFilterIndex 		= 0;
	ofn.lpstrFile			= szFile;
	ofn.nMaxFile 			= sizeof(szFile);
	ofn.lpstrFileTitle 	= szFileTitle;
	ofn.nMaxFileTitle 	= sizeof(szFileTitle);
	ofn.lpstrInitialDir 	= szDirName;
	ofn.lpstrTitle 		= "Specify palette";
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		SetDlgItemText(hWindow, IDC_PALETTE, szFile);
	}
};
