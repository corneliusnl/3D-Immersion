#ifndef IMMERSION_TYPES_H
#define IMMERSION_TYPES_H
// ------------------------------------------------------------
// Types
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>

//
// Vector structure
//
typedef struct
{
	float xComp, yComp, zComp;
	float Length;

} VECTOR;

//
// Vertex Structure
//
typedef struct
{
	float	xObject, yObject, zObject;
	float	xWorld, yWorld, zWorld;
	float	xCamera, yCamera, zCamera;

	int	xScreen, yScreen;

} VERTEX;

//
// Viewport Structure
//
typedef struct
{
	int	xMin;
	int	xMax;
	int	yMin;
	int	yMax;
	int	xCenter;
	int	yCenter;
} VIEWPORT;

#endif
