#ifndef GCONTEXT_GCONTEXT_H
#define GCONTEXT_GCONTEXT_H
// ------------------------------------------------------------
// Graphics Library
//
// Copyright 1997 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>

// ------------------------------------------------------------
// Defines
// ------------------------------------------------------------
#define	DALIGN(a)		((a + 3)& ~3)

// ------------------------------------------------------------
// GRAPHICS_CONTEXT class
// ------------------------------------------------------------
class GCONTEXT
{
private:
	HBITMAP	hBitmap;
	HBITMAP	hOldBmp;

public:
	HPALETTE		hPalette;
	BITMAPINFO	*Info;
	void far		*Surface;
	HDC			GDC;

	// Class Initilization/Destruction
	GCONTEXT();
	GCONTEXT(int Width, int Height);
	~GCONTEXT();

	// Graphic Device
	void	CreateGDC(int Width, int Height);
	void	DeleteGDC();

	// OffScreen Clearing
	int	Clear(COLORREF Color);

	// Screen Bltting
	int	BltToScreen(HDC Screen, int X, int Y);
	int	StretchToScreen(HDC Screen, int XDest, int YDest, int DestW, int DestH,
				int SrcX, int SrcY, int SrcW, int SrcH);

	// Palette Manipulation
	int		LoadPalette(char *Filename);
	HPALETTE	CreateIdentityPalette(RGBQUAD *Color);

	// Inline Functions
	inline int			GetWidth()		{ return Info->bmiHeader.biWidth; }
	inline int			GetHeight()		{ return -Info->bmiHeader.biHeight; }
	inline int			StorageWidth()	{ return (Info->bmiHeader.biWidth + 3)& ~3; }
	inline HPALETTE	GetPalette()	{ return hPalette; }
};

#endif
