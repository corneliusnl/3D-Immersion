// ------------------------------------------------------------
// 3D Immersion
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
// 1998 Canada Wide Science Fair Entry
// Timmins, Ontario
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\WinApp.h"
#include "Include\CommonCtrls.h"
#include "Include\World.h"
#include "Include\Multimedia.h"
#include "Include\Immersion.rh"
#include "Include\Immersion.h"

//
// Varibles
//
WINAPP		WinApp;
HMENU			hMainMenu;
HWND        MainWnd;
HWND			RenderWnd;
HWND			TabWnd;

HDC			Screen;
WORLD 		*World = NULL;
AVIVIDEO		*Video;
TABINDEX		*InfoTab;
bool			Modified = false;

//
// Function Prototpes
//
void	LoadWorld();
void	SaveWorldAs();
void	CloseWorld();
void	CreateChildren(HWND hWnd);
void	CreateEditBar();
void	DestroyEditBar();

// ------------------------------------------------------------
// int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR cmdString, int cmdShow)
// ------------------------------------------------------------
// Description
//  Windows entry point
//
// Parameters
//  hInstance		- Handle of instance
//  hPrevInst		- Handle of previous instance
//  cmdString		- Pointer to string
//  cmdShow			- How to display window
//
// Return
//  Exit status
// ------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR cmdString, int cmdShow)
{
	MSG         Msg;
	WNDCLASS    WndClass;

	WinApp.hInstance = hInstance;
	WinApp.hPrevInst = hPrevInst;
	WinApp.cmdShow   = cmdShow;

	InitCommonControls();
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
		WndClass.lpszClassName  = "IMMERSION_CLASS";

		RegisterClass(&WndClass);
	}

	hMainMenu = LoadMenu(WinApp.hInstance, MAKEINTRESOURCE(IDM_MAINMENU));
	MainWnd   = CreateWindow("IMMERSION_CLASS",
					"3D Immersion v1.0 - Leprechaun Software",
					WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
					CW_USEDEFAULT, CW_USEDEFAULT,
					848, 526,
					NULL, hMainMenu, hInstance, NULL);
	UpdateWindow(MainWnd);

	for(;;)
	{
		if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
		{
			if(Msg.message == WM_QUIT)
				break;
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else if(World)
		{
			World->Render();
			World->GContext->BltToScreen(Screen, 0, 0);
		}
		else
			WaitMessage();
	}
	return Msg.wParam;
};

