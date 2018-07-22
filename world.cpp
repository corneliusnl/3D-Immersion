// ------------------------------------------------------------
// 3D Immersion World
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <Math.h>
#include "Include\Interface.h"
#include "Include\Error.h"
#include "Include\Mathematics.h"
#include "Include\World.h"

// ------------------------------------------------------------
// WORLD::WORLD(int Width, int Height)
// ------------------------------------------------------------
// Description
//  Constructs a empty world for objects to be placed in
//
// Parameters
//  Width		- Width of the display window
//  Height		- Height of the display window
//
// Returns
//  Class instance
// ------------------------------------------------------------
WORLD::WORLD(HWND hWnd, int Width, int Height)
{
	Matrix	= new MATRIX();
	GContext = new GCONTEXT(Width, Height);
	zBuffer	= new long[Width*Height];
//	InitTrigTables();

	// Set defaults
	strcpy(Filename, "<Undefined>");
	strcpy(MusicFile, "<Undefined>");
	strcpy(Description, "");
	SetPalette("Default.pal");
	SetInterface("Standard.dll");
	SetBackgroundColor(0, 0, 0);
	Gravity			= 0;
	hParent			= hWnd;
	Midi				= NULL;
	InterfaceProc  = NULL;
	hLibrary			= NULL;
	Object			= NULL;
	NumObjects		= 0;
	Camera			= NULL;
	NumCameras		= 0;
	Light				= NULL;
	NumLights		= 0;

	// Configure Viewport
	ViewPort.xMin		= 0;
	ViewPort.xMax		= Width;
	ViewPort.yMin		= 0;
	ViewPort.yMax		= Height;
	ViewPort.xCenter	= Width / 2;
	ViewPort.yCenter	= Height / 2;
};

// ------------------------------------------------------------
// WORLD::~WORLD()
// ------------------------------------------------------------
// Description
//  Handle the class destruction by unallocating allocated
// members.
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
WORLD::~WORLD()
{
	//
	// Library must be freed first
	//
	if(hLibrary != NULL)
	{
		SendMessage(IF_DESTROY, 0, 0L);
		FreeLibrary(hLibrary);
	}

	delete	Object;
	delete	Light;
	delete	Camera;
	delete	Midi;
	delete[]	zBuffer;
	GContext->DeleteGDC();
	delete	GContext;
	delete	Matrix;
};

// ------------------------------------------------------------
// void WORLD::New()
// ------------------------------------------------------------
// Description
//  Creates a new world
//
// Parameters
//  Filename	- Name of the world definition file
//
// Returns
//  Class instance
// ------------------------------------------------------------
void WORLD::New()
{
	CAMERA	*TmpCam;
	LIGHT		*TmpLgt;

	// Set default camera
	TmpCam = new CAMERA();
	TmpCam->SetAlias("Default Camera");
	TmpCam->SetYon(50000);
	TmpCam->SetHither(10);
	TmpCam->SetProjectionPlane(1000);
	TmpCam->Translate(0, 100, 0);
	NumCameras++;

	TmpCam->Prev	= NULL;
	TmpCam->Next	= NULL;
	Camera			= TmpCam;

	// Set default light
	TmpLgt = new LIGHT(LGT_INFINITE);
	TmpLgt->SetAlias("Default Light");
	TmpLgt->SetAmbience(0.2);
	TmpLgt->SetDirection(-1, 1, -1);
	NumLights++;

	TmpLgt->Prev	= NULL;
	TmpLgt->Next	= NULL;
	Light				= TmpLgt;
};

// ------------------------------------------------------------
// void WORLD::Load(char *Filename)
// ------------------------------------------------------------
// Description
//  Loads a predefined world from disk
//
// Parameters
//  Filename	- Name of the world definition file
//
// Returns
//  Class instance
// ------------------------------------------------------------
void WORLD::Load(char *Filename)
{
	// Allocate input and tokenizer
	Input = new FILEINPUT(Filename);
	Token = new TOKEN(Input);
	strcpy(this->Filename, Filename);

	// Parse File
	while(Token->Get() != TOK_EOF)
	{
		switch(Token->Code)
		{
			case TOK_WORLD:
				ParseWorld();
				break;

			case TOK_NULL:
				break;

			default:
				Warning(WARN_SYNTAX, Input);
				break;
		}
	}

	// Free tokenizer and input
	delete Token;
	delete Input;
};

