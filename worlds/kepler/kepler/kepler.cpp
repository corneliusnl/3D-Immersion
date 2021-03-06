// ------------------------------------------------------------
// Keplers Law for Planetary Bodies
// Copyright 1997 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\Object.h"
#include "Include\Interface.h"

//
// Function prototypes
//
VECTOR	CalcForce(OBJECT *Obj1, OBJECT *Obj2);

// ------------------------------------------------------------
// long __stdcall ObjectProc(WORLD *World, OBJECT *Object, unsigned int Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Object interface to the immersion program
//
// Parameters
//  World		- Handle of world
//  Object		- Handle of object
//  Message		- Message code
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Long value
// ------------------------------------------------------------
long __stdcall ObjectProc(WORLD *World, OBJECT *Object, unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	VECTOR	Temp, Accel;
	int		i;

	switch(Message)
	{
		case OBJ_CREATE:
			break;

		case OBJ_ANIMATE:
			OBJECT *Obj;

			Temp.xComp = 0;
			Temp.yComp = 0;
			Temp.zComp = 0;

			Accel.xComp = 0;
			Accel.yComp = 0;
			Accel.zComp = 0;

			for(Obj = World->Object; Obj != NULL; Obj = Obj->Next)
			{
				if(Obj != Object)
					Temp = CalcForce(Object, Obj);

				Accel.xComp += Temp.xComp;
				Accel.yComp += Temp.yComp;
				Accel.zComp += Temp.zComp;
			}

			Object->LinearVelocity.xComp += (Object->LinearAcceleration.xComp + Accel.xComp * ((float)World->DeltaTime/1000));
			Object->LinearVelocity.yComp += (Object->LinearAcceleration.yComp + Accel.yComp * ((float)World->DeltaTime/1000));
			Object->LinearVelocity.zComp += (Object->LinearAcceleration.zComp + Accel.zComp * ((float)World->DeltaTime/1000));

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

// ------------------------------------------------------------
// VECTOR CalcForce(OBJECT *Obj1, OBJECT *Obj2)
// ------------------------------------------------------------
// Description
//  Calculates the kepler force between the two objects
//
// Parameters
//  Obj1			- Object 1
//  Obj2			- Object 2
//
// Returns
//  Resultant vector
// ------------------------------------------------------------
VECTOR CalcForce(OBJECT *Obj1, OBJECT *Obj2)
{
	float		G = 6.672 * pow10(-5);
	float		Acceleration = 0;
	VECTOR	Result;

	Result.xComp = Obj2->xTranslate - Obj1->xTranslate;
	Result.yComp = Obj2->yTranslate - Obj1->yTranslate;
	Result.zComp = Obj2->zTranslate - Obj1->zTranslate;

	Result.Length = sqrt((Result.xComp * Result.xComp) + (Result.yComp * Result.yComp) + (Result.zComp * Result.zComp));

	if(Result.Length != 0)
	{
		Acceleration = G * Obj2->Mass / (Result.Length * Result.Length);

		Result.xComp /= Result.Length;
		Result.yComp /= Result.Length;
		Result.zComp /= Result.Length;
	}
	else
	{
		Result.xComp = 0;
		Result.yComp = 0;
		Result.zComp = 0;
	}

	Result.xComp *= Acceleration;
	Result.yComp *= Acceleration;
	Result.zComp *= Acceleration;

	return Result;
};