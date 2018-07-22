#ifndef IMMERSION_DIB_H
#define IMMERSION_DIB_H
// ------------------------------------------------------------
// DIB
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\GContext.h"

//
// DIB Class
//
class DIB
{
public:
	BITMAPINFOHEADER	Info;
	RGBQUAD	*Palette;
	HGLOBAL	hBits;

	// Construction/Destruction
	DIB();
	DIB(char *Filename);
	~DIB();

	// Retrieval
	void	Load(char *Filename);
	void	Free();

	// Bltting
	void	BitBlt(GCONTEXT *GContext, int XDest, int YDest);
	void	TransBlt(GCONTEXT *GContext, int XDest, int YDest, unsigned char TransColor);

	inline int	GetWidth()			{ return Info.biWidth; }
	inline int	GetHeight()			{ return Info.biHeight; }
};

#endif
