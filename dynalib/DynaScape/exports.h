#ifndef IMMERSION_STANDARD_H
#define IMMERSION_STANDARD_H
// ------------------------------------------------------------
// Standard Dynamic Interactive 3D Routine Handler
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include "Object.h"

#define DLL_EXPORT	__declspec(dllexport)

extern "C"
{
	DLL_EXPORT void WINAPI Animate(HWND hWnd, WORLD *World);
	DLL_EXPORT void WINAPI MouseMove(HWND hWnd, WORLD *World);
	DLL_EXPORT void WINAPI MousePress(HWND hWnd, WORLD *World);
};

#endif
