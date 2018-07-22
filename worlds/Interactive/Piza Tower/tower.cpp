// ------------------------------------------------------------
// Leaning Tower of Piza Interface
// Copyright 1997 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\Dib.h"
#include "Include\World.h"
#include "Include\Interface.h"
#include "Piza Tower\Tower.h"

//
// Defines
//
#define UP			0
#define DOWN		1
#define RIGHT		2
#define LEFT		3
#define LOOKUP		4
#define LOOKDOWN	5

//
// Variables
//
bool	Keybrd[8];
DIB	*Dib;
bool	Animate = false;

//
// Function prototypes
//
void	HandleKeyboard(WORLD *World, WPARAM wParam, bool Down);
void	Update(WORLD *World);

// ------------------------------------------------------------
// void HandleKeyboard(WORLD *World, WPARAM wParam, bool Down)
// ------------------------------------------------------------
// Description
//  Handles the world updating
//
// Parameters
//  World		- Handleof world
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
	OBJECT	*Obj;
	float	sinPitch, cosPitch;
	float	sinYaw, cosYaw;
	float	sinRoll, cosRoll;
	float	val;

	if(Animate)
	{
		Obj = World->FindObject("Piza Tower");
		val = 0.02 * ((float)World->DeltaTime/1000);
		Obj->xRotate += val;
		Obj->yRotate += val;
		Obj->zRotate += val;

		if(Obj->xRotate >= 0.12)
			Animate = false;
	}

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
			Animate = false;
			break;

		case IF_KEYDOWN:
			HandleKeyboard(World, wParam, true);
			break;

		case IF_KEYUP:
			HandleKeyboard(World, wParam, false);
			break;

		case IF_UPDATE:
			Update(World);
			break;

		case IF_DESTROY:
			break;
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
			break;

		case OBJ_MOUSEUP:
			World->RemoveObject(Object);
			Animate = true;
			break;

		case OBJ_DESTROY:
			break;
	}
};