// ------------------------------------------------------------
// LRESULT CALLBACK _export WndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam);
// ------------------------------------------------------------
// Description
//  Window procedure handler
//
// Parameters
//  hWindow		- Handle of the window
//  Message		- Message id
//  wParam		- Word Parameter
//  lParam		- Long Parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
LRESULT CALLBACK _export WndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LPMINMAXINFO	MinMax;
	RECT				Rect;

	switch(Message)
	{
		case WM_CREATE:
			CreateChildren(hWindow);
			return 0;

		case WM_SIZE:
			MoveWindow(RenderWnd, 0, 0, 640, 480, true);
			if(World)
			{
				MoveWindow(InfoTab->hWnd, 640, 0, 200, 480, true);
				MoveWindow(TabWnd, 644, 32, 192, 440, true);
			}
			return 0;

		case WM_GETMINMAXINFO:
			MinMax = (LPMINMAXINFO)lParam;
			if(World)
			{
				MinMax->ptMaxSize.x			= 848;
				MinMax->ptMaxSize.y			= 526;
				MinMax->ptMinTrackSize.x	= 848;
				MinMax->ptMinTrackSize.y	= 526;
				MinMax->ptMaxTrackSize.x	= 848;
				MinMax->ptMaxTrackSize.y	= 526;
			}
			else
			{
				MinMax->ptMaxSize.x			= 648;
				MinMax->ptMaxSize.y			= 526;
				MinMax->ptMinTrackSize.x	= 648;
				MinMax->ptMinTrackSize.y	= 526;
				MinMax->ptMaxTrackSize.x	= 648;
				MinMax->ptMaxTrackSize.y	= 526;
         }
			return 0;

		case WM_NOTIFY:
			switch(((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE:
					DestroyWindow(TabWnd);

					switch(InfoTab->GetCurSel())
					{
						case 0:
							TabWnd = CreateDialog(WinApp.hInstance, MAKEINTRESOURCE(IDD_WORLD), hWindow, (DLGPROC)WorldDlgProc);
							break;

						case 1:
							TabWnd = CreateDialog(WinApp.hInstance, MAKEINTRESOURCE(IDD_OBJECTS), hWindow, (DLGPROC)ObjectDlgProc);
							break;

						case 2:
							TabWnd = CreateDialog(WinApp.hInstance, MAKEINTRESOURCE(IDD_CAMERAS), hWindow, (DLGPROC)CameraDlgProc);
							break;

						case 3:
							TabWnd = CreateDialog(WinApp.hInstance, MAKEINTRESOURCE(IDD_LIGHTS), hWindow, (DLGPROC)LightDlgProc);
							break;
					}
					break;
			}
			return 0;

		case WM_KEYDOWN:
			if(World)
				World->SendMessage(IF_KEYDOWN, wParam, NULL);
			return 0;

		case WM_KEYUP:
			if(World)
				World->SendMessage(IF_KEYUP, wParam, NULL);
			return 0;

		case WM_COMMAND:
			switch(wParam)
			{
				case CM_SAVEWLDAS:
					SaveWorldAs();
					break;

				case CM_SAVEWLD:
					if(strcmp("<Undefined>", World->Filename) == NULL)
						SaveWorldAs();
					else
					{
						World->Save(World->Filename);
						Modified = false;
					}
					break;

				case CM_EXIT:
					DestroyWindow(hWindow);
					break;

				case CM_ABOUT:
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_ABOUT), hWindow, (DLGPROC)AboutDlgProc);
					break;

				case CM_NEWWLD:
					if(!World)
					{
						// Stop video
						Video->Stop();

						// Create blank template
						World = new WORLD(RenderWnd, 640, 480);
						SelectPalette(Screen, World->GContext->hPalette, false);
						RealizePalette(Screen);
						World->New();

						// Request information
						DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_EDIT_WLD), hWindow, (DLGPROC)EditWorldDlgProc);

						// Configure windows
						GetWindowRect(hWindow, &Rect);
						CreateEditBar();
						MoveWindow(hWindow, Rect.left, Rect.top, Rect.right+200, Rect.bottom+200, true);

						// Configure menu
						EnableMenuItem(hMainMenu, CM_NEWWLD, MF_GRAYED);
						EnableMenuItem(hMainMenu, CM_LOADWLD, MF_GRAYED);
						EnableMenuItem(hMainMenu, CM_CLOSEWLD, MF_ENABLED);
						EnableMenuItem(hMainMenu, CM_SAVEWLD, MF_GRAYED);
						EnableMenuItem(hMainMenu, CM_SAVEWLDAS, MF_ENABLED);
					}
					break;

				case CM_LOADWLD:
					if(!World)
						LoadWorld();
					break;

				case CM_CLOSEWLD:
					if(World)
					{
						if(Modified)
						{
							DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_SAVE), hWindow, (DLGPROC)SaveDlgProc);
						}
						else
						{
							CloseWorld();
						}
					}
					break;

				case CM_EDITOPTIONS:
					DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_OPTIONS), hWindow, (DLGPROC)OptionsDlgProc);
					break;
			}
			return 0;

		case WM_DESTROY:
			if(World)
			{
				delete World;
				DestroyEditBar();
			}
			DestroyWindow(RenderWnd);
			PostQuitMessage(0);
			return 0;

		default:
			return(DefWindowProc(hWindow, Message, wParam, lParam));
	}
};

