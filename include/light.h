#ifndef IMMERSION_LIGHT_H
#define IMMERSION_LIGHT_H
// ------------------------------------------------------------
// Light
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\Types.h"

//
// Light Types
//
#define LGT_INFINITE	0
#define LGT_POINT		1

//
// Light Class
//
class LIGHT
{
public:
	LIGHT		*Prev, *Next;
	VECTOR	Direction;
	VERTEX	Vertex;
	float		xTranslate, yTranslate, zTranslate;
	float		Ambience;
	char		Identifier[256];
	bool		On;
	int		Type;

	// Class construction/Destruction
	LIGHT(int Type);
	~LIGHT();

	// Exposed functions
	void	SetAlias(char *Alias);
	void	TurnedOn(bool On);
	void	SetAmbience(float Ambience);
	void	SetDirection(float x, float y, float z);
	void	Translate(float x, float y, float z);

	// Method
	float	CalcIllum(VECTOR *Vector);
};

#endif
