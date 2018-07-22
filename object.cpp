// ------------------------------------------------------------
// Object Description File Format Support
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Math.h>
#include "Include\Interface.h"
#include "Include\Mathematics.h"
#include "Include\Dib.h"
#include "Include\Error.h"
#include "Include\Object.h"

int	VertIdx;
int	TextIdx;

// ------------------------------------------------------------
// OBJECT::OBJECT(WORLD *World, char *Filename)
// ------------------------------------------------------------
// Description
//  Loads the specified object file from disk.
//
// Parameters
//  Filename		- Filename of the object definition file
//
// Returns
//  Initialized class
// ------------------------------------------------------------
OBJECT::OBJECT(WORLD *World, char *Filename)
{
	// Allocate input and tokenizer
	Input			= new FILEINPUT(Filename);
	Token			= new TOKEN(Input);
	Next			= NULL;
	this->World = World;

	// Set Defaults
	strcpy(this->Filename, Filename);
	strcpy(Interface, "<Undefined>");
	NumVertices		= 0;
	NumPolygons		= 0;
	hLibrary			= NULL;
	ObjectProc		= NULL;
	Polygon			= NULL;
	xScale			= 1;
	yScale			= 1;
	zScale			= 1;
	xRotate			= 0;
	yRotate			= 0;
	zRotate			= 0;
	xTranslate		= 0;
	yTranslate		= 0;
	zTranslate		= 0;
	Mass				= 0;
	Force				= 0;
	LinearVelocity.xComp				= 0;
	LinearVelocity.yComp				= 0;
	LinearVelocity.zComp				= 0;
	LinearAcceleration.xComp		= 0;
	LinearAcceleration.yComp		= 0;
	LinearAcceleration.zComp		= 0;
	RotationalVelocity.xComp		= 0;
	RotationalVelocity.yComp		= 0;
	RotationalVelocity.zComp		= 0;
	RotationalAcceleration.xComp	= 0;
	RotationalAcceleration.yComp	= 0;
	RotationalAcceleration.zComp	= 0;

	//
	// Parse File
	//
	while(Token->Get() != TOK_EOF)
	{
		switch(Token->Code)
		{
			case TOK_OBJECT:
				ParseObject();
				break;

			case TOK_NULL:
				break;

			default:
				Warning(WARN_SYNTAX, Input);
				break;
		}
	}

	//
	// Check for possible incorrectly specified number of vertices
	//
	if(VertIdx < NumVertices)
	{
		MessageBox(GetFocus(), "Number of vertices read are less than the number allocated!",
			"Syntax Error", MB_OK);
		NumVertices = VertIdx;
	}

	// Free tokenizer and input
	delete Token;
	delete Input;
};

// ------------------------------------------------------------
// OBJECT::~OBJECT()
// ------------------------------------------------------------
// Description
//  Class destruction
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
OBJECT::~OBJECT()
{
	//
	// Free DynaLib must be freed first
	//
	if(hLibrary)
	{
		SendMessage(OBJ_DESTROY, 0, 0L);
		FreeLibrary(hLibrary);
	}

	// Delete polygons
	delete	Polygon;
	Polygon	= NULL;

	// Free Global Verticies
	delete[]	Vertex;

	if(Next)
	{
		delete	Next;
		Next		= NULL;
	}
};

// ------------------------------------------------------------
// void OBJECT::ParseObject()
// ------------------------------------------------------------
// Description
//  Parses the object command
//
// Parameter
//  None
//
// Returns
//  Nothign
// ------------------------------------------------------------
void OBJECT::ParseObject()
{
	//
	// Parameters
	//
	Token->Get(TOK_LPAREN);
	{
		// Number of verticies
		Token->Get(TOK_NUMBER);
		NumVertices = Token->Value.Number;
		Vertex		= new VERTEX[NumVertices];
		VertIdx		= 0;
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
				case TOK_ADDVERTEX:
					ParseVertex();
					break;

				case TOK_ADDWIREPOLYGON:
					ParsePolygon(POLY_WIRE);
					break;

				case TOK_ADDSOLIDPOLYGON:
					ParsePolygon(POLY_SOLID);
					break;

				case TOK_ADDLAMBERTPOLYGON:
					ParsePolygon(POLY_LAMBERT);
					break;

				case TOK_ADDTEXTUREPOLYGON:
					ParsePolygon(POLY_TEXTURE);
					break;

				case TOK_NULL:
					break;

				default:
					Warning(WARN_SYNTAX, Input);
					break;
			}
		}
	}

	SendMessage(OBJ_CREATE, 0, 0L);
};

