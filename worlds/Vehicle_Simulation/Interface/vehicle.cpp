// ------------------------------------------------------------
// Vehicle interface
// Copyright 1997 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\Dib.h"
#include "Include\World.h"
#include "Include\Message.h"
#include "Interface\Interface.h"

//
// Variables
//
DIB	*Dib;

//
// Function prototypes
//

// ------------------------------------------------------------
// long __stdcall InterfaceProc(WORLD *World, unsigned int Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Object interface to the immersion program
//
// Parameters
//  World		- Handle of world
//  Message		- Message code
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Long value
// ------------------------------------------------------------
long __stdcall InterfaceProc(WORLD *World, unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	VECTOR	Temp, Accel;
	int		i;

	switch(Message)
	{
		case IF_CREATE:
			Dib	= new DIB("Dashboard.bmp");
			break;

		case IF_UPDATE:
			World->DrawHorizon();
			World->DrawObjects();
			Dib->BitBlt(World->GContext, 0, 340);
			break;

		case IF_DESTROY:
			delete Dib;
			break;
	}
};
