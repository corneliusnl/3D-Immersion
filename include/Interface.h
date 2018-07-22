#ifndef INTERFACE_INTERFACE_H
#define INTERFACE_INTERFACE_H
// ------------------------------------------------------------
// Interface Message Handler Prototypes
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include "Include\World.h"
#include "Include\Object.h"

#define DLL_EXPORT	__declspec(dllexport)

class	WORLD;
class OBJECT;

//
// Interface Message Defines
//
#define IF_CREATE			0x00000000
#define IF_TIMER			0x00000001
#define IF_KEYDOWN		0x00000002
#define IF_KEYUP			0x00000003
#define IF_UPDATE			0x00000004
#define IF_MOUSEDOWN		0x00000005
#define IF_MOUSEMOVE		0x00000006
#define IF_MOUSEUP  		0x00000007
#define IF_DESTROY		0x00000010

//
// Object Interface Defines
//
#define OBJ_CREATE		0x00000001
#define OBJ_ANIMATE		0x00000002
#define OBJ_MOUSEDOWN	0x00000003
#define OBJ_MOUSEMOVE	0x00000004
#define OBJ_MOUSEUP		0x00000005
#define OBJ_DESTROY		0x00000006
#define OBJ_TIMER			0x00000007

//
// Interface Procedures
//
extern "C"
{
	DLL_EXPORT long __stdcall InterfaceProc(WORLD *World, unsigned int Message, WPARAM wParam, LPARAM lParam);
	DLL_EXPORT long __stdcall ObjectProc(WORLD *World, OBJECT *Object, unsigned int Message, WPARAM wParam, LPARAM lParam);
};

#endif
