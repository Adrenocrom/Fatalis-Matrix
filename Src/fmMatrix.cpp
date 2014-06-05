/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmMatrix.cpp
===============================
*/

#include <FatalisMatrix.h>

// ******************************************************************
// Translationsmatrix berechnen
FATALISMATRIX_API fmMatrix fmMatrixTranslation(const fmVector3& v)
{
	return fmMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		            0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					v.x,  v.y,  v.z,  1.0f);
}

// ******************************************************************
// Rotationsmatrix für Rotation um die x-Achse berechnen
FATALISMATRIX_API fmMatrix fmMatrixRotationX(const float f)
{
	fmMatrix mResult;

	// Rotationsmatrix berechnen
	mResult.m11 = 1.0f; mResult.m12 = 0.0f; mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	mResult.m21 = 0.0f;                                         mResult.m24 = 0.0f;
	mResult.m31 = 0.0f;                                         mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m22 = mResult.m33 = cosf(f);
	mResult.m23 = sinf(f);
	mResult.m32 = -mResult.m23;

	return mResult;
}

// ******************************************************************
// Rotationsmatrix für Rotation um die y-Achse berechnen
FATALISMATRIX_API fmMatrix fmMatrixRotationY(const float f)
{
	fmMatrix mResult;

	// Rotationsmatrix berechnen
	                    mResult.m12 = 0.0f;                     mResult.m14 = 0.0f;
	mResult.m21 = 0.0f; mResult.m22 = 1.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	                    mResult.m32 = 0.0f;                     mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m33 = cosf(f);
	mResult.m31 = sinf(f);
	mResult.m13 = -mResult.m31;

	return mResult;
}

// ******************************************************************
// Rotationsmatrix für Rotation um die z-Achse berechnen
FATALISMATRIX_API fmMatrix fmMatrixRotationZ(const float f)
{
	fmMatrix mResult;

	// Rotationsmatrix berechnen
	                                        mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	                                        mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	mResult.m31 = 0.0f; mResult.m32 = 0.0f; mResult.m33 = 1.0f; mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m22 = cosf(f);
	mResult.m12 = sinf(f);
	mResult.m21 = -mResult.m12;

	return mResult;
}

// ******************************************************************
// Rotiert um alle drei Achsen
FATALISMATRIX_API fmMatrix fmMatrixRotation(const float x,
									        const float y,
									        const float z)
{
	return fmMatrixRotationZ(z) *
		   fmMatrixRotationX(x) *
		   fmMatrixRotationY(y);
}

// ******************************************************************
// Rotiert um alle drei Achsen (Winkel in Vektor)
FATALISMATRIX_API fmMatrix fmMatrixRotation(const fmVector3& v)
{
	return fmMatrixRotationZ(v.z) *
		   fmMatrixRotationX(v.x) *
		   fmMatrixRotationY(v.y);
}

// ******************************************************************
// Rotationsmatrix für Rotation um eine beliebige Achse berechnen
FATALISMATRIX_API fmMatrix fmMatrixRotationAxis(const fmVector3& v,
										        const float f)
{
	// Sinus und Kosinus berechnen
	const float fSin = sinf(-f);
	const float fCos = cosf(-f);
	const float fOneMinusCos = 1.0f - fCos;

	// Achsenvektor normalisieren
	const fmVector3 vAxis(fmVector3Normalize(v));

	// Matrix erstellen
	return fmMatrix((vAxis.x * vAxis.x) * fOneMinusCos + fCos,
		            (vAxis.x * vAxis.y) * fOneMinusCos - (vAxis.z * fSin),
				    (vAxis.x * vAxis.z) * fOneMinusCos + (vAxis.y * fSin),
					0.0f,
					(vAxis.y * vAxis.x) * fOneMinusCos + (vAxis.z * fSin),
					(vAxis.y * vAxis.y) * fOneMinusCos + fCos,
					(vAxis.y * vAxis.z) * fOneMinusCos - (vAxis.x * fSin),
					0.0f,
					(vAxis.z * vAxis.x) * fOneMinusCos - (vAxis.y * fSin),
					(vAxis.z * vAxis.y) * fOneMinusCos + (vAxis.x * fSin),
					(vAxis.z * vAxis.z) * fOneMinusCos + fCos,
					0.0f,
					0.0f,
					0.0f,
					0.0f,
					1.0f);
}

