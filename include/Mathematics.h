#ifndef IMMERSION_MATHEMATICS_H
#define IMMERSION_MATHEMATICS_H
// ------------------------------------------------------------
// Mathematic Routines
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Math.h>
#include "Include\Types.h"

#define	DEGREECOUNT		1024
#define	SIN(a)	(SinTable[a & (DEGREECOUNT-1)])
#define	COS(a)	(CosTable[a & (DEGREECOUNT-1)])
extern float	SinTable[DEGREECOUNT];
extern float	CosTable[DEGREECOUNT];

//
// Function prototypes
//
void		InitTrigTables();
void		Normalize(VECTOR *Vect);
float		DotProduct(VECTOR *Vect1, VECTOR *Vect2);
VECTOR	CrossProduct(VECTOR *Vect1, VECTOR *Vect2);

#endif