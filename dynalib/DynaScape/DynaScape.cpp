// ------------------------------------------------------------
// Dynamic Landscape Interactive 3D Routine Handler
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include	<Windows.h>
#include <Math.h>
#include "World.h"
#include "DynaLib\DynaScape\Exports.h"

#define RAD(z) ((float)(z))*.0174
int	GridWidth, s1=0, s=0, d1=0;

// ------------------------------------------------------------
// extern "C" DLL_EXPORT void WINAPI Create(HWND hWnd, WORLD *World)
// ------------------------------------------------------------
// Description
//
// Parameters
//
// Returns
//  If sucessful ? 1 : 0
// ------------------------------------------------------------
extern "C" DLL_EXPORT void WINAPI Create(HWND hWnd, WORLD *World)
{
	int i;

	GridWidth = sqrt(World->Object->NumVertices);

	for(i = 0; i < World->Object->NumVertices; i++)
	{
			World->Object->Vertex[i].xObject = 0;
			World->Object->Vertex[i].yObject = 0;
			World->Object->Vertex[i].zObject = 0;
	}
	for(i = 0; i < World->Object->NumPolygons; i++)
	{
		World->Object->Polygon[i].Vertex = new VERTEX*[4];
		World->Object->Polygon[i].Type = POLY_LAMBERT;
		World->Object->Polygon[i].Color.rgbRed = 128;
	}


};

// ------------------------------------------------------------
// extern "C" DLL_EXPORT void WINAPI Animate(HWND hWnd, WORLD *World)
// ------------------------------------------------------------
// Description
//
// Parameters
//
// Returns
//  If sucessful ? 1 : 0
// ------------------------------------------------------------
extern "C" DLL_EXPORT void WINAPI Animate(HWND hWnd, WORLD *World)
{
	int	g, h, d;

// s controls one sine wave, d the other
// start s at an external counter value (X sine wave position)
	s = s1;
	s1 += GridWidth;

// loop through all points	for(g = 0; g < GridWidth; ++g)
	{
		d = d1++;

		for(h = 0; h < GridWidth; ++h)		{
			int	Index;

			Index = g * GridWidth + h;

			// start point3d at standard place
			World->Object->Vertex[Index].yObject = (GridWidth-h*3);
			// add in wave offsets, 6 defines wave amplitudes
			World->Object->Vertex[Index].yObject += 6 * sin(RAD((s%360)));
			World->Object->Vertex[Index].yObject += 6 * sin(RAD((d%360)));

			// changing these values will change wave frequency
			s += 1;
			d += GridWidth;
		}
	}
};