// ------------------------------------------------------------
// void CreateChildren(HWND hWnd)
// ------------------------------------------------------------
// Description
//  Creates the main windows children
//
// Parameters
//  hWnd			- Handle of parent window
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CreateChildren(HWND hWnd)
{
	WNDCLASS		WndClass;

	WndClass.style          = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc    = RenderWndProc;
	WndClass.cbClsExtra     = 0;
	WndClass.cbWndExtra     = 0;
	WndClass.hInstance      = WinApp.hInstance;
	WndClass.hIcon          = LoadIcon(WinApp.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground  = (HBRUSH)(GetStockObject(LTGRAY_BRUSH));
	WndClass.lpszMenuName   = NULL;
	WndClass.lpszClassName  = "RenderWndClass";
	RegisterClass(&WndClass);

	RenderWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "RenderWndClass",
					NULL,
					WS_VISIBLE | WS_CHILD,
					CW_USEDEFAULT, CW_USEDEFAULT,
					CW_USEDEFAULT, CW_USEDEFAULT,
					hWnd, NULL, WinApp.hInstance, NULL);
};

// ------------------------------------------------------------
// void CreateEditBar()
// ------------------------------------------------------------
// Description
//  Creates the info windows's tabbed dialog control
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CreateEditBar()
{
	InfoTab = new TABINDEX(MainWnd, WinApp.hInstance, 0, 0, 0, 0);
	InfoTab->CreateImageList(3, 20, 20);
	InfoTab->ImgList->Add(WinApp.hInstance, MAKEINTRESOURCE(IDB_WORLD));
	InfoTab->ImgList->Add(WinApp.hInstance, MAKEINTRESOURCE(IDB_OBJECT));
	InfoTab->ImgList->Add(WinApp.hInstance, MAKEINTRESOURCE(IDB_CAMERA));
	InfoTab->ImgList->Add(WinApp.hInstance, MAKEINTRESOURCE(IDB_LIGHT));
	InfoTab->SetImageList();

	InfoTab->InsertItem(TCIF_IMAGE, 0, 0, "");
	InfoTab->InsertItem(TCIF_IMAGE, 1, 1, "");
	InfoTab->InsertItem(TCIF_IMAGE, 2, 2, "");
	InfoTab->InsertItem(TCIF_IMAGE, 3, 3, "");
	SendMessage(InfoTab->hWnd, TCM_SETPADDING, 0, MAKELPARAM(-9, 4));
	TabWnd = CreateDialog(WinApp.hInstance, MAKEINTRESOURCE(IDD_WORLD), MainWnd, (DLGPROC)WorldDlgProc);
};

// ------------------------------------------------------------
// void DestroyEditBar()
// ------------------------------------------------------------
// Description
//  Destroys the info windows's tabbed dialog control
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void DestroyEditBar()
{
	DestroyWindow(TabWnd);
	delete InfoTab;
};

// ------------------------------------------------------------
// void LoadWorld()
// ------------------------------------------------------------
// Description
//  Loads a world
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LoadWorld()
{
	OPENFILENAME ofn;
	RECT	Rect;
	char 	szFile[80], szDirName[80];
	char	szFileTitle[16];
	char	szFilter[80] = "World file (*.wld)\0*.wld\0";


	szFile[0] = '\0';
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize 		= sizeof(OPENFILENAME);
	ofn.hwndOwner 			= MainWnd;
	ofn.lpstrFilter 		= szFilter;
	ofn.nFilterIndex 		= 0;
	ofn.lpstrFile			= szFile;
	ofn.nMaxFile 			= sizeof(szFile);
	ofn.lpstrFileTitle 	= szFileTitle;
	ofn.nMaxFileTitle 	= sizeof(szFileTitle);
	ofn.lpstrInitialDir 	= szDirName;
	ofn.lpstrTitle 		= "Load world";
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		// Stop video
		Video->Stop();

		// Load world
		World = new WORLD(RenderWnd, 640, 480);
		SelectPalette(Screen, World->GContext->hPalette, false);
		RealizePalette(Screen);
		World->Load(szFile);

		// Configure window
		GetWindowRect(MainWnd, &Rect);
		CreateEditBar();
		MoveWindow(MainWnd, Rect.left, Rect.top, Rect.right+200, Rect.bottom+200, true);
		EnableMenuItem(hMainMenu, CM_NEWWLD, MF_GRAYED);
		EnableMenuItem(hMainMenu, CM_LOADWLD, MF_GRAYED);
		EnableMenuItem(hMainMenu, CM_CLOSEWLD, MF_ENABLED);
		EnableMenuItem(hMainMenu, CM_SAVEWLD, MF_ENABLED);
		EnableMenuItem(hMainMenu, CM_SAVEWLDAS, MF_ENABLED);
	}
};