// ------------------------------------------------------------
// void OBJECT::ParseVertex()
// ------------------------------------------------------------
// Description
//  Parses a vertex command
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::ParseVertex()
{
	//
	// Check for possible incorrectly specified number of vertices
	//
	if(VertIdx >= NumVertices)
	{
		MessageBox(GetFocus(), "Number of vertices exceed those that were allocated!",
			"Syntax Error", MB_OK);
		return;
	}

	//
	// Parameters
	//
	Token->Get(TOK_LPAREN);
	{
		// X coordinate
		Token->Get(TOK_NUMBER);
		Vertex[VertIdx].xObject	= Token->Value.Number;

		// Y coordinate
		Token->Get(TOK_COMMA);
		Token->Get(TOK_NUMBER);
		Vertex[VertIdx].yObject = Token->Value.Number;

		// Z coordinate
		Token->Get(TOK_COMMA);
		Token->Get(TOK_NUMBER);
		Vertex[VertIdx].zObject = Token->Value.Number;
	}
	Token->Get(TOK_RPAREN);

	VertIdx++;
};

// ------------------------------------------------------------
// void OBJECT::ParsePolygon(int Type)
// ------------------------------------------------------------
// Description
//  Parses a polygon command
//
// Parameters
//  Type			- Polygon type
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::ParsePolygon(int Type)
{
	POLYGON	*TmpPoly;

	//
	// Parameters
	//
	Token->Get(TOK_LPAREN);
	{
		// Number of vertices
		Token->Get(TOK_NUMBER);
		TmpPoly = new POLYGON(World, Token->Value.Number);
		TmpPoly->Type = Type;

		// Vertex indexies
		for(int i = 0; i < TmpPoly->NumVertices; i++)
		{
			// Note: must alter to ensure no overflow as vertex
			//  index might be invalid
			Token->Get(TOK_COMMA);
			Token->Get(TOK_NUMBER);
			TmpPoly->Vertex[i] = &Vertex[(int)Token->Value.Number];
		}
	}
	Token->Get(TOK_RPAREN);

	//
	// Code Block
	//
	Token->Get(TOK_LCB);
	while(Token->Get() != TOK_RCB)
	{
		switch(Token->Code)
		{
			case TOK_SETCOLOR:
				Token->Get(TOK_LPAREN);

				// Red component
				Token->Get(TOK_NUMBER);
				TmpPoly->Color.rgbRed = Token->Value.Number;

				// Green component
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				TmpPoly->Color.rgbGreen = Token->Value.Number;

				// Blue component
				Token->Get(TOK_COMMA);
				Token->Get(TOK_NUMBER);
				TmpPoly->Color.rgbBlue = Token->Value.Number;

				Token->Get(TOK_RPAREN);
				break;

			case TOK_SETTEXTURE:
				Token->Get(TOK_LPAREN);

				// Filename
				Token->Get(TOK_STRING);
				TmpPoly->Texture = new DIB(Token->Value.String);
				TextIdx = 0;

				Token->Get(TOK_RPAREN);

				//
				// Code Block
				//
				Token->Get(TOK_LCB);
				while(Token->Get() != TOK_RCB)
				{
					switch(Token->Code)
					{
						case TOK_MAPVERTEX:
							Token->Get(TOK_LPAREN);

							// X Coordinate
							Token->Get(TOK_NUMBER);
							TmpPoly->TextPt[TextIdx].x = Token->Value.Number;

							// Y Coordinate
							Token->Get(TOK_COMMA);
							Token->Get(TOK_NUMBER);
							TmpPoly->TextPt[TextIdx].y = Token->Value.Number;

							Token->Get(TOK_RPAREN);
							TextIdx++;
							break;

						case TOK_SETTRANSPARENCY:
							Token->Get(TOK_LPAREN);

							// Transparency Color
							Token->Get(TOK_NUMBER);
							TmpPoly->Type			= POLY_TRANSTEXTURE;
							TmpPoly->TransColor	= Token->Value.Number;

							Token->Get(TOK_RPAREN);
							break;

						case TOK_NULL:
							break;

						default:
							Warning(WARN_SYNTAX, Input);
							break;
					}
				}
				break;

			case TOK_NULL:
				break;

			default:
				Warning(WARN_SYNTAX, Input);
				break;
		}
	}
	// Place polygon at head of linked list of polygons
	TmpPoly->Prev	= NULL;
	TmpPoly->Next	= Polygon;
	Polygon			= TmpPoly;
	NumPolygons++;
};

