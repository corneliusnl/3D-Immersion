// ------------------------------------------------------------
// Tab Info Window
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\WinApp.h"
#include "Include\World.h"
#include "Include\Immersion.rh"
#include "Include\Immersion.h"

//
// Variables
//
OBJECT		*SelObject;

char 	szFile[80], szDirName[80];
char	szFileTitle[16];
char	szFilter[80] = "Object file (*.odf)\0*.odf\0";

//
// Local function prototypes
//
void AddObject(HWND hWindow);

// ------------------------------------------------------------
// int _export FAR PASCAL ObjectDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
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
int _export FAR PASCAL ObjectDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char		Buff[256];
	int		Index;

	switch(Message)
	{
		case WM_INITDIALOG:
			OBJECT	*Obj;
			RECT		Rect;
			int		i;

			GetClientRect(GetParent(hDlg), &Rect);
			MoveWindow(hDlg, Rect.right-196, 32, 192, Rect.bottom-42, false);
			ShowWindow(hDlg, SW_NORMAL);

			for(Obj = World->Object; Obj; Obj = Obj->Next)
				SendMessage(GetDlgItem(hDlg, IDC_OBJLIST), LB_ADDSTRING, NULL, (LPARAM)Obj->Identifier);

			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_MODIFY:
					Index = SendMessage(GetDlgItem(hDlg, IDC_OBJLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_OBJLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelObject = World->FindObject(Buff);
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_OBJ), hDlg, (DLGPROC)EditObjectDlgProc);
					break;

				case IDC_ADD:
					AddObject(hDlg);
					SendMessage(GetDlgItem(hDlg, IDC_OBJLIST), LB_ADDSTRING, NULL, (LPARAM)SelObject->Identifier);
					break;

				case IDC_REMOVE:
					Index = SendMessage(GetDlgItem(hDlg, IDC_OBJLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_OBJLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelObject = World->FindObject(Buff);
					if(SelObject)
						World->RemoveObject(SelObject);
					break;
			}
			switch(HIWORD(wParam))
			{
				case LBN_SELCHANGE:
					EnableWindow(GetDlgItem(hDlg, IDC_MODIFY), true);
					EnableWindow(GetDlgItem(hDlg, IDC_REMOVE), true);
					break;

				case LBN_DBLCLK:
					Index = SendMessage(GetDlgItem(hDlg, IDC_OBJLIST), LB_GETCURSEL, 0, 0L);
					SendMessage(GetDlgItem(hDlg, IDC_OBJLIST), LB_GETTEXT, Index, (LPARAM)&Buff);
					SelObject = World->FindObject(Buff);
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_OBJ), hDlg, (DLGPROC)EditObjectDlgProc);
					break;
			}
			return 0;

		case WM_DESTROY:
			return 0;
	}
	return 0;
};