// ------------------------------------------------------------
// void SaveWorldAs()
// ------------------------------------------------------------
// Description
//  Saves a world as specified by user
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void SaveWorldAs()
{
	OPENFILENAME ofn;
	char 	szFile[80], szDirName[80];
	char	szFileTitle[16];
	char	szFilter[80] = "World file (*.wld)\0*.wld\0";


	szFile[0] = '\0';
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize 		= sizeof(OPENFILENAME);
	ofn.hwndOwner 			= MainWnd;
	ofn.lpstrFilter 		= szFilter;
	ofn.nFilterIndex 		= 0;
	ofn.lpstrFile			= szFile;
	ofn.nMaxFile 			= sizeof(szFile);
	ofn.lpstrFileTitle 	= szFileTitle;
	ofn.nMaxFileTitle 	= sizeof(szFileTitle);
	ofn.lpstrInitialDir 	= szDirName;
	ofn.lpstrTitle 		= "Save world as";
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST;

	if(GetSaveFileName(&ofn))
	{
		World->Save(szFile);
		strcpy(World->Filename, szFile);
		Modified = false;
	}
};

// ------------------------------------------------------------
// void CloseWorld()
// ------------------------------------------------------------
// Description
//  Closes a world
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CloseWorld()
{
	RECT	Rect;

	// Close world
	delete World;
	World = NULL;

	// Configure Windows
	DestroyEditBar();
	GetWindowRect(MainWnd, &Rect);
	MoveWindow(MainWnd, Rect.left, Rect.top, Rect.right-200, Rect.bottom-200, true);

	// Configure menu
	EnableMenuItem(hMainMenu, CM_NEWWLD, MF_ENABLED);
	EnableMenuItem(hMainMenu, CM_LOADWLD, MF_ENABLED);
	EnableMenuItem(hMainMenu, CM_CLOSEWLD, MF_GRAYED);
	EnableMenuItem(hMainMenu, CM_SAVEWLD, MF_GRAYED);
	EnableMenuItem(hMainMenu, CM_SAVEWLDAS, MF_GRAYED);

	// Play background animation
	Video->Play();
};

// ------------------------------------------------------------
// void LocateInterface(HWND hWindow)
// ------------------------------------------------------------
// Description
//  Prompts for the user to locate the desired interface library
//
// Parameters
//  hWindow		- Parent window
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LocateInterface(HWND hWindow)
{
	OPENFILENAME ofn;
	char 	szFile[80], szDirName[80];
	char	szFileTitle[16];
	char	szFilter[80] = "Interface Library (*.dll)\0*.dll\0";

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
	ofn.lpstrTitle 		= "Locate interface library";
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		SetDlgItemText(hWindow, IDC_DLL, szFile);
	}
};

