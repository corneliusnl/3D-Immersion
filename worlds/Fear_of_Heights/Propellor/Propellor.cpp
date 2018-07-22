// ------------------------------------------------------------
// Propellor Object
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
			Object->LinearVelocity.xComp += (Object->LinearAcceleration.xComp * ((float)World->DeltaTime/1000));
			Object->LinearVelocity.yComp += (Object->LinearAcceleration.yComp * ((float)World->DeltaTime/1000));
			Object->LinearVelocity.zComp += (Object->LinearAcceleration.zComp * ((float)World->DeltaTime/1000));

			Object->xTranslate += (Object->LinearVelocity.xComp * ((float)World->DeltaTime/1000));
			Object->yTranslate += (Object->LinearVelocity.yComp * ((float)World->DeltaTime/1000));
			Object->zTranslate += (Object->LinearVelocity.zComp * ((float)World->DeltaTime/1000));

			Object->RotationalVelocity.xComp += (Object->RotationalAcceleration.xComp * ((float)World->DeltaTime/1000));
			Object->RotationalVelocity.yComp += (Object->RotationalAcceleration.yComp * ((float)World->DeltaTime/1000));
			Object->RotationalVelocity.zComp += (Object->RotationalAcceleration.zComp * ((float)World->DeltaTime/1000));

			Object->xRotate += (Object->RotationalVelocity.xComp * ((float)World->DeltaTime/1000));
			Object->yRotate += (Object->RotationalVelocity.yComp * ((float)World->DeltaTime/1000));
			Object->zRotate += (Object->RotationalVelocity.zComp * ((float)World->DeltaTime/1000));
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
