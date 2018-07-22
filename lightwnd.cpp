// ------------------------------------------------------------
// Light Window
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Math.h>
#include <Windows.h>
#include "Include\WinApp.h"
#include "Include\World.h"
#include "Include\Immersion.h"
#include "Include\Immersion.rh"
#include "Include\CommonCtrls.h"

//
// Local Variables
//
LIGHT			*SelLight;

// ------------------------------------------------------------
// int _export FAR PASCAL LightDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
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
int _export FAR PASCAL LightDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char	Buff[256];
	int	Index;

	switch(Message)
	{
		case WM_INITDIALOG:
			LIGHT *Light;
			RECT Rect;
			int i;

			GetClientRect(GetParent(hDlg), &Rect);
			MoveWindow(hDlg, Rect.right-196, 32, 192, Rect.bottom-42, false);
			ShowWindow(hDlg, SW_NORMAL);

			for(Light = World->Light; Light; Light = Light->Next)
			{
				SendMessage(GetDlgItem(hDlg, IDC_LGTLIST), LB_ADDSTRING, NULL, (LPARAM)Light->Identifier);
			}
			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case LBN_SELCHANGE:
					EnableWindow(GetDlgItem(hDlg, IDC_MODIFY), true);
					break;

				case IDC_MODIFY:
					Index = SendMessage(GetDlgItem(hDlg, IDC_LGTLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_LGTLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelLight = World->FindLight(Buff);
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_LGT), hDlg, (DLGPROC)EditLightDlgProc);
					break;

				case IDC_ADD:
					SelLight = World->AddLight("<Undefined>");
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_LGT), hDlg, (DLGPROC)EditLightDlgProc);
					SendMessage(GetDlgItem(hDlg, IDC_LGTLIST), LB_ADDSTRING, NULL, (LPARAM)SelLight->Identifier);
					break;

				case IDC_REMOVE:
					Index = SendMessage(GetDlgItem(hDlg, IDC_LGTLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_LGTLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelLight = World->FindLight(Buff);
					if(SelLight)
						World->RemoveLight(SelLight);
					break;
			}
			switch(HIWORD(wParam))
			{
				case LBN_SELCHANGE:
					EnableWindow(GetDlgItem(hDlg, IDC_MODIFY), true);
					EnableWindow(GetDlgItem(hDlg, IDC_REMOVE), true);
					break;

				case LBN_DBLCLK:
					Index = SendMessage(GetDlgItem(hDlg, IDC_LGTLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_LGTLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelLight = World->FindLight(Buff);
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_LGT), hDlg, (DLGPROC)EditLightDlgProc);
					break;
			}
			return 0;

		case WM_DESTROY:
			return 0;
	}
	return 0;
};

// ------------------------------------------------------------
// int _export FAR PASCAL EditLightDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Light information dialog box procedure handler
//
// Parameters
//  hDlg			- Handle of dialog
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
int _export FAR PASCAL EditLightDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char	Buff[256];
	
	switch(Message)
	{
		case WM_INITDIALOG:
			// Set Alias
			SetDlgItemText(hDlg, IDC_ALIAS, SelLight->Identifier);

			// Set Ambience
			sprintf(Buff, "%.3f", SelLight->Ambience);
			SetDlgItemText(hDlg, IDC_AMBIENCE, Buff);

			// Set X Translation
			sprintf(Buff, "%.3f", SelLight->xTranslate);
			SetDlgItemText(hDlg, IDC_TX, Buff);

			// Set Y Translation
			sprintf(Buff, "%.3f", SelLight->yTranslate);
			SetDlgItemText(hDlg, IDC_TY, Buff);

			// Set Z Translation
			sprintf(Buff, "%.3f", SelLight->zTranslate);
			SetDlgItemText(hDlg, IDC_TZ, Buff);

			// Set X Direction
			sprintf(Buff, "%.3f", SelLight->Direction.xComp);
			SetDlgItemText(hDlg, IDC_DX, Buff);

			// Set Y Direction
			sprintf(Buff, "%.3f", SelLight->Direction.yComp);
			SetDlgItemText(hDlg, IDC_DY, Buff);

			// Set Z Direction
			sprintf(Buff, "%.3f", SelLight->Direction.zComp);
			SetDlgItemText(hDlg, IDC_DZ, Buff);

			// Set On Status
			if(SelLight->On)
				CheckDlgButton(hDlg, IDC_TURNEDON, BST_CHECKED);
			else
				CheckDlgButton(hDlg, IDC_TURNEDON, BST_UNCHECKED);

			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_OK:
					// Get Alias
					GetDlgItemText(hDlg, IDC_ALIAS, SelLight->Identifier, sizeof(SelLight->Identifier));

					// Get Ambience
					GetDlgItemText(hDlg, IDC_AMBIENCE, Buff, sizeof(Buff));
					SelLight->Ambience = atof(Buff);

					// Get X Translation
					GetDlgItemText(hDlg, IDC_TX, Buff, sizeof(Buff));
					SelLight->xTranslate = atof(Buff);

					// Get Y Translation
					GetDlgItemText(hDlg, IDC_TY, Buff, sizeof(Buff));
					SelLight->yTranslate = atof(Buff);

					// Get Z Translation
					GetDlgItemText(hDlg, IDC_TZ, Buff, sizeof(Buff));
					SelLight->zTranslate = atof(Buff);

					// Get X Direction
					GetDlgItemText(hDlg, IDC_DX, Buff, sizeof(Buff));
					SelLight->Direction.xComp = atof(Buff);

					// Get Y Direction
					GetDlgItemText(hDlg, IDC_DY, Buff, sizeof(Buff));
					SelLight->Direction.yComp = atof(Buff);

					// Get Z Direction
					GetDlgItemText(hDlg, IDC_DZ, Buff, sizeof(Buff));
					SelLight->Direction.zComp = atof(Buff);

					// On/Off Status
					if(IsDlgButtonChecked(hDlg, IDC_TURNEDON) == BST_CHECKED)
						SelLight->On = true;
					else
						SelLight->On = false;

					Modified = true;
					EndDialog(hDlg, true);
					break;

				case IDC_CANCEL:
					EndDialog(hDlg, true);
					break;
			}
			return 0;

		case WM_DESTROY:
			return 0;
	}
	return 0;
};