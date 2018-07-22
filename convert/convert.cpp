// ------------------------------------------------------------
// Conversion v2.0
// Copyright 1998 (c) Leprechaun Software.  All Rigthts Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>
#include <StdIO.h>
#include <String.h>
#include "Defines.h"
#include "WinApp.h"
#include "Palette.h"

//
// Dialog Procedure Handlers
//
int _export FAR PASCAL OptionsDlgProc(HWND, UINT, WPARAM, LPARAM);

//
// Variables
//
WINAPP						WinApp;
HMENU                   hMenuInit;

//
// Function Prototypes
//
int ConvertPlgTo3D(char *, int Multiplier, int Divisor);
int Convert3DToPlg(char *);
int LocateFile(HWND);

//
// Common Dialog Variables
//
char 	szFile[80], szDirName[80];
char	szFileTitle[16];
char	szFilter[80] = "PLG File (*.PLG)\0*.PLG\0 Object DescriptionFile (*.odf)\0*.odf\0";

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
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR nCmdLine, int cmdShow)
{
	DLGPROC		DlgProc;

	WinApp.hInstance = hInstance;
	WinApp.hPrevInst = hPrevInst;
	WinApp.cmdShow   = cmdShow;

	InitCommonControls();

	DlgProc = (DLGPROC)MakeProcInstance(OptionsDlgProc, hInst);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_PROMPTDLG), NULL, DlgProc);
	FreeProcInstance((FARPROC)DlgProc);
	return 0;
};

// ------------------------------------------------------------
// int FAR PASCAL OptionsDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//
// Parameters
//  hDlg				- Handle of dialog
//  Message			- Message code
//  wParam			- Word parameter
//  lParam			- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
int FAR PASCAL OptionsDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	HWND	DlgItem;
	char	ConvertFile[MAX_PATH];
	int	Multiplier, Divisor;

	switch(Message)
	{
		case WM_INITDIALOG:
			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_CANCEL:
					EndDialog(hDlg, TRUE);
					return 0;

				case IDC_OK:
					GetDlgItemText(hDlg, IDC_FILE, ConvertFile, sizeof(ConvertFile));
					Multiplier	= GetDlgItemInt(hDlg, IDC_MULTIPLIER, NULL, true);
					Divisor		= GetDlgItemInt(hDlg, IDC_DIVISOR, NULL, true);
					ConvertPlgTo3D(ConvertFile, Multiplier, Divisor);
					return 0;

				case IDC_BROWSE:
					LocateFile(hDlg);
					return 0;
			}
			return 0;

		case WM_CLOSE:
			EndDialog(hDlg, TRUE);
			return 0;
	}
	return 0;
};

