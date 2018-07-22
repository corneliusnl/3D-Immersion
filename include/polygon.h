#ifndef IMMERSION_POLYGON_H
#define IMMERSION_POLYGON_H
// ------------------------------------------------------------
// Polygon
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\Dib.h"
#include "Include\Types.h"
#include "Include\GContext.h"
#include "Include\Camera.h"
#include "Include\World.h"

class WORLD;

//
// Defines
//
#define UNDEFINED		-32000

//
// Type definitions
//
#define POLY_WIRE				0
#define POLY_SOLID			1
#define POLY_LAMBERT			2
#define POLY_TEXTURE			3
#define POLY_PHONG			4
#define POLY_GOURAUD			5
#define POLY_TRANSTEXTURE	6

typedef struct
{
	long	StartX;
	long	EndX;
	long	StartZ;
	long	EndZ;
	POINT	TextStart;
	POINT	TextEnd;

} SCANLINE;

// ------------------------------------------------------------
// Polygon class
// ------------------------------------------------------------
class POLYGON
{
public:
	POLYGON	*Prev, *Next;
	struct
	{
		VERTEX	*Vertex;
		POINT		*TextPt;
		int		NumVertices;
	} Clip;
	SCANLINE	*Scan;
	RGBQUAD	Color;
	VERTEX	**Vertex;
	VECTOR	Normal;
	WORLD		*World;
	POINT		*TextPt;
	BYTE		TransColor;
	DIB		*Texture;
	float		Illum;
	long		Distance;
	int 		NumVertices;
	int		Type;

	// Construction/Destruction
	POLYGON(WORLD *World, int NumVertices);
	~POLYGON();

	// Draw routines
	void	CalcScanLines();
	void	CalcIllumination();
	void	Draw();
	void	DrawWired();
	void	DrawSolid(RGBQUAD Color);
	void	DrawTextured();
	void	DrawTransTextured();

	// Polygon routines
	bool	ContainsPoint(int x, int y);
	long	CalcDistance();
	void	CalcNormal();
	bool	FacesCamera();
	void	Project();
	void	ClipZ();
	void	ClipXY();
};

#endif
