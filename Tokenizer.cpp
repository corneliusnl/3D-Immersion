// ------------------------------------------------------------
// Lexical Analysis
// Copyright 1997-1998 (c) Leprechaun Software. All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <String.h>
#include <ctype.h>
#include "Include\Error.h"
#include "Include\Tokenizer.h"
#include "Include\Input.h"

//
// Functions
//
char	*Function[] =
{
	"object",				"addvertex",			"addwirepolygon",
	"addsolidpolygon",   "addlambertpolygon",	"addtexturepolygon",
	"setcolor",				"settexture",			"setmusic",
	"mapvertex",			"settransparency",	"addobject",
	"translate",			"rotate",				"scale",
	"turnedon",				"world",					"addcamera",
	"addlight",				"sethither",			"setyon",
	"setambience",			"setinterface",		"setgravity",
	"setbackgroundcolor","setpalette",			"setprojectionplane",
	"setforce",				"setmass",				"setelasticity",
	"setlinearvelocity",						"setlinearacceleration",
	"setrotationalvelocity",				"setrotationalacceleration",
	"setdirection",		"setfriction",
	"\0",
};

// ------------------------------------------------------------
// TOKEN::TOKEN(INPUT Input)
// ------------------------------------------------------------
// Description
//
// Parameters
//
// Returns
//  Nothing
// ------------------------------------------------------------
TOKEN::TOKEN(INPUT *In)
{
	Input = In;
};

// ------------------------------------------------------------
// int TOKEN::Get()
// ------------------------------------------------------------
// Description
//  Reads a token.
//
// Parameters
//  None
//
// Returns
//  Token Type
// ------------------------------------------------------------
int TOKEN::Get()
{
	char	Tmp[256];
	char	ch, *T;
	int	i, Sign = 1, Place;

	ch = Input->Char();

	// Skip Spaces
	while(IsSpace(ch))
		ch = Input->GetChar();

	// Newline characters
	while((ch == '\n') || (ch == '\r'))
		ch = Input->GetChar();

	// Reached end of text, attempt to continue
	// Assuming input will read another chunk of text in
	if(ch == '\0')
	{
		Input->GetChar();
		return(Get());
	}

	//
	// End of File
	//
	if(ch == eofChar)
	{
		return(Code = TOK_EOF);
	}

	//
	// Function
	//
	else if(IsAlpha(ch))
	{
		T = Tmp;
		while(IsAlpha(ch))
		{
			*T++	= tolower(ch);
			ch		= Input->GetChar();
		}
		*T = '\0';
		strcpy(Value.String, Tmp);

		//
		// Function
		//
		for(i = 0; i <= TOK_FUNCEND; i++)
			if(strcmp(Function[i], Tmp) == 0)
			{
				return(Code = i);
			}

		//
		// Boolean
		//
		if(strcmp("true", Tmp) == 0)
		{
			Value.Boolean = true;
			return(Code = TOK_BOOLEAN);
		}
		else if(strcmp("false", Tmp) == 0)
		{
			Value.Boolean = false;
			return(Code = TOK_BOOLEAN);
		}

		return(Code = TOK_NULL);
	}

	//
	// Quoted string
	//
	else if((ch == '\"') || (ch == '\''))
	{
		T	= Tmp;
		ch	= Input->GetChar();

		while((ch != '\"') && (ch != '\'') && (ch != '\0'))
		{
			*T++	= ch;
			ch		= Input->GetChar();
		}
		Input->GetChar();
		*T = '\0';

		strcpy(Value.String, Tmp);
		return(Code = TOK_STRING);
	}

	//
	// Number
	//
	else if((IsDigit(ch)) || (ch == '-'))
	{
		if(ch == '-')
		{
			Sign = -1;
			ch = Input->GetChar();
		}

		Value.Number = 0;
		while(IsDigit(ch))
		{
			Value.Number = (10 * Value.Number) + NumValue(ch);
			ch		= Input->GetChar();
		}

		if(ch == '.')
		{
			Place = 10;
			ch = Input->GetChar();
			while(IsDigit(ch))
			{
				Value.Number += ((float)NumValue(ch) / Place);
				Place *= 10;
				ch = Input->GetChar();
			}
		}

		Value.Number *= Sign;
		return(Code = TOK_NUMBER);
	}

	//
	// Comment
	//
	else if(ch == ';')
	{
		while(Input->GetChar() != '\0');
		Input->GetChar();
		return(Get());
	}

	//
	// Left Parenthesis
	//
	else if(ch == '(')
	{
		Input->GetChar();
		return(Code = TOK_LPAREN);
	}

	//
	// Right Parenthesis
	//
	else if(ch == ')')
	{
		Input->GetChar();
		return(Code = TOK_RPAREN);
	}

	//
	// Left Curly Brace
	//
	else if(ch == '{')
	{
		Input->GetChar();
		return(Code = TOK_LCB);
	}

	//
	// Right Curly Brace
	//
	else if(ch == '}')
	{
		Input->GetChar();
		return(Code = TOK_RCB);
	}

	//
	// Comma
	//
	else if(ch == ',')
	{
		Input->GetChar();
		return(Code = TOK_COMMA);
	}

	//
	// Invalid Token
	//
	else
	{
		return(Code = TOK_NULL);
	}
};

// ------------------------------------------------------------
// TOKEN::Get(int TokCode)
// ------------------------------------------------------------
// Description
//  Gets the subsequent token, however, it this isn't the
// tokem that was expected it prints a error message.
//
// Parameters
//  TokCode			- Token code that is expected
//
// Returns
//  If successful ? 1 : 0
// ------------------------------------------------------------
int TOKEN::Get(int TokCode)
{
	if(Get() == TokCode)
		return Code;
	else
		Warning(TokCode, Input);
};
