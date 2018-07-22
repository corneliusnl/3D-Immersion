// ------------------------------------------------------------
// Camera Routines
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <String.h>
#include "Include\Camera.h"

// ------------------------------------------------------------
// CAMERA::CAMERA()
// ------------------------------------------------------------
// Description
//  Class construction handler
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
CAMERA::CAMERA()
{
	Matrix	= new MATRIX();
	Next		= NULL;

	// Set Defaults
	strcpy(Identifier, "<Undefined>");
	xTranslate			= 0;
	yTranslate			= 0;
	zTranslate			= 0;
	Pitch					= 0;
	Yaw					= 0;
	Roll					= 0;
	ProjectionPlane	= 1000;
	Hither				= 10;
	Yon					= 10000;
};

// ------------------------------------------------------------
// void CAMERA::SetHither(long Hither)
// ------------------------------------------------------------
// Description
//  Sets the hither projection plane distance
//
// Parameters
//  Hither		- Hither distance
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CAMERA::SetHither(long Hither)
{
	this->Hither = Hither;
};

// ------------------------------------------------------------
// void CAMERA::SetYon(long Yon)
// ------------------------------------------------------------
// Description
//  Sets the yon projection plane distance
//
// Parameters
//  Yon			- Yon distance
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CAMERA::SetYon(long Yon)
{
	this->Yon	= Yon;
};

// ------------------------------------------------------------
// void CAMERA::SetProjectionPlane(long ProjectionPlane)
// ------------------------------------------------------------
// Description
//  Sets the camera's projection plane
//
// Parameters
//  ProjectionPlane	- The distance of the projection plane
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CAMERA::SetProjectionPlane(long ProjectionPlane)
{
	this->ProjectionPlane = ProjectionPlane;
};

// ------------------------------------------------------------
// void CAMERA::Translate(float x, float y, float z)
// ------------------------------------------------------------
// Description
//  Translate the camera along the x, y, and z axis
//
// Parameters
//  x				- X component
//  y				- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CAMERA::Translate(float x, float y, float z)
{
	xTranslate	= x;
   yTranslate	= y;
   zTranslate	= z;
};

// ------------------------------------------------------------
// void CAMERA::Rotate(float x, float y, float z)
// ------------------------------------------------------------
// Description
//  Rotates the camera along the x, y, and z axis
//
// Parameters
//  x				- X component
//  y				- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CAMERA::Rotate(float x, float y, float z)
{
	Pitch	= x;
   Yaw	= y;
   Roll	= z;
};

// ------------------------------------------------------------
// void CAMERA::SetAlias(char *Alias)
// ------------------------------------------------------------
// Description
//  Sets the camera's alias
//
// Parameters
//  Alias		- Camera identifier
//
// Returns
//  Nothing
// ------------------------------------------------------------
void CAMERA::SetAlias(char *Alias)
{
	strcpy(Identifier, Alias);
};

// ------------------------------------------------------------
// CAMERA::~CAMERA()
// ------------------------------------------------------------
// Description
//  Class destruction handler
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
CAMERA::~CAMERA()
{
	if(Next)
		delete	Next;
	delete	Matrix;
};