// ******************************************************************
// Skalierungsmatrix berechnen
FATALISMATRIX_API fmMatrix fmMatrixScaling(const fmVector3& v)
{
	// Skalierungsmatrix berechnen
	return fmMatrix(v.x,  0.0f, 0.0f, 0.0f,
		            0.0f, v.y,  0.0f, 0.0f,
					0.0f, 0.0f, v.z,  0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

// ******************************************************************
// Liefert eine Achsenmatrix
FATALISMATRIX_API fmMatrix fmMatrixAxes(const fmVector3& vXAxis,
								        const fmVector3& vYAxis,
								        const fmVector3& vZAxis)
{
	return fmMatrix(vXAxis.x, vXAxis.y, vXAxis.z, 0.0f,
		            vYAxis.x, vYAxis.y, vYAxis.z, 0.0f,
					vZAxis.x, vZAxis.y, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************
// Determinante einer Matrix berechnen
FATALISMATRIX_API float fmMatrixDet(const fmMatrix& m)
{
	// Determinante der linken oberen 3x3-Teilmatrix berechnen
	return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
           m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
           m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}

// ******************************************************************
// Invertierte Matrix berechnen
FATALISMATRIX_API fmMatrix fmMatrixInvert(const fmMatrix& m)
{
	// Kehrwert der Determinante vorberechnen
	float fInvDet = fmMatrixDet(m);
	if(fInvDet == 0.0f) return fmMatrixIdentity();
	fInvDet = 1.0f / fInvDet;

	// Invertierte Matrix berechnen
	fmMatrix mResult;
	mResult.m11 =  fInvDet * (m.m22 * m.m33 - m.m23 * m.m32);
	mResult.m12 = -fInvDet * (m.m12 * m.m33 - m.m13 * m.m32);
	mResult.m13 =  fInvDet * (m.m12 * m.m23 - m.m13 * m.m22);
	mResult.m14 =  0.0f;
	mResult.m21 = -fInvDet * (m.m21 * m.m33 - m.m23 * m.m31);
	mResult.m22 =  fInvDet * (m.m11 * m.m33 - m.m13 * m.m31);
	mResult.m23 = -fInvDet * (m.m11 * m.m23 - m.m13 * m.m21);
	mResult.m24 =  0.0f;
	mResult.m31 =  fInvDet * (m.m21 * m.m32 - m.m22 * m.m31);
	mResult.m32 = -fInvDet * (m.m11 * m.m32 - m.m12 * m.m31);
	mResult.m33 =  fInvDet * (m.m11 * m.m22 - m.m12 * m.m21);
	mResult.m34 =  0.0f;
	mResult.m41 = -(m.m41 * mResult.m11 + m.m42 * mResult.m21 + m.m43 * mResult.m31);
	mResult.m42 = -(m.m41 * mResult.m12 + m.m42 * mResult.m22 + m.m43 * mResult.m32);
	mResult.m43 = -(m.m41 * mResult.m13 + m.m42 * mResult.m23 + m.m43 * mResult.m33);
	mResult.m44 =  1.0f;

	return mResult;
}

// ******************************************************************
// Transponierte Matrix berechnen
FATALISMATRIX_API fmMatrix fmMatrixTranspose(const fmMatrix& m)
{
	// Matrix transponieren
	return fmMatrix(m.m11, m.m21, m.m31, m.m41,
		            m.m12, m.m22, m.m32, m.m42,
					m.m13, m.m23, m.m33, m.m43,
					m.m14, m.m24, m.m34, m.m44);
}

// ******************************************************************
// Projektionsmatrix berechnen
FATALISMATRIX_API fmMatrix fmMatrixProjection(const float fFOV,
										      const float fAspect,
										      const float fNearPlane,
										      const float fFarPlane)
{
	const float s = 1.0f / tanf(fFOV * 0.5f);
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	return fmMatrix(s / fAspect, 0.0f, 0.0f, 0.0f,
		            0.0f, s, 0.0f, 0.0f,
					0.0f, 0.0f, Q, 1.0f,
					0.0f, 0.0f, -Q * fNearPlane, 0.0f);
}

// ******************************************************************
// Kameramatrix berechnen
FATALISMATRIX_API fmMatrix fmMatrixCamera(const fmVector3& vPos,
									      const fmVector3& vLookAt,
									      const fmVector3& vUp) // = fmVector3(0.0f, 1.0f, 0.0f)
{
	// Die z-Achse des Kamerakoordinatensystems berechnen
	fmVector3 vZAxis(fmVector3Normalize(vLookAt - vPos));

	// Die x-Achse ist das Kreuzprodukt aus y- und z-Achse
	fmVector3 vXAxis(fmVector3Normalize(fmVector3Cross(vUp, vZAxis)));

	// y-Achse berechnen
	fmVector3 vYAxis(fmVector3Normalize(fmVector3Cross(vZAxis, vXAxis)));

	// Rotationsmatrix erzeugen und die Translationsmatrix mit ihr multiplizieren
	return fmMatrixTranslation(-vPos) *
	       fmMatrix(vXAxis.x, vYAxis.x, vZAxis.x, 0.0f,
		            vXAxis.y, vYAxis.y, vZAxis.y, 0.0f,
				    vXAxis.z, vYAxis.z, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************
// Texturmatrix berechnen
FATALISMATRIX_API fmMatrix fmMatrixToTex2DMatrix(const fmMatrix& m)
{
	// Texturmatrix berechnen
	return fmMatrix(m.m11, m.m12, m.m14, 0.0f,
		            m.m21, m.m22, m.m24, 0.0f,
					m.m41, m.m42, m.m44, 0.0f,
					0.0f,  0.0f,  0.0f,  1.0f);
}

// ******************************************************************