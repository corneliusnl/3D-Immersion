// ------------------------------------------------------------
// Matrix
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Math.h>
#include <Mem.h>
#include <Stdio.h>
#include "Include\Mathematics.h"
#include "Include\Matrix.h"

// ------------------------------------------------------------
// MATRIX::MATRIX()
// ------------------------------------------------------------
// Description
//  Class initialization
//
// Parameters
//  None
//
// Returns
//  Initialized class
// ------------------------------------------------------------
MATRIX::MATRIX()
{
	MainMatrix.ElemPtr 	= *MainMatrix.Element;
	TempMatrix.ElemPtr 	= *TempMatrix.Element;
	UsageMatrix.ElemPtr 	= *UsageMatrix.Element;

	Identity();
};

// ------------------------------------------------------------
// void MATRIX::Identity()
// ------------------------------------------------------------
// Description
//  Creates an identity matrix for the main matrix
//
// Parameters
//  None
//
// Returns
//  Nothing
// ------------------------------------------------------------
void MATRIX::Identity()
{
	memset(MainMatrix.ElemPtr, 0, sizeof(float) * 15);
	*(MainMatrix.ElemPtr)       = *(MainMatrix.ElemPtr + 5)   =
	*(MainMatrix.ElemPtr + 10)  = *(MainMatrix.ElemPtr + 15)  = 1;
};

// ------------------------------------------------------------
// void MATRIX::Identity(float *MatPtr)
// ------------------------------------------------------------
// Description
//  Creates an identity matrix
//
// Parameters
//  MatPtr			- Matrix to be identified
//
// Returns
//  Nothing
// ------------------------------------------------------------
void MATRIX::Identity(float *MatPtr)
{
	memset(MatPtr, 0, sizeof(float) * 15);
	*(MatPtr)       = *(MatPtr + 5)   =
	*(MatPtr + 10)  = *(MatPtr + 15)  = 1;
};

// ------------------------------------------------------------
// void MATRIX::Multiply(float *Dest, float *ptr1, float *ptr2)
// ------------------------------------------------------------
// Description
//  Multiplys two matricies and puts the result in another
// matrix.
//
// Parameters
//  Dest				- Destination matrix
//  ptr1				- Matrix 1
//  ptr2				- Matrix 2
//
// Returns
//  Nothing
// ------------------------------------------------------------
void MATRIX::Multiply(float *Dest, float *ptr1, float *ptr2)
{
	int 	t;

	for(int i = 0; i < 4; i++)
	{
		t = i * 4;
		*(Dest + t) =
			( (*(ptr1 + t))     * (*(ptr2))     	)
		+	( (*(ptr1 + t + 1)) * (*(ptr2 + 4)) 	)
		+	( (*(ptr1 + t + 2)) * (*(ptr2 + 8)) 	)
		+	( (*(ptr1 + t + 3)) * (*(ptr2 + 12))	);

		t = i * 4;
		*(Dest + t + 1) =
			( (*(ptr1 + t))     * (*(ptr2 + 1))   	)
		+	( (*(ptr1 + t + 1)) * (*(ptr2 + 5)) 	)
		+	( (*(ptr1 + t + 2)) * (*(ptr2 + 9)) 	)
		+	( (*(ptr1 + t + 3)) * (*(ptr2 + 13))	);

		t = i * 4;
		*(Dest + t + 2) =
			( (*(ptr1 + t))     * (*(ptr2 + 2))   	)
		+	( (*(ptr1 + t + 1)) * (*(ptr2 + 6)) 	)
		+	( (*(ptr1 + t + 2)) * (*(ptr2 + 10)) 	)
		+	( (*(ptr1 + t + 3)) * (*(ptr2 + 14))	);

		t = i * 4;
		*(Dest + t + 3) =
			( (*(ptr1 + t))     * (*(ptr2 + 3))   	)
		+	( (*(ptr1 + t + 1)) * (*(ptr2 + 7)) 	)
		+	( (*(ptr1 + t + 2)) * (*(ptr2 + 11)) 	)
		+	( (*(ptr1 + t + 3)) * (*(ptr2 + 15))	);
	}
};

