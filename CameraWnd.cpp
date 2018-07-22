// ------------------------------------------------------------
// Camera Dlg Window
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Math.h>
#include <Windows.h>
#include "Include\WinApp.h"
#include "Include\World.h"
#include "Include\Immersion.rh"
#include "Include\Immersion.h"

//
// Variables
//
CAMERA		*SelCamera;

// ------------------------------------------------------------
// int _export FAR PASCAL CameraDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
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
int _export FAR PASCAL CameraDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char	Buff[256];
	int	Index;

	switch(Message)
	{
		case WM_INITDIALOG:
			CAMERA *Camera;
			RECT Rect;
			int i;

			GetClientRect(GetParent(hDlg), &Rect);
			MoveWindow(hDlg, Rect.right-196, 32, 192, Rect.bottom-42, false);
			ShowWindow(hDlg, SW_NORMAL);

			for(Camera = World->Camera; Camera; Camera = Camera->Next)
			{
				SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_ADDSTRING, NULL, (LPARAM)Camera->Identifier);
			}
			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_SWITCHCAM:
					Index = SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelCamera = World->FindCamera(Buff);
					World->SwitchToCamera(SelCamera);				
					break;

				case IDC_MODIFY:
					Index = SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelCamera = World->FindCamera(Buff);
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_CAM), hDlg, (DLGPROC)EditCameraDlgProc);
					break;

				case IDC_ADD:
					SelCamera = World->AddCamera("<Undefined>");
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_CAM), hDlg, (DLGPROC)EditCameraDlgProc);
					SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_ADDSTRING, NULL, (LPARAM)SelCamera->Identifier);
					break;

				case IDC_REMOVE:
					Index = SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelCamera = World->FindCamera(Buff);
					if(SelCamera)
						World->RemoveCamera(SelCamera);
					break;
			}
			switch(HIWORD(wParam))
			{
				case LBN_SELCHANGE:
					EnableWindow(GetDlgItem(hDlg, IDC_MODIFY), true);
					EnableWindow(GetDlgItem(hDlg, IDC_REMOVE), true);
					EnableWindow(GetDlgItem(hDlg, IDC_SWITCHCAM), true);
					break;

				case LBN_DBLCLK:
					Index = SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_CAMLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelCamera = World->FindCamera(Buff);
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_CAM), hDlg, (DLGPROC)EditCameraDlgProc);
					break;
			}
			return 0;

		case WM_DESTROY:
			return 0;
	}
	return 0;
};

// ------------------------------------------------------------
// int _export FAR PASCAL EditCameraDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Camera information dialog box procedure handler
//
// Parameters
//  hDlg			- Handle of dialog
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
int _export FAR PASCAL EditCameraDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char	Buff[256];

	switch(Message)
	{
		case WM_INITDIALOG:
			// Set Alias
			SetDlgItemText(hDlg, IDC_ALIAS, SelCamera->Identifier);

			// Set Yon
			sprintf(Buff, "%d", SelCamera->Yon);
			SetDlgItemText(hDlg, IDC_YON, Buff);

			// Set Hither
			sprintf(Buff, "%d", SelCamera->Hither);
			SetDlgItemText(hDlg, IDC_HITHER, Buff);

			// Set Projection Plane
			sprintf(Buff, "%d", SelCamera->ProjectionPlane);
			SetDlgItemText(hDlg, IDC_PROPLANE, Buff);

			// Set X Translation
			sprintf(Buff, "%.3f", SelCamera->xTranslate);
			SetDlgItemText(hDlg, IDC_TX, Buff);

			// Set Y Translation
			sprintf(Buff, "%.3f", SelCamera->yTranslate);
			SetDlgItemText(hDlg, IDC_TY, Buff);

			// Set Z Translation
			sprintf(Buff, "%.3f", SelCamera->zTranslate);
			SetDlgItemText(hDlg, IDC_TZ, Buff);

			// Set X Rotation
			sprintf(Buff, "%.3f", (SelCamera->Pitch * (180/M_PI)));
			SetDlgItemText(hDlg, IDC_RX, Buff);

			// Set Y Rotation
			sprintf(Buff, "%.3f", (SelCamera->Yaw * (180/M_PI)));
			SetDlgItemText(hDlg, IDC_RY, Buff);

			// Set Z Rotation
			sprintf(Buff, "%.3f", (SelCamera->Roll * (180/M_PI)));
			SetDlgItemText(hDlg, IDC_RZ, Buff);
			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_OK:
					// Get Alias
					GetDlgItemText(hDlg, IDC_ALIAS, SelCamera->Identifier, sizeof(SelCamera->Identifier));

					// Get Yon
					GetDlgItemText(hDlg, IDC_YON, Buff, sizeof(Buff));
					SelCamera->Yon = atol(Buff);

					// Get Hither
					GetDlgItemText(hDlg, IDC_HITHER, Buff, sizeof(Buff));
					SelCamera->Hither = atol(Buff);

					// Get Projection Plane
					GetDlgItemText(hDlg, IDC_PROPLANE, Buff, sizeof(Buff));
					SelCamera->ProjectionPlane = atol(Buff);

					// Get X Translation
					GetDlgItemText(hDlg, IDC_TX, Buff, sizeof(Buff));
					SelCamera->xTranslate = atof(Buff);

					// Get Y Translation
					GetDlgItemText(hDlg, IDC_TY, Buff, sizeof(Buff));
					SelCamera->yTranslate = atof(Buff);

					// Get Z Translation
					GetDlgItemText(hDlg, IDC_TZ, Buff, sizeof(Buff));
					SelCamera->zTranslate = atof(Buff);

					// Get X Rotation
					GetDlgItemText(hDlg, IDC_RX, Buff, sizeof(Buff));
					SelCamera->Pitch = (atof(Buff) * (M_PI/180));

					// Get Y Rotation
					GetDlgItemText(hDlg, IDC_RY, Buff, sizeof(Buff));
					SelCamera->Yaw = (atof(Buff) * (M_PI/180));

					// Get Z Rotation
					GetDlgItemText(hDlg, IDC_RZ, Buff, sizeof(Buff));
					SelCamera->Roll = (atol(Buff) * (M_PI/180));

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