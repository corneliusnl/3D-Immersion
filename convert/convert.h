#define STRICT

#include <string.h>
#include <stdlib.h>

struct WinApp
{
	static HINSTANCE 	hInstance;
   static HINSTANCE 	hPrevInst;
	static int			cmdShow;
};

long far pascal _export WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL _export FAR PASCAL AboutDlgBoxProc(HWND, UINT, WPARAM, LPARAM);
