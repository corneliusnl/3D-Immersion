// ------------------------------------------------------------
// Light
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\Mathematics.h"
#include "Include\Light.h"

// ------------------------------------------------------------
// LIGHT::LIGHT(int Type)
// ------------------------------------------------------------
// Description
//  Light class construction
//
// Parameters
//  Type			- Type of light
//
// Returns
//  Class instance
// ------------------------------------------------------------
LIGHT::LIGHT(int Type)
{
	this->Type	= Type;
	Next			= NULL;

	// Set defaults
	strcpy(Identifier, "<Undefined>");
	Ambience				= 0;
	xTranslate			= 0;
	yTranslate			= 0;
	zTranslate			= 0;
	On						= true;
	Direction.xComp	= 0;
	Direction.yComp	= 0;
	Direction.zComp	= 0;
};

// ------------------------------------------------------------
// LIGHT::~LIGHT()
// ------------------------------------------------------------
// Description
//  Light class destruction
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
LIGHT::~LIGHT()
{
	if(Next)
		delete Next;
};

// ------------------------------------------------------------
// void LIGHT::SetAlias(char *Alias)
// ------------------------------------------------------------
// Description
//  Sets the alias of the light
//
// Parameters
//  Alias		- String of the light alias
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LIGHT::SetAlias(char *Alias)
{
	strcpy(Identifier, Alias);
};

// ------------------------------------------------------------
// void LIGHT::TurnedOn(bool On)
// ------------------------------------------------------------
// Description
//  Specifies whether or not the light of on or off
//
// Parameters
//  On			- Light state
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LIGHT::TurnedOn(bool On)
{
	this->On = On;
};

// ------------------------------------------------------------
// void LIGHT::SetAmbience(float Ambience)
// ------------------------------------------------------------
// Description
//  Specifies the lights ambience
//
// Parameters
//  Ambience	- Light ambience
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LIGHT::SetAmbience(float Ambience)
{
	this->Ambience = Ambience;
};

// ------------------------------------------------------------
// void LIGHT::SetDirection(float x, float y, float z)
// ------------------------------------------------------------
// Description
//  Sets the direction of the light rays
//
// Parameters
//  x				- The delta x
//  y				- The delta y
//  z				- The delta z
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LIGHT::SetDirection(float x, float y, float z)
{
	Direction.xComp = x;
	Direction.yComp = y;
	Direction.zComp = z;
};

// ------------------------------------------------------------
// void LIGHT::Translate(float x, float y, float z)
// ------------------------------------------------------------
// Description
//  Translates the light source
//
// Parameters
//  x				- The location along the x axis
//  y				- The location along the y axis
//  z				- The location along the z axis
//
// Returns
//  Nothing
// ------------------------------------------------------------
void LIGHT::Translate(float x, float y, float z)
{
/*	xTranslate	= x;
	yTranslate	= y;
	zTranslate	= z;*/
};

// ------------------------------------------------------------
// float LIGHT::CalcIllum(VECTOR *Normal)
// ------------------------------------------------------------
// Description
//  Calculates the illumination of an object
//
// Parameters
//  Normal		- Polygons normal
//
// Returns
//  Nothing
// ------------------------------------------------------------
float LIGHT::CalcIllum(VECTOR *Normal)
{
	float	Illum;
	long	Distance;

	switch(Type)
	{
		case LGT_INFINITE:
			if(On == false)
			{
				Illum = 0.0f;
				break;
			}

			Illum = DotProduct(Normal, &Direction);

			if(Illum < Ambience)
				Illum = Ambience;
			else if(Illum > 1.0)
				Illum = 1.0;
			break;

		case LGT_POINT:
/*			Distance = sqrt((xTranslate * xTranslate)	+
								(yTranslate * yTranslate)	+
								(zTranslate * zTranslate));

			Illum = DotProduct(Normal, &Direction) / Distance;*/
			break;
	}

	return Illum;
};
