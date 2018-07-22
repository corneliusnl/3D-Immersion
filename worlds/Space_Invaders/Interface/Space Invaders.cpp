// ------------------------------------------------------------
// Space Invaders '98 : Our Second Contact
// Interface Library for 3D Immersion v1.0
// Jeremiah Cornelious McCarthy, 16, Level II
//
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\GContext.h"
#include "Include\World.h"
#include "Include\Object.h"
#include "Include\Interface.h"
#include "Interface\Space Invaders.h"

//
// Variables
//
ALIEN	Invaders[6][3];
DIB	*Backdrop, *Cockpit;
float	CameraTilt;
bool	Keybrd[3];

//
// Function prototypes
//
void	Update(WORLD *World);
void	HandleKeyboard(WORLD *World, WPARAM wParam, bool Down);

// ------------------------------------------------------------
// void HandleKeyboard(WPARAM wParam, bool Down)
// ------------------------------------------------------------
// Description
//  Handles the world updating
//
// Parameters
//  wParam		- Scan code
//  Down			- Key press status
//
// Returns
//  Nothing
// ------------------------------------------------------------
void HandleKeyboard(WORLD *World, WPARAM wParam, bool Down)
{
	switch(wParam)
	{
		case VK_LEFT:	Keybrd[LEFT]		= Down;	 	break;
		case VK_RIGHT:	Keybrd[RIGHT]		= Down;		break;
	}
};

// ------------------------------------------------------------
// void Update(WORLD *World)
// ------------------------------------------------------------
// Description
//  Handles the world updating
//
// Parameters
//  World		- World
//
// Returns
//  Nothing
// ------------------------------------------------------------
void Update(WORLD *World)
{
	float tspeed;

	tspeed = 0.6 * ((float)World->DeltaTime/1000);
	if(Keybrd[LEFT])
		if(CameraTilt > -0.50)
			CameraTilt -= tspeed;
	if(Keybrd[RIGHT])
		if(CameraTilt < 0.50)
			CameraTilt += tspeed;

	if(CameraTilt >= 0.50)
		World->Camera->xTranslate += 10;
	else if(CameraTilt <= -0.50)
		World->Camera->xTranslate -= 10;

	tspeed = 0.2 * ((float)World->DeltaTime/1000);
	if((!Keybrd[LEFT]) && (!Keybrd[RIGHT]))
	{
		if(CameraTilt < -0.0001)
		{
			CameraTilt += tspeed;
			if(CameraTilt > -0.0001)
				CameraTilt = 0;
		}
		else if(CameraTilt > 0.0001)
		{
			CameraTilt -= tspeed;
			if(CameraTilt < 0.0001)
				CameraTilt = 0;
		}
	}
	World->Camera->Roll = CameraTilt;

	World->DrawObjects();
};

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
	switch(Message)
	{
		case IF_CREATE:
			Cockpit	= new DIB("Cockpit.bmp");
			Backdrop	= new DIB("Space.bmp");
			break;

		case IF_KEYDOWN:
			HandleKeyboard(World, wParam, true);
			break;

		case IF_KEYUP:
			HandleKeyboard(World, wParam, false);
			break;

		case IF_UPDATE:
			Backdrop->BitBlt(World->GContext, 0, 0);
			Update(World);
			Cockpit->TransBlt(World->GContext, 0, 0, 0);
			break;

		case IF_DESTROY:
			delete Cockpit;
			delete Backdrop;
			break;
	}
};
