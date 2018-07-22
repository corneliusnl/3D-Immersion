// ------------------------------------------------------------
// Boat Object Procedures
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\Object.h"
#include "Include\Interface.h"
#include "Boat\Boat.h"

//
// Variables
//
bool	Keybrd[8];

//
// Function Prototypes
//
void	HandleKeyboard(WORLD *World, WPARAM wParam, bool Down);
void	CameraTransition(WORLD *World, OBJECT *Object);

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
	OBJECT *Boat;

	Boat = World->FindObject("Row boat");

	if(Keybrd[UP])
	{
		World->Camera->zTranslate += 100 * ((float)World->DeltaTime/1000);
		Boat->zTranslate += 100 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[DOWN])
	{
		World->Camera->zTranslate -= 100 * ((float)World->DeltaTime/1000);
		Boat->zTranslate -= 100 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[LEFT])
	{
		World->Camera->Yaw -= 0.526 * ((float)World->DeltaTime/1000);
		Boat->yRotate -= 0.526 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[RIGHT])
	{
		World->Camera->Yaw += 0.526 * ((float)World->DeltaTime/1000);
		Boat->yRotate += 0.526 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[LOOKUP])
	{
		World->Camera->Pitch -= 0.526 * ((float)World->DeltaTime/1000);
	}
	if(Keybrd[LOOKDOWN])
	{
		World->Camera->Pitch += 0.526 * ((float)World->DeltaTime/1000);
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
			MessageBox(GetFocus(), "You now have control of the boat and can now row around... that's if you like that sort of thing :)", "Actung!", MB_OK);
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
			MessageBox(GetFocus(), "By clicking on the boat you can take control of the boat and row around the island.", "Actung!", MB_OK);
			break;

		case OBJ_ANIMATE:
			break;

		case OBJ_MOUSEDOWN:
			break;

		case OBJ_MOUSEMOVE:
			break;

		case OBJ_MOUSEUP:
			CameraTransition(World, Object);
			break;

		case OBJ_DESTROY:
			break;
	}
};

// ------------------------------------------------------------
// void CameraTransition(WORLD *World, OBJECT *Object)
// ------------------------------------------------------------
// Description
//  Takes the current camera and translates it until it
// is is equal to the boat camera.
//
// Parameters
//  World		- Pointer to world
//  Object		- Pointer to object
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CameraTransition(WORLD *World, OBJECT *Object)
{
	CAMERA	*NewCam;

	NewCam = World->FindCamera("Boat Camera");
	if(NewCam != NULL)
	{
		World->SwitchToCamera(NewCam);
		World->SetInterface("boat.dll");
	}
};
