// ------------------------------------------------------------
// Immersion Error Handler
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\Input.h"
#include "Include\Error.h"

//
// Error Strings
//
char	*WarningMsg[] =
{
	"Expecting OBJECT!",
	"Expecting ADDVERTEX!",
	"Expecting ADDWIREPOLYGON!",
	"Expecting ADDSOLIDPOLYGON!",
	"Expecting ADDLAMBERTPOLYGON!",
	"Expecting ADDTEXTUREPOLYGON!",
	"Expecting SETCOLOR!",
	"Expecting SETTEXTURE!",
	"Expecting SETMUSIC!",
	"Expecting MAPVERTEX!",
	"Expecting SETTRANSPARENCY!",
	"Expecting ADDOBJECT!",
	"Expecting TRANSLATE!",
	"Expecting ROTATE!",
	"Expecting SCALE!",
	"Expecting TURNEDON!",
	"Expecting WORLD!",
	"Expecting ADDCAMERA!",
	"Expecting ADDLIGHT!",
	"Expecting SETHITHER!",
	"Expecting SETYON!",
	"Expecting SETAMBIENCE!",
	"Expecting SETINTERFACE!",
	"Expecting SETGRAVITY!",
	"Expecting SETBACKGROUNDCOLOR!",
	"Expecting SETPALETTE!",
	"Expecting SETPROJECTIONPLANE!",
	"Expecting SETFORCE!",
	"Expecting SETMASS!",
	"Expecting SETELASTICITY!",
	"Expecting SETLINEARVELOCTY!",
	"Expecting SETLINEARACCELERATION!",
	"Expecting SETROTATIONALVELOCITY!",
	"Expecting SETROTATIONALACCELERATION!",
	"Expecting SETDIRECTION!",
	"Expecting SETFRICTION!",
	"Expecting... IMPOSSIBLE!",
	"Expecting NOTHING!",
	"Expecting NUMBER!",
	"Expecting STRING!",
	"Expecting COMMA!",
	"Expecting LEFT PARENTHESIS!",
	"Expecting RIGHT PARENTHESIS!",
	"Expecting LEFT CURLY BRACKET!",
	"Expecting RIGHT CURLY BRACKET!",
	"Expecting END OF FILE!",
	"Expecting IDENTIFIER!",
};

// ------------------------------------------------------------
// void Warning(int WarnCode, INPUT *Input)
// ------------------------------------------------------------
// Description
//  Reports an error to the user but does not abort
//
// Parameters
//  WarnCode		- Waring message index
//  Input			- Handle of input with errors
//
// Returns
//  Nothing
// ------------------------------------------------------------
void Warning(int WarnCode, INPUT *Input)
{
	char	Buff[250];

	if(WarnCode == WARN_SYNTAX)
	{
		wsprintf(Buff, "%s\nPotentially hazardous syntax!",
			Input->Print());
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(GetFocus(), Buff, "Syntax error encountered!", MB_OK);
	}
	else
	{
		wsprintf(Buff, "%s\nWarning:\t %s",
			Input->Print(),
			WarningMsg[WarnCode]);
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(GetFocus(), Buff, "Syntax error encountered!", MB_OK);
	}
};