// ------------------------------------------------------------
// int _export FAR PASCAL EditObjectDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Object information dialog box procedure handler
//
// Parameters
//  hDlg			- Handle of dialog
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
int _export FAR PASCAL EditObjectDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char	Buff[256];

	switch(Message)
	{
		case WM_INITDIALOG:
			// Set filename
			SetDlgItemText(hDlg, IDC_FILENAME, SelObject->Filename);

			// Set alias
			SetDlgItemText(hDlg, IDC_ALIAS, SelObject->Identifier);

			// Set dynamic link library
			SetDlgItemText(hDlg, IDC_DLL, SelObject->Interface);

			// Set Scaling
			sprintf(Buff, "%.3f", SelObject->xScale);
			SetDlgItemText(hDlg, IDC_SX, Buff);
			sprintf(Buff, "%.3f", SelObject->yScale);
			SetDlgItemText(hDlg, IDC_SY, Buff);
			sprintf(Buff, "%.3f", SelObject->zScale);
			SetDlgItemText(hDlg, IDC_SZ, Buff);

			// Set Rotation
			sprintf(Buff, "%.3f", (SelObject->xRotate * (180/M_PI)));
			SetDlgItemText(hDlg, IDC_RX, Buff);
			sprintf(Buff, "%.3f", (SelObject->yRotate * (180/M_PI)));
			SetDlgItemText(hDlg, IDC_RY, Buff);
			sprintf(Buff, "%.3f", (SelObject->zRotate * (180/M_PI)));
			SetDlgItemText(hDlg, IDC_RZ, Buff);

			// Set Translation
			sprintf(Buff, "%.3f", SelObject->xTranslate);
			SetDlgItemText(hDlg, IDC_TX, Buff);
			sprintf(Buff, "%.3f", SelObject->yTranslate);
			SetDlgItemText(hDlg, IDC_TY, Buff);
			sprintf(Buff, "%.3f", SelObject->zTranslate);
			SetDlgItemText(hDlg, IDC_TZ, Buff);

			// Set Linear Velocity
			sprintf(Buff, "%.3f", SelObject->LinearVelocity.xComp);
			SetDlgItemText(hDlg, IDC_LVX, Buff);
			sprintf(Buff, "%.3f", SelObject->LinearVelocity.yComp);
			SetDlgItemText(hDlg, IDC_LVY, Buff);
			sprintf(Buff, "%.3f", SelObject->LinearVelocity.zComp);
			SetDlgItemText(hDlg, IDC_LVZ, Buff);

			// Set Linear Acceleration
			sprintf(Buff, "%.3f", SelObject->LinearAcceleration.xComp);
			SetDlgItemText(hDlg, IDC_LAX, Buff);
			sprintf(Buff, "%.3f", SelObject->LinearAcceleration.yComp);
			SetDlgItemText(hDlg, IDC_LAY, Buff);
			sprintf(Buff, "%.3f", SelObject->LinearAcceleration.zComp);
			SetDlgItemText(hDlg, IDC_LAZ, Buff);

			// Set Rotational Velocity
			sprintf(Buff, "%.3f", SelObject->RotationalVelocity.xComp);
			SetDlgItemText(hDlg, IDC_RVX, Buff);
			sprintf(Buff, "%.3f", SelObject->RotationalVelocity.yComp);
			SetDlgItemText(hDlg, IDC_RVY, Buff);
			sprintf(Buff, "%.3f", SelObject->RotationalVelocity.zComp);
			SetDlgItemText(hDlg, IDC_RVZ, Buff);

			// Set Rotational Acceleration
			sprintf(Buff, "%.3f", SelObject->RotationalAcceleration.xComp);
			SetDlgItemText(hDlg, IDC_RAX, Buff);
			sprintf(Buff, "%.3f", SelObject->RotationalAcceleration.yComp);
			SetDlgItemText(hDlg, IDC_RAY, Buff);
			sprintf(Buff, "%.3f", SelObject->RotationalAcceleration.zComp);
			SetDlgItemText(hDlg, IDC_RAZ, Buff);

			// Set Mass
			sprintf(Buff, "%.3f", SelObject->Mass);
			SetDlgItemText(hDlg, IDC_MASS, Buff);

			// Set Force
			sprintf(Buff, "%.3f", SelObject->Force);
			SetDlgItemText(hDlg, IDC_FORCE, Buff);

			// Set Friction
			sprintf(Buff, "%.3f", SelObject->Friction);
			SetDlgItemText(hDlg, IDC_FRICTION, Buff);

			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_OK:
					// Get Alias
					GetDlgItemText(hDlg, IDC_ALIAS, SelObject->Identifier, sizeof(SelObject->Identifier));

					// Get Interface
					GetDlgItemText(hDlg, IDC_DLL, Buff, sizeof(Buff));
					if(strcmp("<Undefined>", Buff) != NULL)
						SelObject->SetInterface(Buff);

					// Get Scaling
					GetDlgItemText(hDlg, IDC_SX, Buff, sizeof(Buff));
					SelObject->xScale = atof(Buff);
					GetDlgItemText(hDlg, IDC_SY, Buff, sizeof(Buff));
					SelObject->yScale = atof(Buff);
					GetDlgItemText(hDlg, IDC_SZ, Buff, sizeof(Buff));
					SelObject->zScale = atof(Buff);

					// Get Rotation
					GetDlgItemText(hDlg, IDC_RX, Buff, sizeof(Buff));
					SelObject->xRotate = (atof(Buff) * (M_PI/180));
					GetDlgItemText(hDlg, IDC_RY, Buff, sizeof(Buff));
					SelObject->yRotate = (atof(Buff) * (M_PI/180));
					GetDlgItemText(hDlg, IDC_RZ, Buff, sizeof(Buff));
					SelObject->zRotate = (atof(Buff) * (M_PI/180));

					// Get Translation
					GetDlgItemText(hDlg, IDC_TX, Buff, sizeof(Buff));
					SelObject->xTranslate = atof(Buff);
					GetDlgItemText(hDlg, IDC_TY, Buff, sizeof(Buff));
					SelObject->yTranslate = atof(Buff);
					GetDlgItemText(hDlg, IDC_TZ, Buff, sizeof(Buff));
					SelObject->zTranslate = atof(Buff);

					// Get Linear Velocity
					GetDlgItemText(hDlg, IDC_LVX, Buff, sizeof(Buff));
					SelObject->LinearVelocity.xComp = atof(Buff);
					GetDlgItemText(hDlg, IDC_LVY, Buff, sizeof(Buff));
					SelObject->LinearVelocity.yComp = atof(Buff);
					GetDlgItemText(hDlg, IDC_LVZ, Buff, sizeof(Buff));
					SelObject->LinearVelocity.zComp = atof(Buff);

					// Get Linear Acceleration
					GetDlgItemText(hDlg, IDC_LAX, Buff, sizeof(Buff));
					SelObject->LinearAcceleration.xComp = atof(Buff);
					GetDlgItemText(hDlg, IDC_LAY, Buff, sizeof(Buff));
					SelObject->LinearAcceleration.yComp = atof(Buff);
					GetDlgItemText(hDlg, IDC_LAZ, Buff, sizeof(Buff));
					SelObject->LinearAcceleration.zComp = atof(Buff);

					// Get Rotational Velocity
					GetDlgItemText(hDlg, IDC_RVX, Buff, sizeof(Buff));
					SelObject->RotationalVelocity.xComp = atof(Buff);
					GetDlgItemText(hDlg, IDC_RVY, Buff, sizeof(Buff));
					SelObject->RotationalVelocity.yComp = atof(Buff);
					GetDlgItemText(hDlg, IDC_RVZ, Buff, sizeof(Buff));
					SelObject->RotationalVelocity.zComp = atof(Buff);

					// Get Rotational Acceleration
					GetDlgItemText(hDlg, IDC_RAX, Buff, sizeof(Buff));
					SelObject->RotationalAcceleration.xComp = atof(Buff);
					GetDlgItemText(hDlg, IDC_RAY, Buff, sizeof(Buff));
					SelObject->RotationalAcceleration.yComp = atof(Buff);
					GetDlgItemText(hDlg, IDC_RAZ, Buff, sizeof(Buff));
					SelObject->RotationalAcceleration.zComp = atof(Buff);

					// Get Mass
					GetDlgItemText(hDlg, IDC_MASS, Buff, sizeof(Buff));
					SelObject->Mass = atof(Buff);

					// Get Force
					GetDlgItemText(hDlg, IDC_FORCE, Buff, sizeof(Buff));
					SelObject->Force = atof(Buff);

					// Get Friction
					GetDlgItemText(hDlg, IDC_FRICTION, Buff, sizeof(Buff));
					SelObject->Friction = atof(Buff);

					Modified = true;
					EndDialog(hDlg, true);
					break;

				case IDC_CANCEL:
					EndDialog(hDlg, true);
					break;

				case IDC_SEARCHDLL:
					LocateInterface(hDlg);
					break;
			}
			return 0;

		case WM_DESTROY:
			return 0;
	}
	return 0;
};

// ------------------------------------------------------------
// void AddObject(HWND hWindow)
// ------------------------------------------------------------
// Description
//  Adds an object to the world
//
// Parameters
//  hWindow		- Parent window
//
// Returns
//  Nothing
// ------------------------------------------------------------
void AddObject(HWND hWindow)
{
	OPENFILENAME ofn;

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
	ofn.lpstrTitle 		= "Add object to world";
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		SelObject = World->AddObject(szFile, "<Undefined>");
		DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_OBJ), hWindow, (DLGPROC)EditObjectDlgProc);
	}
};