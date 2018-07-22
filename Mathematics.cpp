// ------------------------------------------------------------
// Mathematic Routines
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Math.h>
#include <Stdio.h>
#include "Include\Mathematics.h"

//
// Variables
//
float	SinTable[DEGREECOUNT];
float CosTable[DEGREECOUNT];

// ------------------------------------------------------------
// void InitTrigTables()
// ------------------------------------------------------------
// Descripiton
//  Initialiazes the trigonometric functions sin and cos lookup
// tables (LUT).
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void InitTrigTables()
{
	float Unit = (float)(2*M_PI)/(float)DEGREECOUNT;

	for(int i = 0; i < DEGREECOUNT; i++)
	{
		SinTable[i] = sin(Unit * i);
		CosTable[i] = cos(Unit * i);
/*		char Buff[50];
		sprintf(Buff, "%f", SIN(i));
		MessageBox(GetFocus(), Buff, "Actung 1!", MB_OK);*/
	}
};

// ------------------------------------------------------------
// void Normalize(VECTOR *Vect)
// ------------------------------------------------------------
// Description
//  Normalizes the given vector.
//
// Parameters
//  Vect			- Vector to be normalized
//
// Returns
//  Nothing
// ------------------------------------------------------------
void Normalize(VECTOR *Vect)
{
	Vect->Length = sqrt(Vect->xComp * Vect->xComp
							+ Vect->yComp * Vect->yComp
							+ Vect->zComp * Vect->zComp);

	Vect->xComp = (Vect->xComp / Vect->Length);
	Vect->yComp = (Vect->yComp / Vect->Length);
	Vect->zComp = (Vect->zComp / Vect->Length);
};

// ------------------------------------------------------------
// float DotProduct(VECTOR *Vect1, VECTOR *Vect2)
// ------------------------------------------------------------
// Description
//  Obtains the dot product of two vectors.
//
// Parameters
//  Vect1			- Vector 1
//  Vect2			- Vector 2
//
// Returns
//  Dot product
// ------------------------------------------------------------
float DotProduct(VECTOR *Vect1, VECTOR *Vect2)
{
	return 	Vect1->xComp * Vect2->xComp +
				Vect1->yComp * Vect2->yComp +
				Vect1->zComp * Vect2->zComp;
};

// ------------------------------------------------------------
// VECTOR CrossProduct(VECTOR *Vect1, VECTOR *Vect2)
// ------------------------------------------------------------
// Description
//  Obtains the cross product of two vectors
//
// Parameters
//  Vect1		- Vector 1
//  Vect2		- Vector 2
//
// Returns
//  The resultant vector
// ------------------------------------------------------------
VECTOR CrossProduct(VECTOR *Vect1, VECTOR *Vect2)
{
	VECTOR RetVect;

	RetVect.xComp = Vect1->yComp * Vect2->zComp - Vect1->zComp * Vect2->yComp;
	RetVect.yComp = Vect1->zComp * Vect2->xComp - Vect1->xComp * Vect2->zComp;
	RetVect.zComp = Vect1->xComp * Vect2->yComp - Vect1->yComp * Vect2->xComp;

	return RetVect;
};
