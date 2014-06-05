/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmUtils.h
===============================
*/

#pragma once

// Verwendete Symbole vordeklarieren
class fmVector2;
class fmVector3;
class fmMatrix;

// ******************************************************************
// Funktionen deklarieren
FATALISMATRIX_API   void		fmSRand(DWORD dwSeed);								// Zufallsgenerator initialisieren
FATALISMATRIX_API	int			fmIntRandom(const int iMin, const int iMax);		// int-Zufallswert
FATALISMATRIX_API	float		fmFloatRandom(const float fMin, const float fMax);	// float-Zufallswert

FATALISMATRIX_API fmVector2 fmPointsOnSceen(fmVector3 vVector, fmMatrix mProjectionMatrix, fmVector2 vScreen);
FATALISMATRIX_API float     fmPointsLengthSq(fmVector2 vPoint1, fmVector2 vPoint2);
FATALISMATRIX_API fmVector3 fmMultiply3DVectorWithMatrix(fmVector3 vVectorIn, fmMatrix mMatrix);
FATALISMATRIX_API fmVector3 fmMultiply3DVectorWithRotationMatrix(fmVector3 vVectorIn, fmMatrix mMatrix);

FATALISMATRIX_API fmVector2	fmVector2TransformCoords(const fmVector2& v, const fmMatrix& m, float* const pfOutW = NULL);																																																					// 2D-Vektor mit Matrix multiplizieren
FATALISMATRIX_API fmVector2	fmVector2TransformNormal(const fmVector2& v, const fmMatrix& m);																																																												// 2D-Normalenvektor mit Matrix multiplizieren
FATALISMATRIX_API fmVector2	fmVector2TransformNormal_TranspInv(const fmVector2& v, const fmMatrix& m);																																																										// 2D-Normalenvektor mit Matrix multiplizieren
FATALISMATRIX_API fmVector3	fmVector3TransformCoords(const fmVector3& v, const fmMatrix& m, float* const pfOutW = NULL);																																																					// 3D-Vektor mit Matrix multiplizieren
FATALISMATRIX_API fmVector3	fmVector3TransformNormal(const fmVector3& v, const fmMatrix& m);																																																												// 3D-Normalenvektor mit Matrix multiplizieren
FATALISMATRIX_API fmVector3	fmVector3TransformNormal_TranspInv(const fmVector3& v, const fmMatrix& m);