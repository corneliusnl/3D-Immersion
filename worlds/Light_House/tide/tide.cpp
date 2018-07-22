// ------------------------------------------------------------
// Tide Object Procedures
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\Object.h"
#include "Include\Interface.h"

// ------------------------------------------------------------
// long __stdcall ObjectProc(WORLD *World, OBJECT *Object, unsigned int Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Object interface to the immersion program
//
// Parameters
//
// Returns
//  Long value
// ------------------------------------------------------------
long __stdcall ObjectProc(WORLD *World, OBJECT *Object, unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case OBJ_CREATE:
			break;

		case OBJ_ANIMATE:
			if(Object->yTranslate < 60)
				Object->yTranslate += 0.2 * ((float)World->DeltaTime/1000);
			else
				Object->yTranslate -= 0.1 * ((float)World->DeltaTime/1000);
			break;

		case OBJ_MOUSEDOWN:
			break;

		case OBJ_MOUSEMOVE:
			break;

		case OBJ_MOUSEUP:
			break;

		case OBJ_DESTROY:
			break;
	}
};