// ------------------------------------------------------------
// int ConvertPlgToODF(char *File, int Multiplier, int Divisor)
// ------------------------------------------------------------
// Description
//  Converts a PLG file to ODF
//
// Parameters
//  File			- Filename
//  Multiplier	- Vertex coordinate multiplier
//  Divisor		- Vertex coordinate divisor
//
// Returns
//  Status code
// ------------------------------------------------------------
int ConvertPlgTo3D(char *File, int Multiplier, int Divisor)
{
	FILE	*InPlg, *Out3D;
	float	Dist;
	int	NumVert, NumPoly;
	int	NumPoints, Point;
	int	Color;
	float	X, Y, Z;
	int	i, Count;
	char	OutFile[MAX_PATH];
	char	Buff[300], Tmp[300];
	char	Name[100], MsgBuf[300];
	char	*P;

	// Open Output/Input Files
	InPlg = fopen(File, "rt");

	// If Error Opening File
	if(!InPlg)
	{
		wsprintf(Tmp, "Could Not Open File: %s", File);
		MessageBox(GetFocus(), Tmp, "Error", MB_OK);
		return -1;
	}

Start:
	//
	// Read PLG File Info
	//
	while(fgets(Buff, sizeof(Buff), InPlg))
	{
		//
		// Convert comment
		//
		if(P = strchr(Buff, '#'))
		{
			wsprintf(Tmp, ";%s\n", P+1);
			*P = '\0';
		}

		//
		// Convert object definition
		//
		if(sscanf(Buff, "%s %d %d", Name, &NumVert, &NumPoly) == 3)
		{
			sprintf(OutFile, "%s.odf", Name);
			Out3D = fopen(OutFile, "wt");

			fputs("; Converted By Conversion V2.0\n", Out3D);
			fputs("; Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.\n\n", Out3D);

			wsprintf(Tmp, "Object(%d)\n", NumVert);
			fputs(Tmp, Out3D);
			goto Continue;
		}
	}
	goto End;

Continue:
	// Start Object Definition
	fputs("{\n", Out3D);

	//
	// Read PLG Object Vertices
	//
	for(i = 0; i < NumVert; i++)
	{
		while(1)
		{
			fgets(Buff, sizeof(Buff), InPlg);
			MsgBuf[0] = '\0';

			//
			// Convert comments
			//
			if(P = strchr(Buff, '#'))
			{
				*P = '\0';
				P++;
				wsprintf(Tmp, "%s", P);
				if(P = strchr(Tmp, '\n'))
					*P = ' ';
				wsprintf(MsgBuf, "\t; %s", Tmp);
			}

			//
			// Convert vector definition
			//
			if(sscanf(Buff, "%f %f %f", &X, &Y, &Z) == 3)
			{
				X = (X / Divisor) * Multiplier;
				Y = (Y / Divisor) * Multiplier;
				Z = (Z / Divisor) * Multiplier;

				wsprintf(Tmp, "\tAddVertex(%d,%d,%d)%s\n", (int)X, (int)Y, (int)Z, MsgBuf);
				fputs(Tmp, Out3D);
				break;
			}
			else
				continue;
		}
	}
	// Advance To Newline In Destination
	fputs("\n", Out3D);

	//
	// Convert polygons
	//
	for(i = 0; i < NumPoly; i++)
	{
		while(1)
		{
			fgets(Buff, sizeof(Buff), InPlg);

			//
			// Convert comment
			//
			if(P = strchr(Buff, '#'))
				*P = '\0';
			Color = (int)strtoul(Buff, &P, 0) & 255;
			NumPoints = (int)strtoul(P, &P, 0);

			//
			// Convert polygon points
			//
			for(int j = 0; j < NumPoints; j++)
			{
				// If First Polygon Point
				if(j == 0)
				{
					wsprintf(Tmp, "\tAddLambertPolygon(%d,", NumPoints);
					fputs(Tmp, Out3D);
				}
				Point = strtoul(P, &P, 0);

				// Not Last Point
				if(j < NumPoints-1)
					wsprintf(Tmp, "%d,", Point);
				else
					wsprintf(Tmp, "%d)\n", Point);
				fputs(Tmp, Out3D);

				// Last Point
				if(j == NumPoints-1)
				{
					fputs("\t{\n", Out3D);
					wsprintf(Tmp, "\t\tSetColor(%d,%d,%d)\n", Palette[Color].Red, Palette[Color].Green, Palette[Color].Blue);
					fputs(Tmp, Out3D);
					fputs("\t}\n", Out3D);
				}
			}
			P = strtok(Buff, " \t");
			if(P == NULL || P[0] == 0 || P[0] == '\n')
				continue;
			else break;
		}
	}

	fputs("}", Out3D);
	fclose(Out3D);

	goto Start;

End:
	fclose(InPlg);
};

// ------------------------------------------------------------
// int LocateFile(HWND hWindow)
// ------------------------------------------------------------
// Description
//  Display dialog box to allow to locate file
//
// Parameter
//  hWnd			- Handle of parent window
//
// Returns
//  Status code
// ------------------------------------------------------------
int LocateFile(HWND hWnd)
{
	OPENFILENAME ofn;

	szFile[0] = '\0';
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize 		= sizeof(OPENFILENAME);
	ofn.hwndOwner 			= hWnd;
	ofn.lpstrFilter 		= szFilter;
	ofn.nFilterIndex 		= 0;
	ofn.lpstrFile			= szFile;
	ofn.nMaxFile 			= sizeof(szFile);
	ofn.lpstrFileTitle 	= szFileTitle;
	ofn.nMaxFileTitle 	= sizeof(szFileTitle);
	ofn.lpstrInitialDir 	= szDirName;
	ofn.lpstrTitle 		= "Object Definition File";
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		SetDlgItemText(hWnd, IDC_FILE, szFile);
		EnableWindow(GetDlgItem(hWnd, IDC_OK), true);
		return 1;
	}
	return 0;
};
