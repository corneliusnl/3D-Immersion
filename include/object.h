#ifndef IMMERSION_OBJECT_H
#define IMMERSION_OBJECT_H
// ------------------------------------------------------------
// Object Description File Format Support
// Copyright 1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include "Include\Input.h"
#include "Include\Tokenizer.h"
#include "Include\Types.h"
#include "Include\Polygon.h"
#include "Include\World.h"

class WORLD;
class OBJECT;
class POLYGON;

typedef long (CALLBACK *OBJECTPROC)	(WORLD *World, OBJECT *Object, unsigned int Message, WPARAM wParam, LPARAM lParam);

//
// Object Description File Class
//
class OBJECT
{
private:
	OBJECTPROC	ObjectProc;
	HANDLE		hLibrary;
	FILEINPUT	*Input;
	TOKEN			*Token;
	WORLD			*World;

public:
	OBJECT	*Next, *Prev;
	float		xScale, yScale, zScale;
	float		xRotate, yRotate, zRotate;
	float		xTranslate, yTranslate, zTranslate;

	VECTOR	LinearAcceleration;
	VECTOR	LinearVelocity;
	VECTOR	RotationalAcceleration;
	VECTOR	RotationalVelocity;
	float		Mass;
	float		Elasticity;
	float		Force;
	float		Friction;
	long		MinX, MaxX, MinY, MaxY, MinZ, MaxZ;
	bool		Visible;
	struct
	{
		long	X, Y, Z;
		long	Radius;
	} Sphere;
	VERTEX	*Vertex;
	int		NumVertices;

	POLYGON	*Polygon;
	int		NumPolygons;

	char		Identifier[256];
	char		Filename[256];
	char		Interface[256];
	long		Distance;

	// Construction/Destruction
	OBJECT(WORLD *World, char *Filename);
	~OBJECT();

	void	ParseObject();
	void	ParseDynaLib();
	void	ParseVertex();
	void	ParsePolygon(int Type);

	// Exposed functions
	void	SetAlias(char *Alias);
	void	SetInterface(char *Filename);
	void	SetFriction(float Friction);
	void	SetForce(float Force);
	void	SetLinearAcceleration(float X, float Y, float Z);
	void	SetLinearVelocity(float X, float Y, float Z);
	void	SetRotationalAcceleration(float X, float Y, float Z);
	void	SetRotationalVelocity(float X, float Y, float Z);
	void	SetMass(float Mass);
	void	SetElasticity(float Elasticity);
	void	Translate(float X, float Y, float Z);
	void	Rotate(float X, float Y, float Z);
	void	Scale(float X, float Y, float Z);

	// Dynamic library routines
	long	SendMessage(unsigned int Message, WPARAM wParam, LPARAM lParam);

	// Object functions
	bool	ContainsPoint(int x, int y);
	void	Draw();
	void	SortPolygons();
	long	CalcDistance();
	void	CalcBoundingBox();
	void	CalcBoundingSphere();
	void	Transform(MATRIX4x4 Matrix);
	void	TransformToCamera(MATRIX4x4 Matrix);
};

#endif