// ------------------------------------------------------------
// void WORLD::Save(char *Filename)
// ------------------------------------------------------------
// Description
//  Write the world to a file
//
// Parameters
//  Filename		- Filename of world data to be saved to
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::Save(char *Filename)
{
	OBJECT	*Obj;
	CAMERA	*Cam;
   LIGHT		*Lgt;
	FILE	*Output;
	char	Buff[256];

	Output = fopen(Filename, "wt");

	//
	// Write stamp
	//
	fputs("; World Definition File\n", Output);
	fputs("; Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.\n", Output);
	fputs(";\n", Output);
	fputs("\n", Output);

	//
	// Start world structure
	//
	sprintf(Buff, "World(\"%s\")\n", Description);
	fputs(Buff, Output);
	fputs("{\n", Output);

	// Write Interface
	if(strcmp("<Undefined>", Interface) != NULL)
	{
		sprintf(Buff, "\tSetInterface(\"%s\")\n", Interface);
		fputs(Buff, Output);
	}

	// Write Palette
	if(strcmp("<Undefined>", Palette) != NULL)
	{
		sprintf(Buff, "\tSetPalette(\"%s\")\n", Palette);
		fputs(Buff, Output);
	}

	// Write Music
	if(strcmp("<Undefined>", MusicFile) != NULL)
	{
		sprintf(Buff, "\tSetMusic(\"%s\")\n", MusicFile);
		fputs(Buff, Output);
	}

	// Write Background Color
	sprintf(Buff, "\tSetBackgroundColor(%d, %d, %d)\n",
		BGColor.rgbRed,
		BGColor.rgbGreen,
		BGColor.rgbBlue);
	fputs(Buff, Output);

	// Write Gravity Constant
	sprintf(Buff, "\tSetGravity(%f)\n",	Gravity);
	fputs(Buff, Output);

	fputs("\n", Output);

	//
	// Write objects
	//
	for(Obj = Object; Obj; Obj = Obj->Next)
	{
		sprintf(Buff, "\tAddObject(\"%s\", \"%s\")\n", Obj->Filename, Obj->Identifier);
		fputs(Buff, Output);
		fputs("\t{\n", Output);

		// Write Interface
		if(strcmp("<Undefined>", Obj->Interface) != NULL)
		{
			sprintf(Buff, "\t\tSetInterface(\"%s\")\n", Obj->Interface);
			fputs(Buff, Output);
		}

		// Write Rotation
		sprintf(Buff, "\t\tRotate(%.3f, %.3f, %.3f)\n",
			(Obj->xRotate * (180/M_PI)),
			(Obj->yRotate * (180/M_PI)),
			(Obj->zRotate * (180/M_PI)));
		fputs(Buff, Output);

		// Write Translation
		sprintf(Buff, "\t\tTranslate(%.3f, %.3f, %.3f)\n",
			Obj->xTranslate,
			Obj->yTranslate,
			Obj->zTranslate);
		fputs(Buff, Output);

		// Write Scaling
		sprintf(Buff, "\t\tScale(%.3f, %.3f, %.3f)\n",
			Obj->xScale,
			Obj->yScale,
			Obj->zScale);
		fputs(Buff, Output);

		// Write Linear Velocity
		sprintf(Buff, "\t\tSetLinearVelocity(%.3f, %.3f, %.3f)\n",
			Obj->LinearVelocity.xComp,
			Obj->LinearVelocity.yComp,
			Obj->LinearVelocity.zComp);
		fputs(Buff, Output);

		// Write Linear Acceleration
		sprintf(Buff, "\t\tSetLinearAcceleration(%.3f, %.3f, %.3f)\n",
			Obj->LinearAcceleration.xComp,
			Obj->LinearAcceleration.yComp,
			Obj->LinearAcceleration.zComp);
		fputs(Buff, Output);

		// Write Rotational Velocity
		sprintf(Buff, "\t\tSetRotationalVelocity(%.3f, %.3f, %.3f)\n",
			Obj->RotationalVelocity.xComp,
			Obj->RotationalVelocity.yComp,
			Obj->RotationalVelocity.zComp);
		fputs(Buff, Output);

		// Write Rotational Acceleration
		sprintf(Buff, "\t\tSetRotationalAcceleration(%.3f, %.3f, %.3f)\n",
			Obj->RotationalAcceleration.xComp,
			Obj->RotationalAcceleration.yComp,
			Obj->RotationalAcceleration.zComp);
		fputs(Buff, Output);

		// Write Mass
		sprintf(Buff, "\t\tSetMass(%.3f)\n", Obj->Mass);
		fputs(Buff, Output);

		// Write Force
		sprintf(Buff, "\t\tSetForce(%.3f)\n", Obj->Force);
		fputs(Buff, Output);

		// Write Friction
		sprintf(Buff, "\t\tSetFriction(%.3f)\n", Obj->Friction);
		fputs(Buff, Output);

		fputs("\t}\n", Output);
		fputs("\n", Output);
	}

	//
	// Write cameras
	//
	for(Cam = Camera; Cam; Cam = Cam->Next)
	{
		sprintf(Buff, "\tAddCamera(\"%s\")\n", Cam->Identifier);
		fputs(Buff, Output);
		fputs("\t{\n", Output);

		// Write Rotation
		sprintf(Buff, "\t\tRotate(%.3f, %.3f, %.3f)\n",
			(Cam->Pitch * (180/M_PI)),
			(Cam->Yaw * (180/M_PI)),
			(Cam->Roll * (180/M_PI)));
		fputs(Buff, Output);

		// Write Translation
		sprintf(Buff, "\t\tTranslate(%.3f, %.3f, %.3f)\n",
			Cam->xTranslate,
			Cam->yTranslate,
			Cam->zTranslate);
		fputs(Buff, Output);

		// Write Hither
		sprintf(Buff, "\t\tSetHither(%d)\n", Cam->Hither);
		fputs(Buff, Output);

		// Write Yon
		sprintf(Buff, "\t\tSetYon(%d)\n", Cam->Yon);
		fputs(Buff, Output);

		// Write Projection Plane
		sprintf(Buff, "\t\tSetProjectionPlane(%d)\n", Cam->ProjectionPlane);
		fputs(Buff, Output);

		fputs("\t}\n", Output);
		fputs("\n", Output);
	}

	//
	// Write lights
	//
	for(Lgt = Light; Lgt; Lgt = Lgt->Next)
	{
		sprintf(Buff, "\tAddLight(\"%s\")\n", Lgt->Identifier);
		fputs(Buff, Output);
		fputs("\t{\n", Output);

		// Write Direction
		sprintf(Buff, "\t\tSetDirection(%.3f, %.3f, %.3f)\n",
			Lgt->Direction.xComp,
			Lgt->Direction.yComp,
			Lgt->Direction.zComp);
		fputs(Buff, Output);

		// Write Position
		sprintf(Buff, "\t\tTranslate(%.3f, %.3f, %.3f)\n",
			Lgt->xTranslate,
			Lgt->yTranslate,
			Lgt->zTranslate);
		fputs(Buff, Output);

		// Write Ambience
		sprintf(Buff, "\t\tSetAmbience(%.3f)\n", Lgt->Ambience);
		fputs(Buff, Output);

		// Write Switch Status
		if(Lgt->On == true)
			fputs("\t\tTurnedOn(true)\n", Output);
		else
			fputs("\t\tTurnedOn(false)\n", Output);

		fputs("\t}\n", Output);
		fputs("\n", Output);
	}

	fputs("}", Output);

	fclose(Output);
};