// ------------------------------------------------------------
// LRESULT CALLBACK _export RenderWndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam);
// ------------------------------------------------------------
// Description
//  Window procedure handler
//
// Parameters
//  hWindow		- Handle of the window
//  Message		- Message id
//  wParam		- Word Parameter
//  lParam		- Long Parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
LRESULT CALLBACK _export RenderWndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT	ps;
	HMENU		hPopup, hMenu;
	char		Buff[256];

	switch(Message)
	{
		case WM_CREATE:
			Screen = GetDC(hWindow);
			Video = new AVIVIDEO(hWindow, "Immersion.avi");
			Video->Play();
			return 0;

		case WM_PAINT:
			BeginPaint(hWindow, &ps);
			if(World)
			{
				World->GContext->BltToScreen(Screen, 0, 0);
			}
			EndPaint(hWindow, &ps);
			return 0;

		case MM_MCINOTIFY:
			if(World)
				World->MciNotify(wParam);
			return 0;

		case WM_LBUTTONDOWN:
			if(World)
				World->MouseDown(LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_LBUTTONUP:
			if(World)
				World->MouseUp(LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_MOUSEMOVE:
			if(World)
				World->MouseMove(LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_DESTROY:
			ReleaseDC(hWindow, Screen);
			Video->Stop();
			delete Video;
			return 0;

		default:
			return(DefWindowProc(hWindow, Message, wParam, lParam));
	}
};

// ------------------------------------------------------------
// int _export FAR PASCAL AboutDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
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
int _export FAR PASCAL AboutDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
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

// ------------------------------------------------------------
// int _export FAR PASCAL SaveDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Save dialog box procedure handler
//
// Parameters
//  hDlg			- Handle of dialog
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
int _export FAR PASCAL SaveDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_YES:
					SaveWorldAs();
					EndDialog(hDlg, true);
					break;

				case IDC_NO:
					EndDialog(hDlg, true);
					CloseWorld();
					break;

				case IDC_CANCEL:
					EndDialog(hDlg, true);
					break;
			}
			return 0;
	}
	return 0;
};

// ------------------------------------------------------------
// int _export FAR PASCAL OptionsDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Options dialog box procedure handler
//
// Parameters
//  hDlg			- Handle of dialog
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Status code
// ------------------------------------------------------------
int _export FAR PASCAL OptionsDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	HKEY	hKey;
   DWORD	dwDataSize;
	DWORD	X, Y, Z;

	switch(Message)
	{
		case WM_INITDIALOG:
         RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Leprechaun Software\\3D Immersion", 0, KEY_READ, &hKey);

         // Snap X
			RegQueryValueEx(hKey, "Snap X", NULL, NULL, NULL, &dwDataSize);
			RegQueryValueEx(hKey, "Snap X", NULL, NULL, (LPBYTE)&X, &dwDataSize);
         SetDlgItemInt(hDlg, IDC_SNAPX, X, false);

			// Snap Y
			RegQueryValueEx(hKey, "Snap Y", NULL, NULL, NULL, &dwDataSize);
			RegQueryValueEx(hKey, "Snap Y", NULL, NULL, (LPBYTE)&Y, &dwDataSize);
         SetDlgItemInt(hDlg, IDC_SNAPY, Y, false);

			// Snap Z
			RegQueryValueEx(hKey, "Snap Z", NULL, NULL, NULL, &dwDataSize);
			RegQueryValueEx(hKey, "Snap Z", NULL, NULL, (LPBYTE)&Z, &dwDataSize);
         SetDlgItemInt(hDlg, IDC_SNAPZ, Z, false);

		   RegCloseKey(hKey);
			break;

		case WM_COMMAND:
			switch(wParam)
			{
				case IDC_OK:
		         RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Leprechaun Software\\3D Immersion", 0, KEY_WRITE, &hKey);

					// Snap X
               X = GetDlgItemInt(hDlg, IDC_SNAPX, NULL, false);
					RegSetValueEx(hKey, "Snap X", NULL, REG_DWORD, (LPBYTE)&X, sizeof(DWORD));

					// Snap Y
					Y = GetDlgItemInt(hDlg, IDC_SNAPY, NULL, false);
					RegSetValueEx(hKey, "Snap Y", NULL, REG_DWORD, (LPBYTE)&Y, sizeof(DWORD));

					// Snap Z
					Z = GetDlgItemInt(hDlg, IDC_SNAPZ, NULL, false);
					RegSetValueEx(hKey, "Snap Z", NULL, REG_DWORD, (LPBYTE)&Z, sizeof(DWORD));

				   RegCloseKey(hKey);
					EndDialog(hDlg, true);
					break;

            case IDC_CANCEL:
            	EndDialog(hDlg, true);
               break;
			}
			return 0;
	}
	return 0;
};
