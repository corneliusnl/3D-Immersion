// ------------------------------------------------------------
// Standard 3D Immersion Interface DynaLib
// Copyright 1997 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\Object.h"
#include "Include\World.h"
#include "DynaLib\Standard\Standard.h"
#include "Include\Interface.h"

//
// Variables
//
bool	Keybrd[8];

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
		case VK_HOME:		Keybrd[ROLLLEFT]	= Down;	break;
		case VK_END:		Keybrd[ROLLRIGHT]	= Down;	break;

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
	float	sinPitch, cosPitch;
	float	sinYaw, cosYaw;
	float	sinRoll, cosRoll;

	sinPitch	= sin(World->Camera->Pitch);
	cosPitch	= cos(World->Camera->Pitch);
	sinYaw	= sin(World->Camera->Yaw);
	cosYaw	= cos(World->Camera->Yaw);
	sinRoll	= sin(World->Camera->Roll);
	cosRoll	= cos(World->Camera->Roll);

	if(Keybrd[UP])
	{
		World->Camera->xTranslate += sinYaw * cosPitch * 100 * ((float)World->DeltaTime/1000);
		World->Camera->yTranslate -= sinPitch * 100 * ((float)World->DeltaTime/1000);
		World->Camera->zTranslate += cosYaw * cosPitch * 100 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[DOWN])
	{
		World->Camera->xTranslate -= sinYaw * cosPitch * 100 * ((float)World->DeltaTime/1000);
		World->Camera->yTranslate += sinPitch * 100 * ((float)World->DeltaTime/1000);
		World->Camera->zTranslate -= cosYaw * cosPitch * 100 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[LEFT])
	{
		World->Camera->Yaw -= 0.526 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[RIGHT])
	{
		World->Camera->Yaw += 0.526 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[LOOKUP])
	{
		World->Camera->Pitch -= 0.526 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[LOOKDOWN])
	{
		World->Camera->Pitch += 0.526 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[ROLLRIGHT])
	{
		World->Camera->Roll += 0.526 * ((float)World->DeltaTime / 1000);
	}
	if(Keybrd[ROLLLEFT])
	{
		World->Camera->Roll -= 0.526 * ((float)World->DeltaTime / 1000);
	}

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
			break;

		case IF_UPDATE:
			Update(World);
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