// ------------------------------------------------------------
// void MATRIX::Copy(float *dest, float *src)
// ------------------------------------------------------------
// Description
//  Copys the src matrix into the destination matrix
//
// Parameters
//  dest				- Destination matrix
//  src				- Source matrix
//
// Returns
//  Nothing
// ------------------------------------------------------------
void MATRIX::Copy(float *dest, float *src)
{
	memcpy(dest, src, sizeof(float) * 16);
};

// ------------------------------------------------------------
// void MATRIX::Scale(float sx, float sy, float sz)
// ------------------------------------------------------------
// Description
//  Sets the matrix elements which control scaling
//
// Parameters
//  sx				- Amount to be scaled on x-axis
//  sy				- Amount to be scaled on y-axis
//  sz				- Amount to be scaled on z-axis
//
// Returns
//  Nothing
// ------------------------------------------------------------
void MATRIX::Scale(float sx, float sy, float sz)
{
	Identity(UsageMatrix.ElemPtr);
	*(UsageMatrix.ElemPtr)      = (float)sx;
	*(UsageMatrix.ElemPtr + 5)  = (float)sy;
	*(UsageMatrix.ElemPtr + 10) = (float)sz;
	*(UsageMatrix.ElemPtr + 15) = (float)1;

	Multiply(TempMatrix.ElemPtr, UsageMatrix.ElemPtr, MainMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);
};

// ------------------------------------------------------------
// void MATRIX::Rotate(float NX, float NY, float NZ)
// ------------------------------------------------------------
// Description
//  Sets the matrix elements which are responsible for
// rotation.
//
// Parameters
//  NX			- Amount to rotate x-axis
//  NY			- Amount to rotate y-axis
//  XZ			- Amount to rotate z-axis
//
// Returns
//  Nothing
// ------------------------------------------------------------
void MATRIX::Rotate(float NX, float NY, float NZ)
{
	float	vSin, vCos;

	// Y-Axis Rotation
	Identity(UsageMatrix.ElemPtr);
	vSin = sin(NY);
	vCos = cos(NY);
	*(UsageMatrix.ElemPtr)      = vCos;
	*(UsageMatrix.ElemPtr + 2)  = -vSin;
	*(UsageMatrix.ElemPtr + 8)  = vSin;
	*(UsageMatrix.ElemPtr + 10) = vCos;
	Multiply(TempMatrix.ElemPtr, MainMatrix.ElemPtr, UsageMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);

	// X-Axis Rotation
	// NOTE: Must be done in this order as 10 is shared
	Identity(UsageMatrix.ElemPtr);
	vSin = sin(NX);
	vCos = cos(NX);
	*(UsageMatrix.ElemPtr + 5)  = vCos;
	*(UsageMatrix.ElemPtr + 6)  = vSin;
	*(UsageMatrix.ElemPtr + 9)  = -vSin;
	*(UsageMatrix.ElemPtr + 10) = vCos;
	Multiply(TempMatrix.ElemPtr, MainMatrix.ElemPtr, UsageMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);

	// Z-Axis Rotation
	Identity(UsageMatrix.ElemPtr);
	vSin = sin(NZ);
	vCos = cos(NZ);
	*(UsageMatrix.ElemPtr)      = vCos;
	*(UsageMatrix.ElemPtr + 1)  = vSin;
	*(UsageMatrix.ElemPtr + 4)  = -vSin;
	*(UsageMatrix.ElemPtr + 5)  = vCos;
	Multiply(TempMatrix.ElemPtr, MainMatrix.ElemPtr, UsageMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);
};

// ------------------------------------------------------------
// void MATRIX::Translate(float xt, float yt, float zt)
// ------------------------------------------------------------
// Description
//  Sets the matrix elements which are responsible for
// translation.
//
// Parameters
//  xt			- Amount to travel on x-axis
//  yt			- Amount to travel on y-axis
//  zt			- Amount to travel on z-axis
//
// Returns
//  Nothing
// ------------------------------------------------------------
void MATRIX::Translate(float xt, float yt, float zt)
{
	Identity(UsageMatrix.ElemPtr);
	*(UsageMatrix.ElemPtr + 12) = (float)xt;
	*(UsageMatrix.ElemPtr + 13) = (float)yt;
	*(UsageMatrix.ElemPtr + 14) = (float)zt;

	Multiply(TempMatrix.ElemPtr, MainMatrix.ElemPtr, UsageMatrix.ElemPtr);
	Copy(MainMatrix.ElemPtr, TempMatrix.ElemPtr);
};