// ------------------------------------------------------------
// long OBJECT::SendMessage(unsigned int Message, WPARAM wParam, LPARAM lParam)
// ------------------------------------------------------------
// Description
//  Sends a message to the object procedure handler.
//
// Parameters
//  Message		- Message code
//  wParam		- Word parameter
//  lParam		- Long parameter
//
// Returns
//  Long value
// ------------------------------------------------------------
long OBJECT::SendMessage(unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	if(ObjectProc)
	{
		switch(Message)
		{
			case OBJ_CREATE:
				ObjectProc(World, this, Message, wParam, lParam);
				break;

			case OBJ_ANIMATE:
				ObjectProc(World, this, Message, wParam, lParam);
				break;

			case OBJ_MOUSEDOWN:
				ObjectProc(World, this, Message, wParam, lParam);
				break;

			case OBJ_MOUSEMOVE:
				ObjectProc(World, this, Message, wParam, lParam);
				break;

			case OBJ_MOUSEUP:
				ObjectProc(World, this, Message, wParam, lParam);
				break;

			case OBJ_DESTROY:
				ObjectProc(World, this, Message, wParam, lParam);
				break;
		}
	}
};

// ------------------------------------------------------------
// void OBJECT::SetAlias(char *Alias)
// ------------------------------------------------------------
// Description
//  Specifies the object's alias
//
// Parameters
//  Alias		- Identifier of the object
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetAlias(char *Alias)
{
	strcpy(Identifier, Alias);
};

// ------------------------------------------------------------
// void OBJECT::SetFriction(float Friction)
// ------------------------------------------------------------
// Description
//  Sets the objects friction value
//
// Parameters
//  Friction	- Friction value
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetFriction(float Friction)
{
	this->Friction = Friction;
};

// ------------------------------------------------------------
// void OBJECT::SetForce(float Force)
// ------------------------------------------------------------
// Description
//  Sets the amount of force acting upon an object
//
// Parameters
//  Force		- Force acting upon object
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetForce(float Force)
{
	this->Force = Force;
};

// ------------------------------------------------------------
// void OBJECT::SetMass(float Mass)
// ------------------------------------------------------------
// Description
//  Sets the objects mass
//
// Parameters
//  Mass	- Mass value
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetMass(float Mass)
{
	this->Mass = Mass;
};

// ------------------------------------------------------------
// void OBJECT::SetElasticity(float Elasticity)
// ------------------------------------------------------------
// Description
//  Sets the objects elasticity value
//
// Parameters
//  Elasticity		- Elasticity value
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetElasticity(float Elasticity)
{
	this->Elasticity = Elasticity;
};

// ------------------------------------------------------------
// void OBJECT::SetLinearAcceleration(float X, float Y, float Z)
// ------------------------------------------------------------
// Description
//  Sets the objects linear acceleration
//
// Parameters
//  X				- X component
//  Y 			- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetLinearAcceleration(float X, float Y, float Z)
{
	LinearAcceleration.xComp = X;
	LinearAcceleration.yComp = Y;
	LinearAcceleration.zComp = Z;
};

// ------------------------------------------------------------
// void OBJECT::SetLinearVelocity(float X, float Y, float Z)
// ------------------------------------------------------------
// Description
//  Sets the objects linear velocity
//
// Parameters
//  X				- X component
//  Y 			- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetLinearVelocity(float X, float Y, float Z)
{
	LinearVelocity.xComp = X;
	LinearVelocity.yComp = Y;
	LinearVelocity.zComp = Z;
};

// ------------------------------------------------------------
// void OBJECT::SetRotationalAcceleration(float X, float Y, float Z)
// ------------------------------------------------------------
// Description
//  Sets the objects rotational acceleration
//
// Parameters
//  X				- X component
//  Y 			- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetRotationalAcceleration(float X, float Y, float Z)
{
	RotationalAcceleration.xComp = X;
	RotationalAcceleration.yComp = Y;
	RotationalAcceleration.zComp = Z;
};

// ------------------------------------------------------------
// void OBJECT::SetRotationalVelocity(float X, float Y, float Z)
// ------------------------------------------------------------
// Description
//  Sets the objects rotational velocity
//
// Parameters
//  X				- X component
//  Y 			- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetRotationalVelocity(float X, float Y, float Z)
{
	RotationalVelocity.xComp = X;
	RotationalVelocity.yComp = Y;
	RotationalVelocity.zComp = Z;
};

