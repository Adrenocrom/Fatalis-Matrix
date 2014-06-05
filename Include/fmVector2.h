/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmVector2.h
===============================
*/

#pragma once

// Verwendete Symbole vordefinieren
class fmMatrix;

// ******************************************************************
// Die 2D-Vektor-Klasse
class FATALISMATRIX_API fmVector2
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
		};

		float c[2];		// Array der Koordinaten
	};

	// Konstruktoren
	fmVector2()																	{}
	fmVector2(const fmVector2& v) : x(v.x), y(v.y)								{}
	fmVector2(const float f) : x(f), y(f)										{}
	fmVector2(const float _x, const float _y) : x(_x), y(_y)					{}
	fmVector2(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1])	{}

	// Casting-Operatoren
	operator float* () {return (float*)(c);}

	// Zuweisungsoperatoren
	fmVector2& operator = (const fmVector2& v)	{x = v.x; y = v.y; return *this;}
	fmVector2& operator += (const fmVector2& v)	{x += v.x; y += v.y; return *this;}
	fmVector2& operator -= (const fmVector2& v)	{x -= v.x; y -= v.y; return *this;}
	fmVector2& operator *= (const fmVector2& v)	{x *= v.x; y *= v.y; return *this;}
	fmVector2& operator *= (const float f)		{x *= f; y *= f; return *this;}
	fmVector2& operator /= (const fmVector2& v)	{x /= v.x; y /= v.y; return *this;}
	fmVector2& operator /= (const float f)		{x /= f; y /= f; return *this;}
};

// Arithmetische Operatoren
inline fmVector2 operator + (const fmVector2& a, const fmVector2& b)	{return fmVector2(a.x + b.x, a.y + b.y);}
inline fmVector2 operator - (const fmVector2& a, const fmVector2& b)	{return fmVector2(a.x - b.x, a.y - b.y);}
inline fmVector2 operator - (const fmVector2& v)						{return fmVector2(-v.x, -v.y);}
inline fmVector2 operator * (const fmVector2& a, const fmVector2& b)	{return fmVector2(a.x * b.x, a.y * b.y);}
inline fmVector2 operator * (const fmVector2& v, const float f)			{return fmVector2(v.x * f, v.y * f);}
inline fmVector2 operator * (const float f, const fmVector2& v)			{return fmVector2(v.x * f, v.y * f);}
inline fmVector2 operator / (const fmVector2& a, const fmVector2& b)	{return fmVector2(a.x / b.x, a.y / b.y);}
inline fmVector2 operator / (const fmVector2& v, const float f)			{return fmVector2(v.x / f, v.y / f);}

// Vergleichsoperatoren
inline bool operator == (const fmVector2& a, const fmVector2& b) {if(a.x != b.x) return false; return a.y == b.y;}
inline bool operator != (const fmVector2& a, const fmVector2& b) {if(a.x != b.x) return true; return a.y != b.y;}

// ******************************************************************
// Funktionen deklarieren
inline float		fmVector2Length(const fmVector2& v)													{return sqrtf(v.x * v.x + v.y * v.y);}
inline float		fmVector2LengthSq(const fmVector2& v)												{return v.x * v.x + v.y * v.y;}
inline fmVector2	fmVector2Normalize(const fmVector2& v)												{return v / sqrtf(v.x * v.x + v.y * v.y);}
inline fmVector2	fmVector2NormalizeEx(const fmVector2& v)											{return v / (sqrtf(v.x * v.x + v.y * v.y) + 0.0001f);}
inline float		fmVector2Dot(const fmVector2& v1, const fmVector2& v2)								{return v1.x * v2.x + v1.y * v2.y;}
inline float		fmVector2Angle(const fmVector2& v1, const fmVector2& v2)							{return acosf((v1.x * v2.x + v1.y * v2.y) / sqrtf((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y)));}
inline fmVector2	fmVector2InterpolateCoords(const fmVector2& v1, const fmVector2& v2, const float p)	{return v1 + p * (v2 - v1);}
inline fmVector2	fmVector2InterpolateNormal(const fmVector2& v1, const fmVector2& v2, const float p)	{return fmVector2NormalizeEx(v1 + p * (v2 - v1));}
inline fmVector2	fmVector2Min(const fmVector2& v1, const fmVector2& v2)								{return fmVector2(FM_MIN(v1.x, v2.x), FM_MIN(v1.y, v2.y));}
inline fmVector2	fmVector2Max(const fmVector2& v1, const fmVector2& v2)								{return fmVector2(FM_MAX(v1.x, v2.x), FM_MAX(v1.y, v2.y));}
inline fmVector2	fmVector2Random()																	{return fmVector2NormalizeEx(fmVector2(fmFloatRandom(-1.0f, 1.0f), fmFloatRandom(-1.0f, 1.0f)));}

// ******************************************************************