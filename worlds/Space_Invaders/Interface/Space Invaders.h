#ifndef SPACEINV_SPACEINV_H
#define SPACEINV_SPACEINV_H
// ------------------------------------------------------------
// Space Invaders '98: Our Second Contact
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------

//
// Keyboard defines
//
#define	LEFT		0
#define	RIGHT		1
#define	FIRE		2

//
// Alien structure
//
typedef struct
{
	OBJECT	*Model;
	bool		Alive;
} ALIEN;

#endif