// ------------------------------------------------------------
// void OBJECT::Translate(float X, float Y, float Z)
// ------------------------------------------------------------
// Description
//  Translates the object
//
// Parameters
//  X				- X component
//  Y 			- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::Translate(float X, float Y, float Z)
{
	xTranslate = X;
	yTranslate = Y;
	zTranslate = Z;
};

// ------------------------------------------------------------
// void OBJECT::Rotate(float X, float Y, float Z)
// ------------------------------------------------------------
// Description
//  Rotates the object
//
// Parameters
//  X				- X component
//  Y 			- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::Rotate(float X, float Y, float Z)
{
	xRotate	= X;
	yRotate	= Y;
	zRotate	= Z;
};

// ------------------------------------------------------------
// void OBJECT::Scale(float X, float Y, float Z)
// ------------------------------------------------------------
// Description
//  Scales the given object the amound specified for each axis
//
// Parameters
//  X				- X component
//  Y 			- Y component
//  Z				- Z component
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::Scale(float X, float Y, float Z)
{
	xScale	= X;
	yScale	= Y;
	zScale	= Z;
};

// ------------------------------------------------------------
// bool OBJECT::ContainsPoint(int x, int y)
// ------------------------------------------------------------
// Description
//  Determines whether or not the given point is mapped to
// one of the objects polygons.
//
// Parameters
//  x				- X coordinate of point
//  y				- Y coordinate of point
//
// Returns
//  If indeed ? true : false
// ------------------------------------------------------------
bool OBJECT::ContainsPoint(int x, int y)
{
	POLYGON	*Poly;

	for(Poly = Polygon; Poly; Poly = Poly->Next)
		if(Poly->ContainsPoint(x, y))
			return true;

	return false;
};

// ------------------------------------------------------------
// void OBJECT::Draw()
// ------------------------------------------------------------
// Description
//  Draws the object by drawing the viewable polygons.
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::Draw()
{
	POLYGON	*Poly;

	for(Poly = Polygon; Poly; Poly = Poly->Next)
	{
		// Don't draw polygon facing away from camera
		if(Poly->FacesCamera())
		{
			// Draw Polygon
			Poly->ClipZ();
			Poly->Project();
			Poly->ClipXY();
			if(Poly->Clip.NumVertices > 2)
				Poly->Draw();
		}
	}
};

// ------------------------------------------------------------
// void OBJECT::SetInterface(char *Filename)
// ------------------------------------------------------------
// Description
//  Sets the object's procedure handler
//
// Parameters
//  Filename		- Dynalib filename
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::SetInterface(char *Filename)
{
	char		Buff[256];

	//
	// Free Previous DynaLib From Memory
	//
	if(hLibrary != NULL)
	{
//		SendMessage(OBJ_DESTROY, 0, 0L);
		FreeLibrary(hLibrary);
	}

	//
	// Load New DynaLib Into Memory
	//
	strcpy(Interface, Filename);
	if((hLibrary = LoadLibrary(Interface)))
	{
		ObjectProc	= (OBJECTPROC)GetProcAddress(hLibrary, "ObjectProc");
//		SendMessage(OBJ_CREATE, 0, 0L);
	}
	else
	{
		wsprintf(Buff, "Unable to load %s!", Filename);
		MessageBox(GetFocus(), Buff, "Actung!", MB_OK);
		ObjectProc	= NULL;
	}
};

// ------------------------------------------------------------
// long OBJECT::CalcDistance()
// ------------------------------------------------------------
// Description
//  Calculates the distance of the polygon from the camera
//
// Parameters
//  None
//
// Returns
//  Distance
// ------------------------------------------------------------
long OBJECT::CalcDistance()
{
	POLYGON	*Poly;

	Distance = 0L;
	for(Poly = Polygon; Poly; Poly = Poly->Next)
		Distance += Poly->CalcDistance();

	return(Distance /= NumPolygons);
};

// ------------------------------------------------------------
// void OBJECT::CalcBoundingBox()
// ------------------------------------------------------------
// Description
//  Calculates the objects bounding box
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::CalcBoundingBox()
{
	MinX = MaxX = Vertex[0].xWorld;
	MinY = MaxY = Vertex[0].yWorld;
	MinZ = MaxZ = Vertex[0].zWorld;

	for(int i = 1; i < NumVertices; i++)
	{
		if(Vertex[i].xWorld < MinX) MinX = Vertex[i].xWorld;
		if(Vertex[i].xWorld > MaxX) MaxX = Vertex[i].xWorld;
		if(Vertex[i].yWorld < MinY) MinY = Vertex[i].yWorld;
		if(Vertex[i].yWorld > MaxY) MaxY = Vertex[i].yWorld;
		if(Vertex[i].zWorld < MinZ) MinZ = Vertex[i].zWorld;
		if(Vertex[i].zWorld > MaxZ) MaxZ = Vertex[i].zWorld;
	}
};

