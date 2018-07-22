// ------------------------------------------------------------
// Polygon
// Copyright 1997-1998 (c) Leprechaun Software. All Rights Reserved.
// ------------------------------------------------------------
#include "Include\Mathematics.h"
#include "Include\Polygon.h"
#include "Include\World.h"

// ------------------------------------------------------------
// POLYGON::POLYGON(int NumVertices)
// ------------------------------------------------------------
// Description
//  Class construction handler
//
// Parameters
//  NumVertices	- Number of vertices
//
// Returns
//  Class instance
// ------------------------------------------------------------
POLYGON::POLYGON(WORLD *World, int NumVertices)
{
	// Allocate memory for scan lines
	Scan		  	= new SCANLINE[World->GContext->GetHeight()];
	this->World	= World;

	// Allocate memory for polygon
	this->NumVertices	= NumVertices;
	Vertex				= new VERTEX*[NumVertices];
	TextPt				= new POINT[NumVertices];
	Texture				= NULL;
	Next					= NULL;

	// Allocate memory for clipped polygon
	Clip.NumVertices	= 0;
	Clip.Vertex			= new VERTEX[NumVertices+6];
	Clip.TextPt			= new POINT[NumVertices+6];
};

// ------------------------------------------------------------
// POLYGON::~POLYGON()
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
POLYGON::~POLYGON()
{
	// Free Allocated Scan Lines
	delete[]	Scan;

	// Free Allocated Clipped Polygons
	delete[] Clip.Vertex;
	delete[]	Clip.TextPt;
	delete[] Vertex;
	delete[]	TextPt;

	// Free Allocated Texture structures
	if(Texture)
	{
		Texture->Free();
		delete	Texture;
	}

	if(Next)
	{
		delete	Next;
		Next		= NULL;
	}
};

// ------------------------------------------------------------
// void POLYGON::Draw()
// ------------------------------------------------------------
// Description
//  Passes control to the polygon drawer routine specified by
// the type variable.
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::Draw()
{
	RGBQUAD	Clr;
	LIGHT		*Light;

	// Calculate the scanlines of the polygon
	CalcScanLines();
	CalcIllumination();

	switch(Type)
	{
		case POLY_WIRE:
			DrawWired();
			break;

		case POLY_SOLID:
			DrawSolid(Color);
			break;

		case POLY_LAMBERT:
			CalcNormal();
			Normalize(&Normal);

			Clr = Color;

			Clr.rgbRed		*= Illum;
			Clr.rgbGreen	*= Illum;
			Clr.rgbBlue		*= Illum;

			DrawSolid(Clr);
			break;

		case POLY_TEXTURE:
			DrawTextured();
			break;

		case POLY_TRANSTEXTURE:
			DrawTransTextured();
			break;

		default:
			break;
	}
};

// ------------------------------------------------------------
// void POLYGON::CalcScanLines()
// ------------------------------------------------------------
// Description
//  Calculates the scanlines of the given polygon
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::CalcIllumination()
{
	Illum = World->Ambience;
	for(Light = World->Light; Light != NULL; Light = Light->Next)
	{
		Illum += Light->CalcIllum(&Normal);
	}
};

// ------------------------------------------------------------
// void POLYGON::CalcScanLines()
// ------------------------------------------------------------
// Description
//  Calculates the scanlines of the given polygon
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::CalcScanLines()
{
	POINT	t1, t2, tp;
	long	th, tv, ths, tvs;
	long	x1, y1, z1, x2, y2, z2;
	long	x, xs, z, zs;
	long	tmp, y;
	int	i, j;

	for(i = 0; i < World->GContext->GetHeight(); i++)
	{
		Scan[i].StartX	= UNDEFINED;
		Scan[i].EndX	= UNDEFINED;
	}

	j = Clip.NumVertices - 1;
	for(i = 0; i < Clip.NumVertices; i++)
	{
		x1		= (long)Clip.Vertex[j].xScreen;
		y1		= (long)Clip.Vertex[j].yScreen;
		z1		= (long)Clip.Vertex[j].zCamera;
		x2		= (long)Clip.Vertex[i].xScreen;
		y2		= (long)Clip.Vertex[i].yScreen;
		z2		= (long)Clip.Vertex[i].zCamera;
		t1.x	= Clip.TextPt[j].x;
		t1.y	= Clip.TextPt[j].y;
		t2.x	= Clip.TextPt[i].x;
		t2.y	= Clip.TextPt[i].y;

		if(y2 != y1)
		{
			if(y2 < y1)
			{
				tmp 	= y2;
				y2		= y1;
				y1		= tmp;

				tmp	= x2;
				x2		= x1;
				x1		= tmp;

				tmp	= z2;
				z2		= z1;
				z1		= tmp;

				tp		= t2;
				t2 	= t1;
				t1 	= tp;
			}

			x		= (long)(x1 << 8);
			xs		= ((long)(x2 - x1) << 8) / (y2 - y1);
			z		= (long)(z1 << 8);
			zs		= ((long)(z2 - z1) << 8) / (y2 - y1);
			th		= (long)(t1.x << 8);
			tv		= (long)(t1.y << 8);
			ths	= ((long)(t2.x - t1.x) << 8) / (y2 - y1);
			tvs	= ((long)(t2.y - t1.y) << 8) / (y2 - y1);

			x	+= xs;
			z	+= zs;
			th	+= ths;
			tv	+= tvs;
			y1++;

			for(y = y1; y <= y2; y++)
			{
				if((y >= 0) && (y < World->GContext->GetHeight()))
				{
					if(Scan[y].StartX == UNDEFINED)
					{
						Scan[y].StartX			= x >> 8;
						Scan[y].StartZ			= z >> 8;
						Scan[y].TextStart.x	= th >> 8;
						Scan[y].TextStart.y	= tv >> 8;
					}
					else
					{
						Scan[y].EndX			= x >> 8;
						Scan[y].EndZ			= z >> 8;
						Scan[y].TextEnd.x		= th >> 8;
						Scan[y].TextEnd.y 	= tv >> 8;
					}
				}
				x += xs;
				z += zs;
				th += ths;
				tv += tvs;
			}
		}

		j = i;
	}
};

