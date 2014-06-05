/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmColor.h
===============================
*/

#pragma once

// Makros und Definitionen
#define FM_COLOR_CONV (0.003921568627450980392156862745098f) // = 1/255

// ******************************************************************
// Die Farbklasse
class FATALISMATRIX_API fmColor
{
public:
	// Variablen
	union
	{
		struct
		{
			float r;	// Rot-, Gruen-, Blau- und Alphakomponente
			float g;
			float b;
			float a;
		};

		struct
		{
			float fRed;		// Rot-, Gruen-, Blau- und Alphakomponente
			float fGreen;
			float fBlue;
			float fAlpha;
		};

		float			c[4];			// Array der Komponenten
		D3DCOLORVALUE	D3DColorValue;	// Farbe als D3DCOLORVALUE
	};

	// Konstruktoren
	fmColor()																																																		{}
	fmColor(const fmColor& c) : r(c.r), g(c.g), b(c.b), a(c.a)																																						{}
	fmColor(const float f) : r(f), g(f), b(f), a(1.0f)																																								{}
	fmColor(const float _r, const float _g, const float _b) : r(_r), g(_g), b(_b), a(1.0f)																															{}
	fmColor(const float _r, const float _g, const float _b, const float _a) : r(_r), g(_g), b(_b), a(_a)																											{}
	fmColor(const float* pfComponent) : r(pfComponent[0]), g(pfComponent[1]), b(pfComponent[2]), a(pfComponent[3])																									{}
	fmColor(const BYTE _r, const BYTE _g, const BYTE _b) : r((float)(_r) * FM_COLOR_CONV), g((float)(_g) * FM_COLOR_CONV), b((float)(_b) * FM_COLOR_CONV), a(1.0f)													{}
	fmColor(const BYTE _r, const BYTE _g, const BYTE _b, const BYTE _a) : r((float)(_r) * FM_COLOR_CONV), g((float)(_g) * FM_COLOR_CONV), b((float)(_b) * FM_COLOR_CONV), a((float)(_a) * FM_COLOR_CONV)			{}
	fmColor(const BYTE* pComponent) : r((float)(pComponent[0]) * FM_COLOR_CONV), g((float)(pComponent[1]) * FM_COLOR_CONV), b((float)(pComponent[2]) * FM_COLOR_CONV), a((float)(pComponent[3]) * FM_COLOR_CONV)	{}
	fmColor(const DWORD c) : r(FM_COLOR_CONV * (float)(BYTE)(c >> 16)), g(FM_COLOR_CONV * (float)(BYTE)(c >> 8)), b(FM_COLOR_CONV * (float)(BYTE)(c)), a(FM_COLOR_CONV * (float)(BYTE)(c >> 24))					{}
	fmColor(const D3DCOLORVALUE& c) : D3DColorValue(c)																																								{}

	// Casting-Operatoren
	operator DWORD () const
	{
		return ((a >= 1.0f ? 255 : a <= 0.0f ? 0 : (DWORD)(a * 255.0f)) << 24) |
			   ((r >= 1.0f ? 255 : r <= 0.0f ? 0 : (DWORD)(r * 255.0f)) << 16) |
			   ((g >= 1.0f ? 255 : g <= 0.0f ? 0 : (DWORD)(g * 255.0f)) << 8)  |
			   (b >= 1.0f ? 255 : b <= 0.0f ? 0 : (DWORD)(b * 255.0f));
	}

	operator D3DCOLORVALUE& ()	{return D3DColorValue;}
	operator float* ()			{return (float*)(c);}

	// Zuweisungsoperatoren
	fmColor& operator = (const fmColor& c)	{D3DColorValue = c.D3DColorValue; return *this;}
	fmColor& operator += (const fmColor& c)	{r += c.r; g += c.g; b += c.b; a += c.a; return *this;}
	fmColor& operator -= (const fmColor& c)	{r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;}
	fmColor& operator *= (const fmColor& c)	{r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this;}
	fmColor& operator *= (const float f)	{r *= f; g *= f; b *= f; a *= f; return *this;}
	fmColor& operator /= (const fmColor& c)	{r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this;}
	fmColor& operator /= (const float f)	{r /= f; g /= f; b /= f; a /= f; return *this;}
};

// Arithmetische Operatoren
inline fmColor operator + (const fmColor& a, const fmColor& b)	{return fmColor(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);}
inline fmColor operator - (const fmColor& a, const fmColor& b)	{return fmColor(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);}
inline fmColor operator - (const fmColor& c)					{return fmColor(-c.r, -c.g, -c.b, c.a);}
inline fmColor operator * (const fmColor& a, const fmColor& b)	{return fmColor(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);}
inline fmColor operator * (const fmColor& c, const float f)		{return fmColor(c.r * f, c.g * f, c.b * f, c.a * f);}
inline fmColor operator * (const float f, const fmColor& c)		{return fmColor(c.r * f, c.g * f, c.b * f, c.a * f);}
inline fmColor operator / (const fmColor& a, const fmColor& b)	{return fmColor(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);}
inline fmColor operator / (const fmColor& c, const float f)		{return fmColor(c.r / f, c.g / f, c.b / f, c.a / f);}

// Vergleichsoperatoren
inline bool operator == (const fmColor& a, const fmColor& b) {if(a.r != b.r) return false; if(a.g != b.g) return false; if(a.b != b.b) return false; return a.a == b.a;}
inline bool operator != (const fmColor& a, const fmColor& b) {if(a.r != b.r) return true; if(a.g != b.g) return true; if(a.b != b.b) return true; return a.a != b.a;}

// ******************************************************************
// Funktionen deklarieren
inline fmColor	fmColorNegate(const fmColor& c)											{return fmColor(1.0f - c.r, 1.0f - c.g, 1.0f - c.b, 1.0f - c.a);}
inline float	fmColorBrightness(const fmColor& c)										{return c.r * 0.299f + c.g * 0.587f + c.b * 0.114f;}
inline fmColor	fmColorInterpolate(const fmColor& c1, const fmColor& c2, const float p)	{return c1 + p * (c2 - c1);}
inline fmColor	fmColorMin(const fmColor& c1, const fmColor& c2)						{return fmColor(FM_MIN(c1.r, c2.r), FM_MIN(c1.g, c2.g), FM_MIN(c1.b, c2.b), FM_MIN(c1.a, c2.a));}
inline fmColor	fmColorMax(const fmColor& c1, const fmColor& c2)						{return fmColor(FM_MAX(c1.r, c2.r), FM_MAX(c1.g, c2.g), FM_MAX(c1.b, c2.b), FM_MAX(c1.a, c2.a));}
inline fmColor	fmColorRandom(float fAlpha)												{return fmColor(fmFloatRandom(0.0f, 1.0f), fmFloatRandom(0.0f, 1.0f), fmFloatRandom(0.0f, 1.0f), fAlpha < 0.0f ? fmFloatRandom(0.0f, 1.0f) : fAlpha);}

// ******************************************************************