// ------------------------------------------------------------
// void WORLD::ParseWorld()
// ------------------------------------------------------------
// Description
//  Parses the world command
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::ParseWorld()
{
	int	Red, Green, Blue;

	//
	// Parameters
	//
	Token->Get(TOK_LPAREN);
	{
		// Descriptive text
		Token->Get(TOK_STRING);
		strcpy(Description, Token->Value.String);
	}
	Token->Get(TOK_RPAREN);

	//
	// Code Block
	//
	Token->Get(TOK_LCB);
	{
		while(Token->Get() != TOK_RCB)
		{
			switch(Token->Code)
			{
				case TOK_SETGRAVITY:
					Token->Get(TOK_LPAREN);

					Token->Get(TOK_NUMBER);
					SetGravity(Token->Value.Number);

					Token->Get(TOK_RPAREN);
					break;

				case TOK_SETMUSIC:
					Token->Get(TOK_LPAREN);

					Token->Get(TOK_STRING);
					SetMusic(Token->Value.String);

					Token->Get(TOK_RPAREN);
					break;

				case TOK_SETPALETTE:
					Token->Get(TOK_LPAREN);

					// Filename
					Token->Get(TOK_STRING);
					SetPalette(Token->Value.String);

					Token->Get(TOK_RPAREN);
					break;

				case TOK_SETINTERFACE:
					Token->Get(TOK_LPAREN);

					// Dynamic link library
					Token->Get(TOK_STRING);
					SetInterface(Token->Value.String);

					Token->Get(TOK_RPAREN);
					break;

				case TOK_SETBACKGROUNDCOLOR:
					Token->Get(TOK_LPAREN);

					// Red Component
					Token->Get(TOK_NUMBER);
					Red	= Token->Value.Number;

					// Green Component
					Token->Get(TOK_COMMA);
					Token->Get(TOK_NUMBER);
					Green	= Token->Value.Number;

					// Blue Component
					Token->Get(TOK_COMMA);
					Token->Get(TOK_NUMBER);
					Blue	= Token->Value.Number;

					SetBackgroundColor(Red, Green, Blue);

					Token->Get(TOK_RPAREN);
					break;

				case TOK_ADDOBJECT:
					ParseAddObject();
					break;

				case TOK_ADDCAMERA:
					ParseAddCamera();
					break;

				case TOK_ADDLIGHT:
					ParseAddLight();
					break;

				case TOK_NULL:
					break;

				default:
					Warning(WARN_SYNTAX, Input);
					break;
			}
		}
	}

	//
	// Notify interface handler
	//
	CurrentTime = timeGetTime();
	SendMessage(IF_CREATE, 0, 0L);
};

