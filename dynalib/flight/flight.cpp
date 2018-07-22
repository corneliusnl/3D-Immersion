// ------------------------------------------------------------
// Standard 3D Immersion Interface DynaLib
// Copyright 1997 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\Object.h"
#include "Include\World.h"
#include "DynaLib\Flight\Flight.h"
#include "Include\Interface.h"

//
// Variables
//
DIB	*Cockpit;
bool	Keybrd[8];
float	CameraTilt;
long	Speed;

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
		case VK_UP:			Keybrd[UP]			= Down;	break;
		case VK_DOWN:		Keybrd[DOWN]		= Down;	break;
		case VK_LEFT:		Keybrd[LEFT]		= Down; 	break;
		case VK_RIGHT:		Keybrd[RIGHT]		= Down;	break;
		case VK_PRIOR:		Keybrd[LOOKUP]		= Down;	break;
		case VK_NEXT:		Keybrd[LOOKDOWN]	= Down;	break;
		case VK_ADD:		Keybrd[SPEEDUP]	= Down;	break;
		case VK_SUBTRACT:	Keybrd[SPEEDDOWN]	= Down;	break;
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
	float newp, tspeed;
/*	float	sinPitch, cosPitch;
	float	sinYaw, cosYaw;
	float	sinRoll, cosRoll;

	sinPitch	= sin(World->Camera->Pitch);
	cosPitch	= cos(World->Camera->Pitch);
	sinYaw	= sin(World->Camera->Yaw);
	cosYaw	= cos(World->Camera->Yaw);
	sinRoll	= sin(World->Camera->Roll);
	cosRoll	= cos(World->Camera->Roll);*/

	tspeed = 0.0006 * World->DeltaTime;
	if(Keybrd[LEFT])
	{
		World->Camera->Yaw += tspeed;
		if(CameraTilt < 0.50)
			CameraTilt += tspeed;
	}
	if(Keybrd[RIGHT])
	{
		World->Camera->Yaw -= tspeed;
		if(CameraTilt > -0.50)
			CameraTilt -= tspeed;
	}

	if(Keybrd[UP])
		World->Camera->Pitch -= tspeed;

	if(Keybrd[DOWN])
		World->Camera->Pitch += tspeed;

	tspeed = 0.0002 * World->DeltaTime;
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

	if(Keybrd[SPEEDUP])
	{
		Speed += 2;
		if(Speed > 106)
			Speed = 106;
	}
	if(Keybrd[SPEEDDOWN])
	{
		Speed -= 2;
		if(Speed < 0)
			Speed = 0;
	}

	tspeed = (float)(World->DeltaTime / 24.0) * (float)Speed;
	World->Camera->yTranslate -= tspeed * sin(World->Camera->Pitch);

	newp = tspeed * sin(World->Camera->Yaw) * cos(World->Camera->Pitch);
	World->Camera->zTranslate += newp;

	newp = tspeed * sin(World->Camera->Yaw) * cos(World->Camera->Pitch);
	World->Camera->xTranslate += newp;

	World->DrawObjects();
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
long __stdcall InterfaceProc(WORLD *World, unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case IF_CREATE:
			Cockpit = new DIB("Cockpit.bmp");
			break;

		case IF_UPDATE:
			Update(World);
			Cockpit->TransBlt(World->GContext, 0, 0, 0);
			break;

		case IF_TIMER:
			break;

		case IF_KEYDOWN:
			HandleKeyboard(World, wParam, true);
			break;

		case IF_KEYUP:
			HandleKeyboard(World, wParam, false);
			break;

		case IF_MOUSEDOWN:
			break;

		case IF_MOUSEMOVE:
			break;

		case IF_MOUSEUP:
			break;

		case IF_DESTROY:
			delete Cockpit;
			break;

//		default:
//			DefInterfaceProc(World, Message, wParam, lParam);
//			break;
	}
};

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
			break;

		case OBJ_MOUSEDOWN:
			break;

		case OBJ_MOUSEMOVE:
			Object->xRotate += 0.02;
			Object->yRotate += 0.02;
			Object->zRotate += 0.02;
			break;

		case OBJ_MOUSEUP:
			break;

		case OBJ_DESTROY:
			break;
	}
};
