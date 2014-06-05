/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmVector3.h
===============================
*/

#pragma once

// Verwendete Symbole vordefinieren
class fmMatrix;

class FATALISMATRIX_API fmVector3
{
public:

	// Variablen
	union
	{
		struct
		{
			float x; // Koordinaten
			float y;
			float z;
		};

		struct
		{
			float u; // texturkoordinaten
			float v;
			float w;
		};

		float     c[3];       // Array der Koordinaten
		D3DVECTOR vD3DVector; // Vector in Form eines Direct3D-Vectors
	};

	// Konstruktoren
	fmVector3()																						{}
	fmVector3(const fmVector3& v) : x(v.x), y(v.y), z(v.z)											{}
	fmVector3(const float f) : x(f), y(f), z(f)														{}
	fmVector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)					{}
	fmVector3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])	{}
	fmVector3(const D3DVECTOR& v) : vD3DVector(v)													{}

	// Casting-Operatoren
	operator float* ()					{return (float*)(c);}
	operator D3DVECTOR& ()				{return vD3DVector;}
	operator const D3DVECTOR& () const	{return vD3DVector;}

	// Zuweisungsoperatoren
	fmVector3& operator =  (const fmVector3& v)	{x = v.x; y = v.y; z = v.z; return *this;}
	fmVector3& operator += (const fmVector3& v)	{x += v.x; y += v.y; z += v.z; return *this;}
	fmVector3& operator -= (const fmVector3& v)	{x -= v.x; y -= v.y; z -= v.z; return *this;}
	fmVector3& operator *= (const fmVector3& v)	{x *= v.x; y *= v.y; z *= v.z; return *this;}
	fmVector3& operator *= (const float f)		{x *= f; y *= f; z *= f; return *this;}
	fmVector3& operator /= (const fmVector3& v)	{x /= v.x; y /= v.y; z /= v.z; return *this;}
	fmVector3& operator /= (const float f)		{x /= f; y /= f; z /= f; return *this;}
};

// Arithmetische Operatoren
inline fmVector3 operator + (const fmVector3& a, const fmVector3& b)	{return fmVector3(a.x + b.x, a.y + b.y, a.z + b.z);}
inline fmVector3 operator - (const fmVector3& a, const fmVector3& b)	{return fmVector3(a.x - b.x, a.y - b.y, a.z - b.z);}
inline fmVector3 operator - (const fmVector3& v)						{return fmVector3(-v.x, -v.y, -v.z);}
inline fmVector3 operator * (const fmVector3& a, const fmVector3& b)	{return fmVector3(a.x * b.x, a.y * b.y, a.z * b.z);}
inline fmVector3 operator * (const fmVector3& v, const float f)			{return fmVector3(v.x * f, v.y * f, v.z * f);}
inline fmVector3 operator * (const float f, const fmVector3& v)			{return fmVector3(v.x * f, v.y * f, v.z * f);}
inline fmVector3 operator / (const fmVector3& a, const fmVector3& b)	{return fmVector3(a.x / b.x, a.y / b.y, a.z / b.z);}
inline fmVector3 operator / (const fmVector3& v, const float f)			{return fmVector3(v.x / f, v.y / f, v.z / f);}

// Vergleichsoperatoren
inline bool operator == (const fmVector3& a, const fmVector3& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; return a.z == b.z;}
inline bool operator != (const fmVector3& a, const fmVector3& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; return a.z != b.z;}

// ******************************************************************
// Funktionen deklarieren
inline float		fmVector3Length(const fmVector3& v)													{return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);}
inline float		fmVector3LengthSq(const fmVector3& v)												{return v.x * v.x + v.y * v.y + v.z * v.z;}
inline fmVector3	fmVector3Normalize(const fmVector3& v)												{return v / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);}
inline fmVector3	fmVector3NormalizeEx(const fmVector3& v)											{return v / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);}
inline fmVector3	fmVector3Cross(const fmVector3& v1, const fmVector3& v2)							{return fmVector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
inline float		fmVector3Dot(const fmVector3& v1, const fmVector3& v2)								{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
inline float		fmVector3Angle(const fmVector3& v1, const fmVector3& v2)							{return acosf((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / sqrtf((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)));}
inline fmVector3	fmVector3InterpolateCoords(const fmVector3& v1, const fmVector3& v2, const float p)	{return v1 + p * (v2 - v1);}
inline fmVector3	fmVector3InterpolateNormal(const fmVector3& v1, const fmVector3& v2, const float p)	{return fmVector3NormalizeEx(v1 + p * (v2 - v1));}
inline fmVector3	fmVector3Min(const fmVector3& v1, const fmVector3& v2)								{return fmVector3(FM_MIN(v1.x, v2.x), FM_MIN(v1.y, v2.y), FM_MIN(v1.z, v2.z));}
inline fmVector3	fmVector3Max(const fmVector3& v1, const fmVector3& v2)								{return fmVector3(FM_MAX(v1.x, v2.x), FM_MAX(v1.y, v2.y), FM_MAX(v1.z, v2.z));}
inline fmVector3	fmVector3Random()																	{return fmVector3NormalizeEx(fmVector3(fmFloatRandom(-1.0f, 1.0f), fmFloatRandom(-1.0f, 1.0f), fmFloatRandom(-1.0f, 1.0f)));}

// ******************************************************************