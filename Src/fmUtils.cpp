/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmUtils.cpp
===============================
*/

#include <FatalisMatrix.h>

FATALISMATRIX_API void fmSRand(DWORD dwSeed)
{
	srand(dwSeed);
}

// ******************************************************************
// int-Zufallswert
FATALISMATRIX_API	int fmIntRandom(const int iMin,
							const int iMax)
{
	return iMin + (rand() % (iMax - iMin + 1));
}

// ******************************************************************
// float-Zufallswert
FATALISMATRIX_API	float fmFloatRandom(const float fMin,
								const float fMax)
{
	return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);
}

FATALISMATRIX_API fmVector2 fmPointsOnSceen(fmVector3 vVector, fmMatrix mProjectionMatrix, fmVector2 vScreen)
{
    fmVector3   vTemp1;
	float		fTempInvW;
	fmVector2	vTemp2;

	vTemp1.x = mProjectionMatrix.m11*vVector.x + mProjectionMatrix.m21*vVector.y + mProjectionMatrix.m31*vVector.z + mProjectionMatrix.m41;
	vTemp1.y = mProjectionMatrix.m12*vVector.x + mProjectionMatrix.m22*vVector.y + mProjectionMatrix.m32*vVector.z + mProjectionMatrix.m42;  
	vTemp1.z = mProjectionMatrix.m14*vVector.x + mProjectionMatrix.m24*vVector.y + mProjectionMatrix.m34*vVector.z + mProjectionMatrix.m44; 

	fTempInvW = 1 / vTemp1.z;

	vTemp2.x = (1.0f + (vTemp1.x*fTempInvW)) * 0.5f * vScreen.x;
	vTemp2.y = (1.0f - (vTemp1.y*fTempInvW)) * 0.5f * vScreen.y;

	return vTemp2;
}

FATALISMATRIX_API float fmPointsLengthSq(fmVector2 vPoint1, fmVector2 vPoint2)
{
	return ((vPoint2.x-vPoint1.x)*(vPoint2.x-vPoint1.x))+((vPoint2.y-vPoint1.y)*(vPoint2.y-vPoint1.y));
}

FATALISMATRIX_API fmVector3 fmMultiply3DVectorWithMatrix(fmVector3 vVectorIn, fmMatrix mMatrix)
{
	fmVector3 vTemp1;

	vTemp1 = vVectorIn;

    vTemp1.x = vVectorIn.x*mMatrix.m11 + vVectorIn.y*mMatrix.m21 + vVectorIn.z*mMatrix.m31 + mMatrix.m41; 
    vTemp1.y = vVectorIn.x*mMatrix.m12 + vVectorIn.y*mMatrix.m22 + vVectorIn.z*mMatrix.m32 + mMatrix.m42; 
    vTemp1.z = vVectorIn.x*mMatrix.m13 + vVectorIn.y*mMatrix.m23 + vVectorIn.z*mMatrix.m33 + mMatrix.m43;

	return vTemp1;
}

FATALISMATRIX_API fmVector3 fmMultiply3DVectorWithRotationMatrix(fmVector3 vVectorIn, fmMatrix mMatrix)
{
    fmVector3 vTemp1;

    vTemp1.x = vVectorIn.x*mMatrix.m11 + vVectorIn.y*mMatrix.m21 + vVectorIn.z*mMatrix.m31; 
    vTemp1.y = vVectorIn.x*mMatrix.m12 + vVectorIn.y*mMatrix.m22 + vVectorIn.z*mMatrix.m32; 
    vTemp1.z = vVectorIn.x*mMatrix.m13 + vVectorIn.y*mMatrix.m23 + vVectorIn.z*mMatrix.m33;

	return vTemp1;
}

// ******************************************************************
// 2D-Positionsvektor transformieren
FATALISMATRIX_API fmVector2 fmVector2TransformCoords(const fmVector2& v,
													 const fmMatrix& m,
													 float* const pfOutW) // = NULL
{
	// Vektor mit Matrix transformieren
	fmVector2 vResult(v.x * m.m11 + v.y * m.m21 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + m.m42);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	const float w = v.x * m.m14 + v.y * m.m24 + m.m44;
	if(w != 1.0f) vResult /= w;

	// Wenn erwünscht, w kopieren
	if(pfOutW) *pfOutW = w;

	return fmVector2Normalize(vResult) * fmVector2Length(v);
}

// ******************************************************************
// 2D-Richtungsvektor transformieren
FATALISMATRIX_API fmVector2 fmVector2TransformNormal(const fmVector2& v,
													 const fmMatrix& m)
{
	// Vektorlänge berechnen
	const float fLength = fmVector2Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte inverse Matrix berechnen
	const fmMatrix mTransform(fmMatrixTranspose(fmMatrixInvert(m)));

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return fmVector2Normalize(fmVector2(v.x * mTransform.m11 + v.y * mTransform.m21,
		                                v.x * mTransform.m12 + v.y * mTransform.m22))
	       * fLength;
}

// ******************************************************************
// 2D-Richtungsvektor transformieren (transponierte invertierte Matrix)
FATALISMATRIX_API fmVector2 fmVector2TransformNormal_TranspInv(const fmVector2& v,
															   const fmMatrix& m)
{
	// Vektorlänge berechnen
	const float fLength = fmVector2Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return fmVector2Normalize(fmVector2(v.x * m.m11 + v.y * m.m21,
		                                v.x * m.m12 + v.y * m.m22))
	       * fLength;
}

// ******************************************************************
// 3D-Positionsvektor transformieren
FATALISMATRIX_API fmVector3 fmVector3TransformCoords(const fmVector3& v,
													 const fmMatrix& m,
													 float* const pfOutW) // = NULL
{
	// Vektor mit Matrix multiplizieren
	fmVector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	const float w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44;
	if(w != 1.0f) vResult /= w;

	// Wenn erwünscht, w kopieren
	if(pfOutW) *pfOutW = w;

	return vResult;
}

// ******************************************************************
// 3D-Richtungsvektor transformieren
FATALISMATRIX_API fmVector3 fmVector3TransformNormal(const fmVector3& v,
													 const fmMatrix& m)
{
	// Vektorlänge berechnen
	const float fLength = fmVector3Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte invertierte Matrix berechnen
	const fmMatrix mTransform(fmMatrixTranspose(fmMatrixInvert(m)));

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return fmVector3Normalize(fmVector3(v.x * mTransform.m11 + v.y * mTransform.m21 + v.z * mTransform.m31,
		                                v.x * mTransform.m12 + v.y * mTransform.m22 + v.z * mTransform.m32,
										v.x * mTransform.m13 + v.y * mTransform.m23 + v.z * mTransform.m33))
		   * fLength;
}

// ******************************************************************
// 3D-Richtungsvektor transformieren (transponierte invertierte Matrix)
FATALISMATRIX_API fmVector3 fmVector3TransformNormal_TranspInv(const fmVector3& v,
															   const fmMatrix& m)
{
	// Vektorlänge berechnen
	const float fLength = fmVector3Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return fmVector3Normalize(fmVector3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
		                                v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
										v.x * m.m13 + v.y * m.m23 + v.z * m.m33))
		   * fLength;
}