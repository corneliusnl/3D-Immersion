#ifndef IMMERSION_WORLD_H
#define IMMERSION_WORLD_H
// ------------------------------------------------------------
// 3D Immersion World
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include "Include\Object.h"
#include "Include\GContext.h"
#include "Include\Camera.h"
#include "Include\Light.h"
#include "Include\Matrix.h"
#include "Include\Input.h"
#include "Include\Tokenizer.h"
#include "Include\Types.h"
#include "Include\Interface.h"
#include "Include\Multimedia.h"

class OBJECT;
class WORLD;

typedef long (CALLBACK *INTERFACEPROC)	(WORLD *World, unsigned int Message, WPARAM wParam, LPARAM lParam);

//
// World Class
//
class WORLD
{
private:
	FILEINPUT	*Input;
	TOKEN			*Token;

	// Interface handler procedures
	HANDLE			hLibrary;
	INTERFACEPROC	InterfaceProc;

public:
	VIEWPORT	ViewPort;
	GCONTEXT	*GContext;
	CAMERA	*Camera;
	MATRIX	*Matrix;
	OBJECT	*Object;
	LIGHT		*Light;
	RGBQUAD	BGColor;
	DWORD		DeltaTime, OldTime, CurrentTime;
	MCISEQ	*Midi;
	HWND		hParent;
	float		Gravity;
	char		Filename[256];
	char		Description[256];
	char		MusicFile[256];
	char		Interface[256];
	char		Palette[256];
	long		*zBuffer;
	int		NumObjects, NumCameras, NumLights;

	// Construction/Destruction
	WORLD(HWND hWnd, int Width, int Height);
	~WORLD();

	// File Operations
	void	New();
	void	Load(char *Filename);
	void	Save(char *Filename);

	// File Parsing
	void	ParseWorld();
	void	ParseAddObject();
	void	ParseAddLight();
	void	ParseAddCamera();

	// World procedures
	void		Render();
	OBJECT	*AddObject(char *Filename, char *Alias);
	CAMERA	*AddCamera(char *Alias);
	LIGHT		*AddLight(char *Alias);
	OBJECT	*FindObject(char *Alias);
	CAMERA	*FindCamera(char *Alias);
	LIGHT		*FindLight(char *Alias);
	void		RemoveObject(OBJECT *Obj);
	void		RemoveCamera(CAMERA *Obj);
	void		RemoveLight(LIGHT *Obj);
	void		SwitchToCamera(CAMERA *Cam);
	void		DrawObjects();
	void		MciNotify(WPARAM wParam);
	void		SetMusic(char *Filename);
	void		SetInterface(char *Filename);
	void		SetGravity(float Gravity);
	void		SetBackgroundColor(int Red, int Green, int Blue);
	void		SetPalette(char *Filename);

	// Interface procedures
	void	MouseDown(int X, int Y);
	void	MouseMove(int X, int Y);
	void	MouseUp(int X, int Y);
	long	SendMessage(unsigned int Message, WPARAM wParam, LPARAM lParam);
};

#endif
