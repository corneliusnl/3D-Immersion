#ifndef LIB_COMMONCTRLS_H
#define LIB_COMMONCTRLS_H
// ------------------------------------------------------------
// Common Controls Library
// ------------------------------------------------------------
#include <CommCtrl.h>
#include <Windows.h>

// ------------------------------------------------------------
// Image List Class
// ------------------------------------------------------------
class IMAGELIST
{
private:
public:
	HIMAGELIST	hWnd;

	// Class Construction/Destruction
	IMAGELIST(int Num, int Width, int Height);
	~IMAGELIST();

   // Modifiers
   int Add(HINSTANCE hInst, LPCTSTR Image);
};

// ------------------------------------------------------------
// Tool Tip Control Class
// ------------------------------------------------------------
class TOOLTIP
{
private:
public:
	HWND hWnd;

	// Class construction/destruction
	TOOLTIP(HINSTANCE hInst);
   ~TOOLTIP();

	// Modifiers
	void Add(HWND hWnd, HINSTANCE hInst, RECT Rect, LPSTR TipText);
};

// ------------------------------------------------------------
// Tree View Class
// ------------------------------------------------------------
class TREEVIEW
{
public:
	HIMAGELIST	ImgLst;
	HWND 			hWnd;

	// Create/Destroy TreeView
	TREEVIEW(HWND hParent, HINSTANCE hInst, int X, int Y, int Width,
		int Height, int WindowID);
	~TREEVIEW();

	// Tree Items
	void 			SetImageList(HIMAGELIST ImgLst);
	HTREEITEM 	AddListItem(HTREEITEM ParentItm, HTREEITEM InsAfter,
						LPSTR szText, int TreeImgID);

	// User Interaction
	int	DragBegin();
	int 	DrawMove();
	int 	DragEnd();
};

// ------------------------------------------------------------
// Tab Control Class
// ------------------------------------------------------------
class TABINDEX
{
private:
public:
	IMAGELIST	*ImgList;
	HWND			hWnd;

	// Create / Destroy Tab Index
	TABINDEX(HWND hWnd, HINSTANCE hInst, int X, int Y, int Width, int Height);
	~TABINDEX();

	// Methods
	void	CreateImageList(int NumImgs, int Width, int Height);
	void	SetImageList();
	int	InsertItem(UINT Type, int Image, int Loc, char *String);
	int	GetCurSel();
};

// ------------------------------------------------------------
// Property Sheet Class
// ------------------------------------------------------------
class PROPERTY
{
private:
public:
	// Create / Destory Property Sheet
	PROPERTY();
	~PROPERTY();

	// Property Sheet Manipulation
	PROPSHEETPAGE CreatePage(HINSTANCE hInst, LPCSTR Dialog, LPCSTR Icon, DLGPROC DlgProc);
	void CreateSheet(HWND hWnd, HINSTANCE hInst, PROPSHEETPAGE *Psp, LPCSTR Icon, LPCSTR Caption);
};

// ------------------------------------------------------------
// Progress Control Class
// ------------------------------------------------------------
class PROGRESS
{
private:
public:
	HWND hWnd;

	// Create/Destroy Progress Bar
	PROGRESS(HWND hParent, HINSTANCE hInst, int X, int Y, int Width,
		int Height, int WindowID);
   ~PROGRESS();

	// Progress Settings
   void SetRange(int Min, int Max);
   void SetStep(long Step);
   void SetPos(int Pos);
   void Increment(int Amount);
};

// ------------------------------------------------------------
// Trackbar Control Class
// ------------------------------------------------------------
class TRACKBAR
{
private:
public:
	HWND	hWnd;

	// Create/Destroy Track Bar
	TRACKBAR(HWND hParent, HINSTANCE hInst, DWORD WinStyle, int X, int Y,
		int Width, int Height, int WindowID);
	~TRACKBAR();

	// Settings
   void SetRange(int Min, int Max);
	void SetPos(int Pos);
   void TickFreq(int Freq);
};

// ------------------------------------------------------------
// Status Bar Control Class
// ------------------------------------------------------------
class STATUS
{
private:
public:
	HWND	hWnd;

	// Create/Destroy Status Window
	STATUS(HWND hParent, HINSTANCE hInst, int X, int Y, int Width,
				int Height, int WindowID);
	~STATUS();

   // Settings
   void SetText(LPSTR Text);
};

#endif
