#ifndef IMMERSION_MATRIX_H
#define IMMERSION_MATRIX_H
// ------------------------------------------------------------
// Immersion Matrix
// Copyright 1997-1998 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------

//
// Matrix structure
//
typedef struct
{
	float Element[4][4];
	float *ElemPtr;

} MATRIX4x4;

//
// Matrix Class
//
class MATRIX
{
public:
	MATRIX4x4		MainMatrix;
	MATRIX4x4		UsageMatrix;
	MATRIX4x4		TempMatrix;

	// Matrix Initilization
	MATRIX();

   // Matrix Manuiplation
	void	Identity();
	void 	Identity(float *ptr);
	void 	Scale(float sx, float sy, float sz);
	void 	Rotate(float rx, float ry, float rz);
	void 	Translate(float tx, float ty, float tz);
	void	Multiply(float *dest, float *mat1, float *mat2);
	void	Copy(float *dest, float *src);
};

#endif