// ------------------------------------------------------------
// void POLYGON::DrawWired()
// ------------------------------------------------------------
// Description
//  Draws the given polygon
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::DrawWired()
{
};

// ------------------------------------------------------------
// void POLYGON::DrawSolid(RGBQUAD Clr)
// ------------------------------------------------------------
// Description
//  Draws the given polygon
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::DrawSolid(RGBQUAD Color)
{
	char 	*Dest;
	long	*Buff;
	long	z, zs;
	long 	tmp;
	BYTE 	Index;

	//
	// Draw Scanlines
	//
	Index	= GetNearestPaletteIndex(World->GContext->GetPalette(), RGB(Color.rgbRed, Color.rgbGreen, Color.rgbBlue));

	for(int i = 0; i < World->GContext->GetHeight(); i++)
	{
		if(Scan[i].StartX != UNDEFINED)
		{
			if(Scan[i].EndX == UNDEFINED)
			{
				Scan[i].EndX = Scan[i].StartX;
				Scan[i].EndZ = Scan[i].StartZ;
			}

			if(Scan[i].EndX < Scan[i].StartX)
			{
				tmp 			  	= Scan[i].EndX;
				Scan[i].EndX  	= Scan[i].StartX;
				Scan[i].StartX	= tmp;

				tmp				= Scan[i].EndZ;
				Scan[i].EndZ	= Scan[i].StartZ;
				Scan[i].StartZ	= tmp;
			}

			z = (Scan[i].StartZ << 8);
			if((Scan[i].EndX - Scan[i].StartX) == 0)
				zs = 0;
			else
				zs = ((Scan[i].EndZ - Scan[i].StartZ) << 8) / (Scan[i].EndX - Scan[i].StartX);

			Dest	= (char *)World->GContext->Surface + (i * World->GContext->GetWidth() + Scan[i].StartX);
			Buff	= (long *)World->zBuffer + (i * World->GContext->GetWidth() + Scan[i].StartX);

			for(int j = 0; j < ((Scan[i].EndX - Scan[i].StartX) >> 2); j++)
			{
				if(*Buff > (long)(z >> 8))
				{
					*Dest = Index;
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z += zs;

				if(*Buff > (long)(z >> 8))
				{
					*Dest = Index;
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z += zs;

				if(*Buff > (long)(z >> 8))
				{
					*Dest = Index;
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z += zs;

				if(*Buff > (long)(z >> 8))
				{
					*Dest = Index;
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z += zs;
			}

			for(int j = 0; j < ((Scan[i].EndX - Scan[i].StartX) & 0x03); j++)
			{
				if(*Buff > (long)(z >> 8))
				{
					*Dest = Index;
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z += zs;
			}
		}
	}
};

// ------------------------------------------------------------
// void POLYGON::DrawTextured()
// ------------------------------------------------------------
// Description
//  Draws the given polygon
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::DrawTextured()
{
	POINT	tp;
	char	*Dest, *Src;
	long	th, tv, ths, tvs;
	long	z, zs;
	long	tmp;
	int	i, j;
	long	*Buff;

	if(!Texture->hBits)
		return;

	//
	// Draw Scanlines
	//
	for(i = 0; i < World->GContext->GetHeight(); i++)
	{
		if(Scan[i].StartX != UNDEFINED)
		{
			if(Scan[i].EndX == UNDEFINED)
			{
				Scan[i].EndX		= Scan[i].StartX;
				Scan[i].EndZ		= Scan[i].StartZ;
				Scan[i].TextEnd	= Scan[i].TextStart;
			}

			if(Scan[i].EndX < Scan[i].StartX)
			{
				tmp					= Scan[i].EndX;
				Scan[i].EndX		= Scan[i].StartX;
				Scan[i].StartX		= tmp;

				tmp					= Scan[i].EndZ;
				Scan[i].EndZ		= Scan[i].StartZ;
				Scan[i].StartZ		= tmp;

				tp						= Scan[i].TextEnd;
				Scan[i].TextEnd	= Scan[i].TextStart;
				Scan[i].TextStart	= tp;
			}

			z 	= (long)(Scan[i].StartZ << 8);
			th	= (long)(Scan[i].TextStart.x << 8);
			tv	= (long)(Scan[i].TextStart.y << 8);

			if((Scan[i].EndX - Scan[i].StartX) == 0)
			{
				ths	= 0;
				tvs	= 0;
				zs		= 0;
			}
			else
			{
				ths	= ((Scan[i].TextEnd.x - Scan[i].TextStart.x) << 8) / (Scan[i].EndX - Scan[i].StartX);
				tvs	= ((Scan[i].TextEnd.y - Scan[i].TextStart.y) << 8) / (Scan[i].EndX - Scan[i].StartX);
				zs		= ((Scan[i].EndZ - Scan[i].StartZ) << 8) / (Scan[i].EndX - Scan[i].StartX);
			}

			Dest	= (char *)World->GContext->Surface + (i * World->GContext->GetWidth() + Scan[i].StartX);
			Buff	= (long *)World->zBuffer + (i * World->GContext->GetWidth() + Scan[i].StartX);
			Src	= (char *)Texture->hBits;

			for(j = 0; j < ((Scan[i].EndX - Scan[i].StartX) >> 2); j++)
			{
				if(*Buff > (long)(z >> 8))
				{
					*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z	+= zs;
				th	+= ths;
				tv	+= tvs;

				if(*Buff > (long)(z >> 8))
				{
					*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z	+= zs;
				th	+= ths;
				tv	+= tvs;

				if(*Buff > (long)(z >> 8))
				{
					*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z	+= zs;
				th	+= ths;
				tv	+= tvs;

				if(*Buff > (long)(z >> 8))
				{
					*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
					*Buff = (long)(z >> 8);
				}
				Dest++;
				Buff++;
				z	+= zs;
				th	+= ths;
				tv	+= tvs;
			}

			if((Scan[i].EndX - Scan[i].StartX) & 0x03)
			{
				for(j = 0; j < ((Scan[i].EndX - Scan[i].StartX) & 0x03); j++)
				{
					if(*Buff > (long)(z >> 8))
					{
						*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
						*Buff = (long)(z >> 8);
					}
					Dest++;
					Buff++;
					z	+= zs;
					th	+= ths;
					tv	+= tvs;
				}
			}
		}
	}
};

// ------------------------------------------------------------
// void POLYGON::DrawTransTextured()
// ------------------------------------------------------------
// Description
//  Draws the given polygon with a texture omitting the
// parts that are specified to be transparent
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::DrawTransTextured()
{
	POINT	tp;
	char	*Dest, *Src;
	long	th, tv, ths, tvs;
	long	z, zs;
	long	tmp;
	int	i, j;
	long	*Buff;

	if(!Texture->hBits)
		return;

	//
	// Draw Scanlines
	//
	for(i = 0; i < World->GContext->GetHeight(); i++)
	{
		if(Scan[i].StartX != UNDEFINED)
		{
			if(Scan[i].EndX == UNDEFINED)
			{
				Scan[i].EndX		= Scan[i].StartX;
				Scan[i].EndZ		= Scan[i].StartZ;
				Scan[i].TextEnd	= Scan[i].TextStart;
			}

			if(Scan[i].EndX < Scan[i].StartX)
			{
				tmp					= Scan[i].EndX;
				Scan[i].EndX		= Scan[i].StartX;
				Scan[i].StartX		= tmp;

				tmp					= Scan[i].EndZ;
				Scan[i].EndZ		= Scan[i].StartZ;
				Scan[i].StartZ		= tmp;

				tp						= Scan[i].TextEnd;
				Scan[i].TextEnd	= Scan[i].TextStart;
				Scan[i].TextStart	= tp;
			}

			z 	= (Scan[i].StartZ << 8);
			th	= (Scan[i].TextStart.x << 8);
			tv	= (Scan[i].TextStart.y << 8);

			if((Scan[i].EndX - Scan[i].StartX) == 0)
			{
				ths	= 0;
				tvs	= 0;
				zs		= 0;
			}
			else
			{
				ths	= ((Scan[i].TextEnd.x - Scan[i].TextStart.x) << 8) / (Scan[i].EndX - Scan[i].StartX);
				tvs	= ((Scan[i].TextEnd.y - Scan[i].TextStart.y) << 8) / (Scan[i].EndX - Scan[i].StartX);
				zs		= ((Scan[i].EndZ - Scan[i].StartZ) << 8) / (Scan[i].EndX - Scan[i].StartX);
			}

			Dest	= (char *)World->GContext->Surface + (i * World->GContext->GetWidth() + Scan[i].StartX);
			Buff	= (long *)World->zBuffer + (i * World->GContext->GetWidth() + Scan[i].StartX);
			Src	= (char *)Texture->hBits;

			for(j = 0; j < ((Scan[i].EndX - Scan[i].StartX) >> 2); j++)
			{
				if((z >> 8) < *Buff)
				{
					if((*(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8))) != TransColor)
					{
						*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
						*Buff = (long)(z >> 8);
					}
				}
				Dest++;
				Buff++;
				z	+= zs;
				th	+= ths;
				tv	+= tvs;

				if((z >> 8) < *Buff)
				{
					if((*(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8))) != TransColor)
					{
						*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
						*Buff = (long)(z >> 8);
					}
				}
				Dest++;
				Buff++;
				z	+= zs;
				th	+= ths;
				tv	+= tvs;

				if((z >> 8) < *Buff)
				{
					if((*(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8))) != TransColor)
					{
						*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
						*Buff = (long)(z >> 8);
					}
				}
				Dest++;
				Buff++;
				z	+= zs;
				th	+= ths;
				tv	+= tvs;

				if((z >> 8) < *Buff)
				{
					if((*(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8))) != TransColor)
					{
						*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
						*Buff = (long)(z >> 8);
					}
				}
				Dest++;
				Buff++;
				z	+= zs;
				th	+= ths;
				tv	+= tvs;
			}

			if((Scan[i].EndX - Scan[i].StartX) & 0x03)
			{
				for(j = 0; j < ((Scan[i].EndX - Scan[i].StartX) & 0x03); j++)
				{
					if((z >> 8) < *Buff)
					{
						if((*(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8))) != TransColor)
						{
							*Dest = *(Src + ((tv >> 8) * Texture->GetWidth()) + (th >> 8));
							*Buff = (long)(z >> 8);
						}
					}
					Dest++;
					Buff++;
					z	+= zs;
					th	+= ths;
					tv	+= tvs;
				}
			}
		}
	}
};

// ------------------------------------------------------------
// bool POLYGON::ContainsPoint(int x, int y)
// ------------------------------------------------------------
// Description
//  Determines whether the polygon contains the specified
// point.
//
// Parameters
//  x				- X coordinate of point
//  y				- Y coordinate of point
//
// Returns
//  Nothing
// ------------------------------------------------------------
bool POLYGON::ContainsPoint(int x, int y)
{
	if((x >= Scan[y].StartX) && (x <= Scan[y].EndX))
		return true;

	return false;
};

// ------------------------------------------------------------
// long POLYGON::CalcDistance()
// ------------------------------------------------------------
// Description
//  Finds the polygon distance by averaging the Z
// coordinate values of vertices.
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
long POLYGON::CalcDistance()
{
	Distance = 0L;
	for(int i = 0; i < NumVertices; i++)
		Distance += (long)Vertex[i]->zCamera;

	return(Distance /= NumVertices);
};

// ------------------------------------------------------------
// void POLYGON::CalcNormal()
// ------------------------------------------------------------
// Description
//  Calculates the polygon's normal.
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::CalcNormal()
{
	VECTOR Vect1, Vect2;

	Vect1.xComp = (Vertex[0]->xWorld - Vertex[1]->xWorld);
	Vect1.yComp = (Vertex[0]->yWorld - Vertex[1]->yWorld);
	Vect1.zComp = (Vertex[0]->zWorld - Vertex[1]->zWorld);

	Vect2.xComp = (Vertex[2]->xWorld - Vertex[1]->xWorld);
	Vect2.yComp = (Vertex[2]->yWorld - Vertex[1]->yWorld);
	Vect2.zComp = (Vertex[2]->zWorld - Vertex[1]->zWorld);

	Normal = CrossProduct(&Vect1, &Vect2);
};

// ------------------------------------------------------------
// bool POLYGON::BackfaceCull()
// ------------------------------------------------------------
// Description
//  Detects whether or not the polygon is facing the viewer.
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
bool POLYGON::FacesCamera()
{
	float X1, Y1, Z1;
	float X2, Y2, Z2;
	float X3, Y3, Z3;
	float D;

	if(NumVertices < 3)
		return 0;

	X1 = Vertex[0]->xCamera;
	Y1 = Vertex[0]->yCamera;
	Z1 = Vertex[0]->zCamera;

	X2 = Vertex[1]->xCamera;
	Y2 = Vertex[1]->yCamera;
	Z2 = Vertex[1]->zCamera;

	X3 = Vertex[2]->xCamera;
	Y3 = Vertex[2]->yCamera;
	Z3 = Vertex[2]->zCamera;

	D = (	X3 * ((Z1 * Y2) - (Y1 * Z2)) )
	  + ( Y3 * ((X1 * Z2) - (Z1 * X2)) )
	  + ( Z3 * ((Y1 * X2) - (X1 * Y2)) );

	return ((D < 0) ? true : false);
};

// ------------------------------------------------------------
// void POLYGON::Project()
// ------------------------------------------------------------
// Description
//  Projects the polygon onto the viewing plain
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::Project()
{
	float Val;

	for(int i = 0; i < Clip.NumVertices; i++)
	{
		Val = World->Camera->ProjectionPlane / Clip.Vertex[i].zCamera;
		Clip.Vertex[i].xScreen = Clip.Vertex[i].xCamera * Val + World->ViewPort.xCenter;
		Clip.Vertex[i].yScreen = Clip.Vertex[i].yCamera * -Val + World->ViewPort.yCenter;
	}
};

// ------------------------------------------------------------
// void POLYGON::ClipZ()
// ------------------------------------------------------------
// Description
//  Clips polygons relative to the camera's hither plane
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::ClipZ()
{
	float	m;
	int	v1, cp;

	// Initialize pointer to last vertex:
	v1 = NumVertices-1;
	cp = 0;

	// Loop through all edges of polygon
	for(int v2 = 0; v2 < NumVertices; v2++)
	{
		//
		// Entirely inside front view
		//
		if((Vertex[v1]->zCamera >=	World->Camera->Hither) &&
			(Vertex[v2]->zCamera >=	World->Camera->Hither))
		{
			Clip.Vertex[cp].xCamera	= Vertex[v2]->xCamera;
			Clip.Vertex[cp].yCamera	= Vertex[v2]->yCamera;
			Clip.Vertex[cp].zCamera	= Vertex[v2]->zCamera;
			Clip.TextPt[cp]			= TextPt[v2];
			cp++;
		}

		//
		// Edge is entirely past front
		//
		if((Vertex[v1]->zCamera <	World->Camera->Hither) &&
			(Vertex[v2]->zCamera <	World->Camera->Hither))
		{
		}

		//
		// Edge is leaving front view
		//
		if((Vertex[v1]->zCamera >= World->Camera->Hither) &&
			(Vertex[v2]->zCamera <	World->Camera->Hither))
		{
			m =	(float)(World->Camera->Hither - Vertex[v1]->zCamera) /
					(float)(Vertex[v2]->zCamera - Vertex[v1]->zCamera);

			Clip.Vertex[cp].xCamera	= Vertex[v1]->xCamera + (Vertex[v2]->xCamera - Vertex[v1]->xCamera) * m;
			Clip.Vertex[cp].yCamera	= Vertex[v1]->yCamera + (Vertex[v2]->yCamera - Vertex[v1]->yCamera) * m;
			Clip.Vertex[cp].zCamera	= World->Camera->Hither;
			Clip.TextPt[cp].x		  	= TextPt[v1].x + ((TextPt[v2].x - TextPt[v1].x) * m);
			Clip.TextPt[cp].y 	  	= TextPt[v1].y + ((TextPt[v2].y - TextPt[v1].y) * m);
			cp++;
		}

		//
		// Edge is entering front view
		//
		if((Vertex[v1]->zCamera <	World->Camera->Hither) &&
			(Vertex[v2]->zCamera >=	World->Camera->Hither))
		{
			m =	(float)(World->Camera->Hither - Vertex[v1]->zCamera) /
					(float)(Vertex[v2]->zCamera - Vertex[v1]->zCamera);

			Clip.Vertex[cp].xCamera	= Vertex[v1]->xCamera + (Vertex[v2]->xCamera - Vertex[v1]->xCamera) * m;
			Clip.Vertex[cp].yCamera	= Vertex[v1]->yCamera + (Vertex[v2]->yCamera - Vertex[v1]->yCamera) * m;
			Clip.Vertex[cp].zCamera	= World->Camera->Hither;
			Clip.TextPt[cp].x		  	= TextPt[v1].x + ((TextPt[v2].x - TextPt[v1].x) * m);
			Clip.TextPt[cp].y		  	= TextPt[v1].y + ((TextPt[v2].y - TextPt[v1].y) * m);
			cp++;

			Clip.Vertex[cp].xCamera	= Vertex[v2]->xCamera;
			Clip.Vertex[cp].yCamera	= Vertex[v2]->yCamera;
			Clip.Vertex[cp].zCamera	= Vertex[v2]->zCamera;
			Clip.TextPt[cp]			= TextPt[v2];
			cp++;
		}
		v1 = v2;
	}

	Clip.NumVertices = cp;
};

// ------------------------------------------------------------
// void POLYGON::ClipXY()
// ------------------------------------------------------------
// Description
//  Clips the polygons to the sides of the ViewPort
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void POLYGON::ClipXY()
{
	VERTEX	*TmpVert;
	POINT		*TmpText;
	float		DeltaX, DeltaY, DeltaZ;
	float		m;
	int		v1, v2, cp;

	TmpVert	= new VERTEX[NumVertices+6];
	TmpText	= new POINT[NumVertices+6];

	// ------------------------------------------------------------
	// Clip against left edge of ViewPort
	// ------------------------------------------------------------
	cp = 0;
	v1 = Clip.NumVertices-1;

	for(v2 = 0; v2 < Clip.NumVertices; v2++)
	{
		DeltaX = Clip.Vertex[v2].xScreen - Clip.Vertex[v1].xScreen;
		DeltaY = Clip.Vertex[v2].yScreen - Clip.Vertex[v1].yScreen;
		DeltaZ = Clip.Vertex[v2].zCamera - Clip.Vertex[v1].zCamera;

		//
		// Well within ViewPort
		//
		if((Clip.Vertex[v1].xScreen >=	World->ViewPort.xMin) &&
			(Clip.Vertex[v2].xScreen >=	World->ViewPort.xMin))
		{
			// Clip texture map
			TmpText[cp]				= Clip.TextPt[v2];

			// Clip polygon
			TmpVert[cp].xScreen 	= Clip.Vertex[v2].xScreen;
			TmpVert[cp].yScreen	= Clip.Vertex[v2].yScreen;
			TmpVert[cp].zCamera	= Clip.Vertex[v2].zCamera;
			cp++;
		}

		//
		// Edge is entirely out of ViewPort
		//
		if((Clip.Vertex[v1].xScreen <	World->ViewPort.xMin) &&
			(Clip.Vertex[v2].xScreen <	World->ViewPort.xMin))
		{
		}

		//
		// Edge is leaving ViewPort
		//
		if((Clip.Vertex[v1].xScreen >=	World->ViewPort.xMin) &&
			(Clip.Vertex[v2].xScreen <		World->ViewPort.xMin))
		{
			// Clip texture map
			m =	(float)(World->ViewPort.xMin - Clip.Vertex[v1].xScreen) /
					(float)(Clip.Vertex[v2].xScreen - Clip.Vertex[v1].xScreen);
			TmpText[cp].x			= Clip.TextPt[v1].x + ((Clip.TextPt[v2].x - Clip.TextPt[v1].x) * m);
			TmpText[cp].y			= Clip.TextPt[v1].y + ((Clip.TextPt[v2].y - Clip.TextPt[v1].y) * m);

			// Clip polygon
			if(DeltaX != 0)
			{
				m = (float)(DeltaY/DeltaX);
				TmpVert[cp].yScreen	= Clip.Vertex[v1].yScreen + ((World->ViewPort.xMin - Clip.Vertex[v1].xScreen) * m);
				m = (float)(DeltaZ/DeltaX);
				TmpVert[cp].zCamera	= Clip.Vertex[v1].zCamera + ((World->ViewPort.xMin - Clip.Vertex[v1].xScreen) * m);
			}
			else
			{
				TmpVert[cp].yScreen = Clip.Vertex[v1].yScreen;
				TmpVert[cp].zCamera = Clip.Vertex[v1].zCamera;
			}

			TmpVert[cp].xScreen	= World->ViewPort.xMin;
			cp++;
		}

		//
		// Edge is entering ViewPort
		//
		if((Clip.Vertex[v1].xScreen <		World->ViewPort.xMin) &&
			(Clip.Vertex[v2].xScreen >=	World->ViewPort.xMin))
		{
			// Clip polygon
			if(DeltaX != 0)
			{
				m = (float)(DeltaY/DeltaX);
				TmpVert[cp].yScreen	= Clip.Vertex[v1].yScreen + ((World->ViewPort.xMin - Clip.Vertex[v1].xScreen) * m);
				m = (float)(DeltaZ/DeltaX);
				TmpVert[cp].zCamera	= Clip.Vertex[v1].zCamera + ((World->ViewPort.xMin - Clip.Vertex[v1].xScreen) * m);
			}
			else
			{
				TmpVert[cp].yScreen = Clip.Vertex[v1].yScreen;
				TmpVert[cp].zCamera = Clip.Vertex[v1].zCamera;
			}
			TmpVert[cp].xScreen	= World->ViewPort.xMin;

			// Clip texture map
			m =	(float)(World->ViewPort.xMin - Clip.Vertex[v1].xScreen) /
					(float)(Clip.Vertex[v2].xScreen - Clip.Vertex[v1].xScreen);
			TmpText[cp].x			= Clip.TextPt[v1].x + ((Clip.TextPt[v2].x - Clip.TextPt[v1].x) * m);
			TmpText[cp].y			= Clip.TextPt[v1].y + ((Clip.TextPt[v2].y - Clip.TextPt[v1].y) * m);
			cp++;

			TmpVert[cp].xScreen	= Clip.Vertex[v2].xScreen;
			TmpVert[cp].yScreen	= Clip.Vertex[v2].yScreen;
			TmpVert[cp].zCamera	= Clip.Vertex[v2].zCamera;
			TmpText[cp]				= Clip.TextPt[v2];
			cp++;
		}
		v1 = v2;
	}
	Clip.NumVertices = cp;

	// ------------------------------------------------------------
	// Clip against right edge of ViewPort
	// ------------------------------------------------------------
	cp = 0;
	v1 = Clip.NumVertices-1;

	for(v2 = 0; v2 < Clip.NumVertices; v2++)
	{
		DeltaX = TmpVert[v2].xScreen - TmpVert[v1].xScreen;
		DeltaY = TmpVert[v2].yScreen - TmpVert[v1].yScreen;
		DeltaZ = TmpVert[v2].zCamera - TmpVert[v1].zCamera;

		//
		// Edge isn't off right side of ViewPort
		//
		if((TmpVert[v1].xScreen <=	World->ViewPort.xMax) &&
			(TmpVert[v2].xScreen <=	World->ViewPort.xMax))
		{
			// Clip texture map
			Clip.TextPt[cp]			= TmpText[v2];

			// Clip polygon
			Clip.Vertex[cp].xScreen	= TmpVert[v2].xScreen;
			Clip.Vertex[cp].yScreen	= TmpVert[v2].yScreen;
			Clip.Vertex[cp].zCamera = TmpVert[v2].zCamera;
			cp++;
		}

		//
		// Edge is entirely off right side of ViewPort
		//
		if((TmpVert[v1].xScreen > World->ViewPort.xMax) &&
			(TmpVert[v2].xScreen > World->ViewPort.xMax))
		{
		}

		//
		// Edge is leaving ViewPort
		//
		if((TmpVert[v1].xScreen <=	World->ViewPort.xMax) &&
			(TmpVert[v2].xScreen >	World->ViewPort.xMax))
		{
			// Clip texture map
			m =	(float)(World->ViewPort.xMax - TmpVert[v1].xScreen)/
					(float)(TmpVert[v2].xScreen - TmpVert[v1].xScreen);
			Clip.TextPt[cp].x			= TmpText[v1].x + ((TmpText[v2].x - TmpText[v1].x) * m);
			Clip.TextPt[cp].y			= TmpText[v1].y + ((TmpText[v2].y - TmpText[v1].y) * m);

			// Clip polygon
			if(DeltaX != 0)
			{
				m = (float)(DeltaY/DeltaX);
				Clip.Vertex[cp].yScreen	= TmpVert[v1].yScreen + ((World->ViewPort.xMax - TmpVert[v1].xScreen) * m);
				m = (float)(DeltaZ/DeltaX);
				Clip.Vertex[cp].zCamera = TmpVert[v1].zCamera + ((World->ViewPort.xMax - TmpVert[v1].xScreen) * m);
			}
			else
			{
				Clip.Vertex[cp].yScreen = TmpVert[v1].yScreen;
				Clip.Vertex[cp].zCamera = TmpVert[v1].zCamera;
			}
			Clip.Vertex[cp].xScreen	= World->ViewPort.xMax;
			cp++;
		}

		//
		// Edge is entering ViewPort
		//
		if((TmpVert[v1].xScreen >	World->ViewPort.xMax) &&
			(TmpVert[v2].xScreen <=	World->ViewPort.xMax))
		{
			// Clip texture map
			m =	(float)(World->ViewPort.xMax - TmpVert[v1].xScreen) /
					(float)(TmpVert[v2].xScreen - TmpVert[v1].xScreen);
			Clip.TextPt[cp].x			= TmpText[v1].x + ((TmpText[v2].x - TmpText[v1].x) * m);
			Clip.TextPt[cp].y			= TmpText[v1].y + ((TmpText[v2].y - TmpText[v1].y) * m);

			// Clip polygon
			if(DeltaX != 0)
			{
				m = (float)(DeltaY/DeltaX);
				Clip.Vertex[cp].yScreen	= TmpVert[v1].yScreen + ((World->ViewPort.xMax - TmpVert[v1].xScreen) * m);
				m = (float)(DeltaZ/DeltaX);
				Clip.Vertex[cp].zCamera = TmpVert[v1].zCamera + ((World->ViewPort.xMax - TmpVert[v1].xScreen) * m);
			}
			else
			{
				Clip.Vertex[cp].yScreen	= TmpVert[v1].yScreen;
				Clip.Vertex[cp].zCamera = TmpVert[v1].zCamera;
			}
			Clip.Vertex[cp].xScreen	= World->ViewPort.xMax;
			cp++;

			Clip.Vertex[cp].xScreen	= TmpVert[v2].xScreen;
			Clip.Vertex[cp].yScreen	= TmpVert[v2].yScreen;
			Clip.Vertex[cp].zCamera	= TmpVert[v2].zCamera;
			Clip.TextPt[cp]			= TmpText[v2];
			cp++;
		}
		v1 = v2;
	}
	Clip.NumVertices = cp;

	// ------------------------------------------------------------
	// Clip against top of ViewPort
	// ------------------------------------------------------------
	cp = 0;
	v1 = Clip.NumVertices-1;

	for(v2 = 0; v2 < Clip.NumVertices; v2++)
	{
		DeltaX = Clip.Vertex[v2].xScreen - Clip.Vertex[v1].xScreen;
		DeltaY = Clip.Vertex[v2].yScreen - Clip.Vertex[v1].yScreen;
		DeltaZ = Clip.Vertex[v2].zCamera - Clip.Vertex[v1].zCamera;

		//
		// Edge is not off top of ViewPort
		//
		if((Clip.Vertex[v1].yScreen >= World->ViewPort.yMin) &&
			(Clip.Vertex[v2].yScreen >= World->ViewPort.yMin))
		{
			// Clip texture map
			TmpText[cp]				= Clip.TextPt[v2];

			// Clip polygon
			TmpVert[cp].xScreen	= Clip.Vertex[v2].xScreen;
			TmpVert[cp].yScreen	= Clip.Vertex[v2].yScreen;
			TmpVert[cp].zCamera	= Clip.Vertex[v2].zCamera;
			cp++;
		}

		//
		// Edge is entirely off top of ViewPort
		//
		if((Clip.Vertex[v1].yScreen < World->ViewPort.yMin) &&
			(Clip.Vertex[v2].yScreen < World->ViewPort.yMin))
		{
		}

		//
		// Edge is leaving ViewPort
		//
		if((Clip.Vertex[v1].yScreen >=	World->ViewPort.yMin) &&
			(Clip.Vertex[v2].yScreen <		World->ViewPort.yMin))
		{
			// Clip texture map
			m =	(float)(World->ViewPort.yMin - Clip.Vertex[v1].yScreen) /
					(float)(Clip.Vertex[v2].yScreen - Clip.Vertex[v1].yScreen);
			TmpText[cp].x			= Clip.TextPt[v1].x + ((Clip.TextPt[v2].x - Clip.TextPt[v1].x) * m);
			TmpText[cp].y			= Clip.TextPt[v1].y + ((Clip.TextPt[v2].y - Clip.TextPt[v1].y) * m);

			// Clip polygon
			if(DeltaY != 0)
			{
				m = (float)(DeltaX/DeltaY);
				TmpVert[cp].xScreen = Clip.Vertex[v1].xScreen + ((World->ViewPort.yMin - Clip.Vertex[v1].yScreen) * m);
				m = (float)(DeltaZ/DeltaY);
				TmpVert[cp].zCamera = Clip.Vertex[v1].zCamera + ((World->ViewPort.yMin - Clip.Vertex[v1].yScreen) * m);
			}
			else
			{
				TmpVert[cp].xScreen = Clip.Vertex[v1].xScreen;
				TmpVert[cp].zCamera = Clip.Vertex[v1].zCamera;
			}
			TmpVert[cp].yScreen	= World->ViewPort.yMin;
			cp++;
		}

		//
		// Edge is entering ViewPort
		//
		if((Clip.Vertex[v1].yScreen <		World->ViewPort.yMin) &&
			(Clip.Vertex[v2].yScreen >=	World->ViewPort.yMin))
		{
			// Clip texture map
			m =	(float)(World->ViewPort.yMin - Clip.Vertex[v1].yScreen) /
					(float)(Clip.Vertex[v2].yScreen - Clip.Vertex[v1].yScreen);
			TmpText[cp].x			= Clip.TextPt[v1].x + ((Clip.TextPt[v2].x - Clip.TextPt[v1].x) * m);
			TmpText[cp].y			= Clip.TextPt[v1].y + ((Clip.TextPt[v2].y - Clip.TextPt[v1].y) * m);

			// Clip polygon
			if(DeltaY != 0)
			{
				m = (float)(DeltaX/DeltaY);
				TmpVert[cp].xScreen = Clip.Vertex[v1].xScreen + ((World->ViewPort.yMin - Clip.Vertex[v1].yScreen) * m);
				m = (float)(DeltaZ/DeltaY);
				TmpVert[cp].zCamera = Clip.Vertex[v1].zCamera + ((World->ViewPort.yMin - Clip.Vertex[v1].yScreen) * m);
			}
			else
			{
				TmpVert[cp].xScreen	= Clip.Vertex[v1].xScreen;
				TmpVert[cp].zCamera 	= Clip.Vertex[v1].zCamera;
			}
			TmpVert[cp].yScreen	= World->ViewPort.yMin;
			cp++;

			TmpVert[cp].xScreen	= Clip.Vertex[v2].xScreen;
			TmpVert[cp].yScreen	= Clip.Vertex[v2].yScreen;
			TmpVert[cp].zCamera	= Clip.Vertex[v2].zCamera;
			TmpText[cp]				= Clip.TextPt[v2];
			cp++;
		}
		v1 = v2;
	}
	Clip.NumVertices = cp;

	// ------------------------------------------------------------
	// Clip against bottom of ViewPort
	// ------------------------------------------------------------
	cp = 0;
	v1 = Clip.NumVertices-1;

	for(v2 = 0; v2 < Clip.NumVertices; v2++)
	{
		DeltaX = TmpVert[v2].xScreen - TmpVert[v1].xScreen;
		DeltaY = TmpVert[v2].yScreen - TmpVert[v1].yScreen;
		DeltaZ = TmpVert[v2].zCamera - TmpVert[v1].zCamera;

		//
		// Edge is not off bottom of ViewPort
		//
		if((TmpVert[v1].yScreen <= World->ViewPort.yMax) &&
			(TmpVert[v2].yScreen <= World->ViewPort.yMax))
		{
			// Clip texture map
			Clip.TextPt[cp]			= TmpText[v2];

			// Clip polygon
			Clip.Vertex[cp].xScreen	= TmpVert[v2].xScreen;
			Clip.Vertex[cp].yScreen	= TmpVert[v2].yScreen;
			Clip.Vertex[cp].zCamera	= TmpVert[v2].zCamera;
			cp++;
		}

		//
		// Edge is entirely off bottom of ViewPort
		//
		if((TmpVert[v1].yScreen >	World->ViewPort.yMax) &&
			(TmpVert[v2].yScreen >	World->ViewPort.yMax))
		{
		}

		//
		// Edge is leaving ViewPort
		//
		if((TmpVert[v1].yScreen <=	World->ViewPort.yMax) &&
			(TmpVert[v2].yScreen >	World->ViewPort.yMax))
		{
			// Clip texture map
			m =	(float)(World->ViewPort.yMax - TmpVert[v1].yScreen) /
					(float)(TmpVert[v2].yScreen - TmpVert[v1].yScreen);
			Clip.TextPt[cp].x			= TmpText[v1].x + ((TmpText[v2].x - TmpText[v1].x) * m);
			Clip.TextPt[cp].y			= TmpText[v1].y + ((TmpText[v2].y - TmpText[v1].y) * m);

			// Clip polygon
			if(DeltaY != 0)
			{
				m = (float)(DeltaX/DeltaY);
				Clip.Vertex[cp].xScreen = TmpVert[v1].xScreen + ((World->ViewPort.yMax - TmpVert[v1].yScreen) * m);
				m = (float)(DeltaZ/DeltaY);
				Clip.Vertex[cp].zCamera = TmpVert[v1].zCamera + ((World->ViewPort.yMax - TmpVert[v1].yScreen) * m);
			}
			else
			{
				Clip.Vertex[cp].xScreen	= TmpVert[v1].xScreen;
				Clip.Vertex[cp].zCamera = TmpVert[v1].zCamera;
			}
			Clip.Vertex[cp].yScreen	= World->ViewPort.yMax;
			cp++;
		}

		//
		// Edge is entering ViewPort
		//
		if((TmpVert[v1].yScreen >	World->ViewPort.yMax) &&
			(TmpVert[v2].yScreen <=	World->ViewPort.yMax))
		{
			// Clip texture map
			m =	(float)(World->ViewPort.yMax - TmpVert[v1].yScreen) /
					(float)(TmpVert[v2].yScreen - TmpVert[v1].yScreen);
			Clip.TextPt[cp].x	= TmpText[v1].x + ((TmpText[v2].x - TmpText[v1].x) * m);
			Clip.TextPt[cp].y	= TmpText[v1].y + ((TmpText[v2].y - TmpText[v1].y) * m);

			// Clip polygon
			if(DeltaY != 0)
			{
				m = (float)(DeltaX/DeltaY);
				Clip.Vertex[cp].xScreen = TmpVert[v1].xScreen + ((World->ViewPort.yMax - TmpVert[v1].yScreen) * m);

				m = (float)(DeltaZ/DeltaY);
				Clip.Vertex[cp].zCamera = TmpVert[v1].zCamera + ((World->ViewPort.yMax - TmpVert[v1].yScreen) * m);
			}
			else
			{
				Clip.Vertex[cp].xScreen	= Clip.Vertex[v1].xScreen;
				Clip.Vertex[cp].zCamera = Clip.Vertex[v1].zCamera;
			}
			Clip.Vertex[cp].yScreen	= World->ViewPort.yMax;
			cp++;

			Clip.Vertex[cp].xScreen	= TmpVert[v2].xScreen;
			Clip.Vertex[cp].yScreen	= TmpVert[v2].yScreen;
			Clip.Vertex[cp].zCamera	= TmpVert[v2].zCamera;
			Clip.TextPt[cp]			= TmpText[v2];
			cp++;
		}
		v1 = v2;
	}
	Clip.NumVertices = cp;

	delete[]	TmpVert;
	delete[]	TmpText;
};

