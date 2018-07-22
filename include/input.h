#ifndef IMMERSION_INPUT_H
#define IMMERSION_INPUT_H
// ------------------------------------------------------------
// File Input
// Copyright 1997 (c) Leprechaun Software. All Rights Reserved.
// ------------------------------------------------------------
#include <StdIO.h>

//
// Externals
//
extern char eofChar;

//
// Input Abstract Class
//
class INPUT
{
protected:
	char	Text[300];
	char	*pText;

public:

	// Text Functions
	virtual char	GetChar() = 0;
	virtual char	Char() = 0;
	virtual char	*Print() = 0;
};

//
// File Input Class
//
class FILEINPUT : public INPUT
{
private:
	FILE	*Input;
	char	Filename[256];
	int	CurrLine;

	char	GetLine();

public:

	// Constructor/Destructor
	FILEINPUT(char *Filename);
	~FILEINPUT();

	// Methods
	virtual char	GetChar();
	virtual char	Char();
	virtual char	*Print();
};

//
// String Input Class
//
class STRINGINPUT : public INPUT
{
public:

	// Constructor/Destructor
	STRINGINPUT(char *String);
	~STRINGINPUT();

	// Methods
	virtual char	GetChar();
	virtual char	Char();
	virtual char	*Print();
};

#endif
