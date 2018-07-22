#ifndef IMMERSION_TOKENIZER_H
#define IMMERSION_TOKENIZER_H
// ------------------------------------------------------------
// Tokenzier
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include "Include\Input.h"

//
// Value Structure
//
typedef struct
{
	float	Number;
	bool	Boolean;
	char	String[256];
} VALUE;

//
// Token Type Definitions
//
enum TokenType
{
	TOK_OBJECT,					TOK_ADDVERTEX,				TOK_ADDWIREPOLYGON,
	TOK_ADDSOLIDPOLYGON,		TOK_ADDLAMBERTPOLYGON,	TOK_ADDTEXTUREPOLYGON,
	TOK_SETCOLOR,				TOK_SETTEXTURE,			TOK_SETMUSIC,
	TOK_MAPVERTEX,				TOK_SETTRANSPARENCY,		TOK_ADDOBJECT,
	TOK_TRANSLATE,				TOK_ROTATE,					TOK_SCALE,
	TOK_TURNEDON,				TOK_WORLD,					TOK_ADDCAMERA,
	TOK_ADDLIGHT,				TOK_SETHITHER,				TOK_SETYON,
	TOK_SETAMBIENCE,			TOK_SETINTERFACE,			TOK_SETGRAVITY,
	TOK_SETBACKGROUNDCOLOR, TOK_SETPALETTE,			TOK_SETPROJECTIONPLANE,
	TOK_SETFORCE,				TOK_SETMASS,				TOK_SETELASTICITY,
	TOK_SETLINEARVELOCITY,				TOK_SETLINEARACCELERATION,
	TOK_SETROTATIONALVELOCITY,			TOK_SETROTATIONALACCELERATION,
	TOK_SETDIRECTION,			TOK_SETFRICTION,

	TOK_FUNCEND,
	
	TOK_NULL,					TOK_NUMBER,					TOK_BOOLEAN,
	TOK_STRING,					TOK_COMMA,					TOK_LPAREN,
	TOK_RPAREN,					TOK_LCB,						TOK_RCB,
	TOK_EOF,          		TOK_IDENTIFIER,
};

#define IsDigit(A)	((((A) >= '0') && ((A) <= '9')) ? 1 : 0)
#define IsAlpha(A)	(((((A) >= 'a') && ((A) <= 'z')) || (((A) >= 'A') && ((A) <= 'Z'))) ? 1 : 0)
#define IsSpace(A)	((((A) == ' ') || ((A) == '\t')) ? 1 : 0)
#define NumValue(A)	((A) - '0')

//
// Tokenizer Class
//
class TOKEN
{
private:
	INPUT	*Input;

public:
	int	Code;
	VALUE	Value;

	// Constructor/Destructor
	TOKEN(INPUT *Input);

	// Get Token Routines
	int	Get();
	int	Get(int TokenCode);
};

#endif