// ------------------------------------------------------------
// void WORLD::ParseAddObject()
// ------------------------------------------------------------
// Description
//  Parses the add object command
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::ParseAddObject()
{
	OBJECT	*TmpObj;
	float		X, Y, Z;

	//
	// Parameters
	//
	Token->Get(TOK_LPAREN);

	// Object Definition File
	Token->Get(TOK_STRING);
	TmpObj	= AddObject(Token->Value.String, "<Undefined>");

	// Alias
	Token->Get(TOK_COMMA);
	Token->Get(TOK_STRING);
	TmpObj->SetAlias(Token->Value.String);

	Token->Get(TOK_RPAREN);

	//
	// Code Block
	//
	Token->Get(TOK_LCB);
	while(Token->Get() != TOK_RCB)
	{
		switch(Token->Code)
		{
			case TOK_SETFRICTION:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				TmpObj->SetFriction(Token->Value.Number);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETFORCE:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				TmpObj->SetForce(Token->Value.Number);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETINTERFACE:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_STRING);
				TmpObj->SetInterface(Token->Value.String);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETLINEARACCELERATION:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number;

				TmpObj->SetLinearAcceleration(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETLINEARVELOCITY:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number;

				TmpObj->SetLinearVelocity(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETROTATIONALACCELERATION:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number;

				TmpObj->SetRotationalAcceleration(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETROTATIONALVELOCITY:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z	= Token->Value.Number;

				TmpObj->SetRotationalVelocity(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETMASS:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				TmpObj->SetMass(Token->Value.Number);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETELASTICITY:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				TmpObj->SetElasticity(Token->Value.Number);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_TRANSLATE:
				Token->Get(TOK_LPAREN);

				// X component
				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				// Y component
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				// Z component
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number;

				TmpObj->Translate(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_ROTATE:
				Token->Get(TOK_LPAREN);

				// X component
				Token->Get(TOK_NUMBER);
				X = (Token->Value.Number * (M_PI/180));

				// Y component
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = (Token->Value.Number * (M_PI/180));

				// Z component
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = (Token->Value.Number * (M_PI/180));

				TmpObj->Rotate(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SCALE:
				Token->Get(TOK_LPAREN);

				// X component
				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				// Y component
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				// Z component
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number;

				TmpObj->Scale(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_NULL:
				break;

			default:
				Warning(WARN_SYNTAX, Input);
				break;
		}
	}
};

// ------------------------------------------------------------
// void WORLD::ParseAddCamera()
// ------------------------------------------------------------
// Description
//  Parses the add object command
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::ParseAddCamera()
{
	CAMERA	*TmpCam;
	float		X, Y, Z;

	//
	// Set Defaults
	//
	TmpCam =	AddCamera("<Undefined>");
	TmpCam->SetHither(10);
	TmpCam->SetYon(10000);
	TmpCam->SetProjectionPlane(1000);

	//
	// Parameters
	//
	Token->Get(TOK_LPAREN);

	// Alias
	Token->Get(TOK_STRING);
	strcpy(TmpCam->Identifier, Token->Value.String);

	Token->Get(TOK_RPAREN);

	//
	// Code Block
	//
	Token->Get(TOK_LCB);
	while(Token->Get() != TOK_RCB)
	{
		switch(Token->Code)
		{
			case TOK_TRANSLATE:
				Token->Get(TOK_LPAREN);

				// X
				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				// Y
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				// Z
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number;

				TmpCam->Translate(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_ROTATE:
				Token->Get(TOK_LPAREN);

				// X
				Token->Get(TOK_NUMBER);
				X	= (Token->Value.Number * (M_PI/180));

				// Y
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = (Token->Value.Number * (M_PI/180));

				// Z
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number * (M_PI/180);

				TmpCam->Rotate(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETHITHER:
				Token->Get(TOK_LPAREN);

				// Hither
				Token->Get(TOK_NUMBER);
				TmpCam->SetHither(Token->Value.Number);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETYON:
				Token->Get(TOK_LPAREN);

				// Yon
				Token->Get(TOK_NUMBER);
				TmpCam->SetYon(Token->Value.Number);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETPROJECTIONPLANE:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_NUMBER);
				TmpCam->SetProjectionPlane(Token->Value.Number);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_NULL:
				break;

			default:
				Warning(WARN_SYNTAX, Input);
				break;
		}
	}
};

// ------------------------------------------------------------
// void WORLD::ParseAddLight()
// ------------------------------------------------------------
// Description
//  Parses the add light command
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::ParseAddLight()
{
	LIGHT	*TmpLgt;
	float	X, Y, Z;
	char	Buff[256];

	//
	// Set Defaults
	//
	TmpLgt = AddLight("<Undefined>");
	TmpLgt->TurnedOn(true);

	//
	// Parameters
	//
	Token->Get(TOK_LPAREN);

	// Alias
	Token->Get(TOK_STRING);
	strcpy(TmpLgt->Identifier, Token->Value.String);

	Token->Get(TOK_RPAREN);

	//
	// Code Block
	//
	Token->Get(TOK_LCB);
	while(Token->Get() != TOK_RCB)
	{
		switch(Token->Code)
		{
			case TOK_SETAMBIENCE:
				Token->Get(TOK_LPAREN);

				// Ambience value
				Token->Get(TOK_NUMBER);
				TmpLgt->SetAmbience(Token->Value.Number);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_TURNEDON:
				Token->Get(TOK_LPAREN);

				Token->Get(TOK_BOOLEAN);
				TmpLgt->TurnedOn(Token->Value.Boolean);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETDIRECTION:
				Token->Get(TOK_LPAREN);

				// X
				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				// Y
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				// Z
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number;

				TmpLgt->SetDirection(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_TRANSLATE:
				Token->Get(TOK_LPAREN);

				// X
				Token->Get(TOK_NUMBER);
				X = Token->Value.Number;

				// Y
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Y = Token->Value.Number;

				// Z
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				Z = Token->Value.Number;

				TmpLgt->Translate(X, Y, Z);

				Token->Get(TOK_RPAREN);
				break;

			case TOK_NULL:
				break;

			default:
				Warning(WARN_SYNTAX, Input);
				break;
		}
	}
};

// ------------------------------------------------------------
// void WORLD::MciNotify(WPARAM wParam)
// ------------------------------------------------------------
// Description
//  Handles the mci notify message
//
// Parameters
//  wParam		- Word parameter of the MM_MCINOTIFY message
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::MciNotify(WPARAM wParam)
{
	switch(wParam)
	{
		case MCI_NOTIFY_SUCCESSFUL:
			delete Midi;
			Midi = new MCISEQ(hParent, MusicFile);
			Midi->Play();
			break;
	}
}


// ------------------------------------------------------------
// long WORLD::SendMessage(unsigned int Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Sets the specified message to the world's interface
// procedure handler.
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
long WORLD::SendMessage(unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	if(InterfaceProc)
	{
		switch(Message)
		{
			case IF_CREATE:
				InterfaceProc(this, Message, wParam, lParam);
				break;

			case IF_KEYDOWN:
				InterfaceProc(this, Message, wParam, lParam);
				break;

			case IF_KEYUP:
				InterfaceProc(this, Message, wParam, lParam);
				break;

			case IF_UPDATE:
				InterfaceProc(this, Message, wParam, lParam);
				break;

			case IF_DESTROY:
				InterfaceProc(this, Message, wParam, lParam);
				break;
		}
	}
};

// ------------------------------------------------------------
// OBJECT *WORLD::AddObject(char *Filename, char *Alias)
// ------------------------------------------------------------
// Description
//  Adds an object to the worlds doubly linked list of objects
//
// Parameters
//  Filename	- Name of the object
//  Alias		- Alias of the object
//
// Returns
//  Pointer to object
// ------------------------------------------------------------
OBJECT *WORLD::AddObject(char *Filename, char *Alias)
{
	OBJECT	*Obj;

	Obj = new OBJECT(this, Filename);
	Obj->SetAlias(Alias);
	Obj->Prev = NULL;
	Obj->Next = Object;
	if(Object != NULL)
		Object->Prev = Obj;
	Object = Obj;
	NumObjects++;

	return Object;
};

// ------------------------------------------------------------
// void WORLD::RemoveObject(OBJECT *Obj)
// ------------------------------------------------------------
// Description
//  Removes an object to the worlds doubly linked list of objects
//
// Parameters
//  Obj			- Object to be removed from list
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::RemoveObject(OBJECT *Obj)
{
	if(Obj->Prev != NULL)
		Obj->Prev->Next = Obj->Next;
	else
		Object = Obj->Next;

	if(Obj->Next != NULL)
		Obj->Next->Prev = Obj->Prev;

	NumObjects--;
};

// ------------------------------------------------------------
// CAMERA *WORLD::AddCamera(char *Alias)
// ------------------------------------------------------------
// Description
//  Adds an camera to the worlds doubly linked list of camera's
//
// Parameters
//  Alias		- Alias of the camera
//
// Returns
//  Pointer to camera
// ------------------------------------------------------------
CAMERA *WORLD::AddCamera(char *Alias)
{
	CAMERA	*Cam;

	Cam = new CAMERA();
	Cam->SetAlias(Alias);
	Cam->Prev		= NULL;
	Cam->Next		= Camera;
	if(Camera != NULL)
		Camera->Prev	= Cam;
	Camera			= Cam;
	NumCameras++;

	return Cam;
};

// ------------------------------------------------------------
// void WORLD::RemoveCamera(CAMERA *Cam)
// ------------------------------------------------------------
// Description
//  Removes a camera from the worlds doubly linked list of objects
//
// Parameters
//  Cam			- Pointer of camera to be removed
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::RemoveCamera(CAMERA *Cam)
{
	if(Cam->Prev != NULL)
		Cam->Prev->Next = Cam->Next;
	else
		Camera = Cam->Next;

	if(Cam->Next != NULL)
		Cam->Next->Prev = Cam->Prev;

	NumCameras--;
};

// ------------------------------------------------------------
// void WORLD::SwitchToCamera(CAMERA *Cam)
// ------------------------------------------------------------
// Description
//  Switchs the current to the one specified
//
// Parameters
//  Cam			- Pointer of camera to be removed
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::SwitchToCamera(CAMERA *Cam)
{
	if(Cam->Prev != NULL)
		Cam->Prev->Next = Cam->Next;
	else
		Camera = Cam->Next;

	if(Cam->Next != NULL)
		Cam->Next->Prev = Cam->Prev;

	Cam->Prev		= NULL;
	Cam->Next		= Camera;
	if(Camera != NULL)
		Camera->Prev	= Cam;
	Camera			= Cam;
};

// ------------------------------------------------------------
// LIGHT *WORLD::AddLight(char *Alias)
// ------------------------------------------------------------
// Description
//  Adds an light to the worlds doubly linked list of lights
//
// Parameters
//  Alias		- Alias of the light
//
// Returns
//  Pointer to light
// ------------------------------------------------------------
LIGHT *WORLD::AddLight(char *Alias)
{
	LIGHT *Lgt;

	Lgt = new LIGHT(LGT_INFINITE);
	Lgt->SetAlias(Alias);
	Lgt->Prev	= NULL;
	Lgt->Next	= Light;
	if(Light != NULL)
		Light->Prev = Lgt;
	Light			= Lgt;
	NumLights++;

	return Lgt;
};

// ------------------------------------------------------------
// void WORLD::RemoveLight(LIGHT *Lgt)
// ------------------------------------------------------------
// Description
//  Removes a light from the worlds doubly linked list of objects
//
// Parameters
//  Lgt			- Pointer to light to be removed
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::RemoveLight(LIGHT *Lgt)
{
	if(Lgt->Prev != NULL)
		Lgt->Prev->Next = Lgt->Next;
	else
		Light = Lgt->Next;

	if(Lgt->Next != NULL)
		Lgt->Next->Prev = Lgt->Prev;

	NumLights--;
};


// ------------------------------------------------------------
// void WORLD::SetMusic(char *Filename)
// ------------------------------------------------------------
// Description
//  Sets the background music of the world
//
// Parameters
//  Filename		- Specifies the name of the MIDI file
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::SetMusic(char *Filename)
{
	if(Midi)
		delete Midi;

	strcpy(MusicFile, Filename);
	Midi = new MCISEQ(hParent, MusicFile);
	Midi->Play();
};

// ------------------------------------------------------------
// void WORLD::SetInterface(char *Filename)
// ------------------------------------------------------------
// Description
//  Sets the world's dynamic interface handler
//
// Parameters
//  Filename		- Name of the interface library
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::SetInterface(char *Filename)
{
	char	Buff[256];

	//
	// Free previously allocated library
	//
	if(hLibrary != NULL)
	{
//		SendMessage(IF_DESTROY, 0, 0L);
		FreeLibrary(hLibrary);
	}

	//
	// Load new interface library
	//
	strcpy(Interface, Filename);
	if((hLibrary = LoadLibrary(Interface)))
	{
		InterfaceProc	= (INTERFACEPROC)GetProcAddress(hLibrary, "InterfaceProc");
//		SendMessage(IF_CREATE, 0, 0L);
	}
	else
	{
		wsprintf(Buff, "Unable to load %s!", Token->Value.String);
		MessageBox(GetFocus(), Buff, "Actung!", MB_OK);
		InterfaceProc	= NULL;
	}
};

// ------------------------------------------------------------
// void WORLD::SetGravity(float Gravity)
// ------------------------------------------------------------
// Description
//  Sets the worlds gravity
//
// Parameters
//  Gravity		- Specifies the world's desired gravitational
//					pull factor
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::SetGravity(float Gravity)
{
	this->Gravity = Gravity;
};

// ------------------------------------------------------------
// void WORLD::SetBackgroundColor(int Red, int Green, int Blue)
// ------------------------------------------------------------
// Description
//  Specifies the color that is to paint the background
// between successive frames.
//
// Parameters
//  Red			- Red component of color
//  Green		- Green component of color
//  Blue			- Blue component of color
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::SetBackgroundColor(int Red, int Green, int Blue)
{
	BGColor.rgbRed		= Red;
	BGColor.rgbGreen	= Green;
	BGColor.rgbBlue	= Blue;
};

// ------------------------------------------------------------
// void WORLD::SetPalette(char *Filename)
// ------------------------------------------------------------
// Description
//  Sets the world's color palette
//
// Parameters
//	 Filename		- Name of the ASCII palette file
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::SetPalette(char *Filename)
{
	strcpy(Palette, Filename);
	GContext->LoadPalette(Palette);
};

// ------------------------------------------------------------
// void WORLD::MouseDown(int X, int Y)
// ------------------------------------------------------------
// Description
//  Handles sending the mouse down message to the object
// having the specified X and Y coordinates
//
// Parameters
//  X				- Mouse X coordinate
//  Y				- Mouse Y coordinate
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::MouseDown(int X, int Y)
{
	for(OBJECT *Obj = Object; Obj; Obj = Obj->Next)
	{
		if(Obj->ContainsPoint(X, Y))
			Obj->SendMessage(OBJ_MOUSEDOWN, NULL, MAKELONG(X, Y));
	}
};

// ------------------------------------------------------------
// void WORLD::MouseDown(int X, int Y)
// ------------------------------------------------------------
// Description
//  Handles sending the mouse down move to the object
// having the specified X and Y coordinates
//
// Parameters
//  X				- Mouse X coordinate
//  Y				- Mouse Y coordinate
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::MouseMove(int X, int Y)
{
	for(OBJECT *Obj = Object; Obj; Obj = Obj->Next)
	{
		if(Obj->ContainsPoint(X, Y))
			Obj->SendMessage(OBJ_MOUSEMOVE, NULL, MAKELONG(X, Y));
	}
};

// ------------------------------------------------------------
// void WORLD::MouseDown(int X, int Y)
// ------------------------------------------------------------
// Description
//  Handles sending the mouse up message to the object
// having the specified X and Y coordinates
//
// Parameters
//  X				- Mouse X coordinate
//  Y				- Mouse Y coordinate
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::MouseUp(int X, int Y)
{
	for(OBJECT *Obj = Object; Obj; Obj = Obj->Next)
	{
		if(Obj->ContainsPoint(X, Y))
			Obj->SendMessage(OBJ_MOUSEUP, NULL, MAKELONG(X, Y));
	}
};

// ------------------------------------------------------------
// OBJECT *WORLD::FindObject(char *Alias)
// ------------------------------------------------------------
// Description
//  Finds the object with the specified alias
//
// Parameters
//  Alias		- Alias of the object to seach for
//
// Returns
//  Pointer to object, else NULL
// ------------------------------------------------------------
OBJECT *WORLD::FindObject(char *Alias)
{
	for(OBJECT *Obj = Object; Obj; Obj = Obj->Next)
		if(strcmp(Alias, Obj->Identifier) == 0)
			return Obj;

	return NULL;
};

// ------------------------------------------------------------
// CAMERA *WORLD::FindCamera(char *Alias)
// ------------------------------------------------------------
// Description
//  Finds the camera with the specified alias
//
// Parameters
//  Alias		- Alias of the object to seach for
//
// Returns
//  Pointer to camera, else NULL
// ------------------------------------------------------------
CAMERA *WORLD::FindCamera(char *Alias)
{
	for(CAMERA *Cam = Camera; Cam; Cam = Cam->Next)
		if(strcmp(Alias, Cam->Identifier) == 0)
			return Cam;

	return NULL;
};

// ------------------------------------------------------------
// LIGHT *WORLD::FindLight(char *Alias)
// ------------------------------------------------------------
// Description
//  Finds the light with the specified alias
//
// Parameters
//  Alias		- Alias of the object to seach for
//
// Returns
//  Pointer to light, else NULL
// ------------------------------------------------------------
LIGHT *WORLD::FindLight(char *Alias)
{
	for(LIGHT *Lgt = Light; Lgt; Lgt = Lgt->Next)
		if(strcmp(Alias, Lgt->Identifier) == 0)
			return Lgt;

	return NULL;
};

// ------------------------------------------------------------
// void WORLD::Render()
// ------------------------------------------------------------
// Descripition
//  Renders the world
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::Render()
{
	long	*c;

	OldTime		= CurrentTime;
	CurrentTime	= timeGetTime();
	DeltaTime	= CurrentTime - OldTime;

	// Reset Z-Buffer
	c = (long *)zBuffer;
	for(int i = 0; i < ((GContext->GetWidth() * GContext->GetHeight()) >> 3); i++)
	{
		*(long *)c++ = Camera->Yon;	*(long *)c++ = Camera->Yon;
		*(long *)c++ = Camera->Yon;	*(long *)c++ = Camera->Yon;
		*(long *)c++ = Camera->Yon;	*(long *)c++ = Camera->Yon;
		*(long *)c++ = Camera->Yon;	*(long *)c++ = Camera->Yon;
	}

	// Clear screen
	GContext->Clear(RGB(BGColor.rgbRed, BGColor.rgbGreen, BGColor.rgbBlue));

	// Pass control to interface library
	SendMessage(IF_UPDATE, NULL, NULL);
};

// ------------------------------------------------------------
// void WORLD::DrawObjects()
// ------------------------------------------------------------
// Descripition
//  Draws the world's objects
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void WORLD::DrawObjects()
{
	OBJECT *Obj;

	// Obtain primary angle (0 < Angle < 2*M_PI)
	if(Camera->Pitch	>	2*M_PI)
		Camera->Pitch	-=	2*M_PI;
	if(Camera->Pitch	<	0)
		Camera->Pitch	+=	2*M_PI;

	if(Camera->Yaw		>	2*M_PI)
		Camera->Yaw		-=	2*M_PI;
	if(Camera->Yaw		<	0)
		Camera->Yaw		+=	2*M_PI;

	if(Camera->Roll	>	2*M_PI)
		Camera->Roll	-=	2*M_PI;
	if(Camera->Roll	<	0)
		Camera->Roll	+=	2*M_PI;

	// Compute camera matrix
	Camera->Matrix->Identity();
	Camera->Matrix->Translate(-Camera->xTranslate, -Camera->yTranslate, -Camera->zTranslate);
	Camera->Matrix->Rotate(-Camera->Pitch, -Camera->Yaw, -Camera->Roll);

	for(Obj = Object; Obj; Obj = Obj->Next)
	{
		// Call external animate rountine
		Obj->SendMessage(OBJ_ANIMATE, NULL, NULL);

		// Obtain primary angle (0 < Angle < 2*M_PI)
		if(Obj->xRotate >		2*M_PI)
			Obj->xRotate -=	2*M_PI;
		if(Obj->xRotate <		0)
			Obj->xRotate +=	2*M_PI;

		if(Obj->yRotate >		2*M_PI)
			Obj->yRotate -=	2*M_PI;
		if(Obj->yRotate <		0)
			Obj->yRotate +=	2*M_PI;

		if(Obj->zRotate >		2*M_PI)
			Obj->zRotate -=	2*M_PI;
		if(Obj->zRotate <		0)
			Obj->zRotate +=	2*M_PI;

		// Compute object matrix
		Matrix->Identity();
		Matrix->Scale(Obj->xScale, Obj->yScale, Obj->zScale);
		Matrix->Rotate(Obj->xRotate, Obj->yRotate, Obj->zRotate);
		Matrix->Translate(Obj->xTranslate, Obj->yTranslate, Obj->zTranslate);

		// Transform object to it's matrix and the camera matrix
		Obj->Transform(Matrix->MainMatrix);
		Obj->TransformToCamera(Camera->Matrix->MainMatrix);

		Obj->CalcDistance();
		if(Obj->Distance < Camera->Yon)
		{
			Obj->Visible = true;
			Obj->Draw();
		}
		else
			Obj->Visible = false;
	}
};