// ------------------------------------------------------------
// void OBJECT::CalcBoundingSphere()
// ------------------------------------------------------------
// Description
//  Calculates the objects bounding sphere
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::CalcBoundingSphere()
{
	long	r = 0;

	// Compute the center of the object
	Sphere.X = (MaxX - MinX) / 2 + MinX;
	Sphere.Y = (MaxY - MinY) / 2 + MinY;
	Sphere.Z = (MaxZ - MinZ) / 2 + MinZ;

	// Computer point farest away from center
	for(int i = 0; i < NumVertices; i++)
	{
		r = sqrt(((Vertex[i].xWorld - Sphere.X) * (Vertex[i].xWorld - Sphere.X))
				+	((Vertex[i].yWorld - Sphere.Y) * (Vertex[i].yWorld - Sphere.Y))
				+	((Vertex[i].zWorld - Sphere.Z) * (Vertex[i].zWorld - Sphere.Z)));

		if (r > Sphere.Radius)
			Sphere.Radius = r;
	}
};

// ------------------------------------------------------------
// void OBJECT::Transform(MATRIX4x4 Matrix)
// ------------------------------------------------------------
// Description
//  Transforms the object to the matrix.
//
// Parameters
//  Matrix			- Matrix to be transformed by.
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::Transform(MATRIX4x4 Matrix)
{
	for(int i = 0; i < NumVertices; i++)
	{
		Vertex[i].xWorld =	(Vertex[i].xObject * (*(Matrix.ElemPtr))		) +
									(Vertex[i].yObject * (*(Matrix.ElemPtr + 4))	) +
									(Vertex[i].zObject * (*(Matrix.ElemPtr + 8))	) +
									(*(Matrix.ElemPtr + 12));

		Vertex[i].yWorld =	(Vertex[i].xObject * (*(Matrix.ElemPtr + 1))	) +
									(Vertex[i].yObject * (*(Matrix.ElemPtr + 5))	) +
									(Vertex[i].zObject * (*(Matrix.ElemPtr + 9))	) +
									(*(Matrix.ElemPtr + 13));

		Vertex[i].zWorld =	(Vertex[i].xObject * (*(Matrix.ElemPtr + 2))	) +
									(Vertex[i].yObject * (*(Matrix.ElemPtr + 6))	) +
									(Vertex[i].zObject * (*(Matrix.ElemPtr + 10))) +
									(*(Matrix.ElemPtr + 14));
	}
};

// ------------------------------------------------------------
// void OBJECT::TransformToCamera(MATRIX4x4 Matrix)
// ------------------------------------------------------------
// Description
//  Transforms the object to the matrix.
//
// Parameters
//  Matrix			- Matrix to be transformed by.
//
// Returns
//  Nothing
// ------------------------------------------------------------
void OBJECT::TransformToCamera(MATRIX4x4 Matrix)
{
	for(int i = 0; i < NumVertices; i++)
	{
		Vertex[i].xCamera =	(Vertex[i].xWorld * (*(Matrix.ElemPtr))		) +
									(Vertex[i].yWorld * (*(Matrix.ElemPtr + 4))	) +
									(Vertex[i].zWorld * (*(Matrix.ElemPtr + 8))	) +
									(*(Matrix.ElemPtr + 12));

		Vertex[i].yCamera =	(Vertex[i].xWorld * (*(Matrix.ElemPtr + 1))	) +
									(Vertex[i].yWorld * (*(Matrix.ElemPtr + 5))	) +
									(Vertex[i].zWorld * (*(Matrix.ElemPtr + 9))	) +
									(*(Matrix.ElemPtr + 13));

		Vertex[i].zCamera =	(Vertex[i].xWorld * (*(Matrix.ElemPtr + 2))	) +
									(Vertex[i].yWorld * (*(Matrix.ElemPtr + 6))	) +
									(Vertex[i].zWorld * (*(Matrix.ElemPtr + 10))) +
									(*(Matrix.ElemPtr + 14));
	}
};
