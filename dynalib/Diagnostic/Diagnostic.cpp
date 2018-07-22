// ------------------------------------------------------------
// Standard 3D Immersion Interface DynaLib
// Copyright 1997 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\Object.h"
#include "Include\World.h"
#include "Include\Interface.h"

// ------------------------------------------------------------
// long InterfaceProc(WORLD *World, unsigned int Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Interface point between the DLL and immersion program
//
// Parameters
//  World		- World
//  Message		- Message code
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Long value
// ------------------------------------------------------------
long __stdcall InterfaceProc(WORLD *World, unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case IF_CREATE:
			MessageBox(GetFocus(), "Create", "Actung!", MB_OK);
			break;

//		case IF_UPDATE:
//			MessageBox(GetFocus(), "Update", "Actung!", MB_OK);
//			break;

		case IF_TIMER:
			break;

		case IF_KEYDOWN:
			MessageBox(GetFocus(), "KeyDown", "Actung!", MB_OK);
			break;

		case IF_KEYUP:
			MessageBox(GetFocus(), "KeyUp", "Actung!", MB_OK);
			break;

		case IF_MOUSEDOWN:
			MessageBox(GetFocus(), "MouseDown", "Actung!", MB_OK);
			break;

//		case IF_MOUSEMOVE:
//			MessageBox(GetFocus(), "MouseMove", "Actung!", MB_OK);
//			break;

		case IF_MOUSEUP:
			MessageBox(GetFocus(), "MouseUp", "Actung!", MB_OK);
			break;

		case IF_DESTROY:
			MessageBox(GetFocus(), "Destroy", "Actung!", MB_OK);
			break;

//		default:
//			DefInterfaceProc(World, Message, wParam, lParam);
//			break;
	}
};

// ------------------------------------------------------------
// long InterfaceProc(WORLD *World, unsigned int Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Interface point between the DLL and immersion program
//
// Parameters
//  World		- World
//  Message		- Message code
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Long value
// ------------------------------------------------------------
long __stdcall ObjectProc(WORLD *World, OBJECT *Object, unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case OBJ_CREATE:
			MessageBox(GetFocus(), "Create", "Actung!", MB_OK);
			break;

//		case OBJ_ANIMATE:
//			MessageBox(GetFocus(), "Animate", "Actung!", MB_OK);
//			break;

		case OBJ_MOUSEDOWN:
			MessageBox(GetFocus(), "MouseDown", "Actung!", MB_OK);
			break;

//		case OBJ_MOUSEMOVE:
//			MessageBox(GetFocus(), "MouseMove", "Actung!", MB_OK);
//			break;

		case OBJ_MOUSEUP:
			MessageBox(GetFocus(), "MouseUp", "Actung!", MB_OK);
			break;

		case OBJ_DESTROY:
			MessageBox(GetFocus(), "Destroy", "Actung!", MB_OK);
			break;
	}
};