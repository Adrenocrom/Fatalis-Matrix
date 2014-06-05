/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmPlane.h
===============================
*/

#pragma once

// Die Ebenenklasse
class FATALISMATRIX_API fmPlane
{
public:
	// Variablen
	union
	{
		struct
		{
			float a;		// Variablen der Ebenengleichung
			float b;
			float c;
			float d;
		};

		struct
		{
			fmVector3	n;	// Normalenvektor
			float		d;	// Distanz
		};

		float v[4];			// Array der Variablen
	};

	// Konstruktoren
	fmPlane()																								{}
	fmPlane(const fmPlane& p) : a(p.a), b(p.b), c(p.c), d(p.d)												{}
	fmPlane(const float _a, const float _b, const float _c, const float _d) : a(_a), b(_b), c(_c), d(_d)	{}
	fmPlane(const float* pfValue) : a(pfValue[0]), b(pfValue[1]), c(pfValue[2]), d(pfValue[3])				{}
	fmPlane(const fmVector3& _n, float _d) : n(_n), d(_d)													{}

	// Casting-Operatoren
	operator float* () {return (float*)(v);}

	// Zuweisungsoperatoren
	fmPlane& operator = (const fmPlane& p) {a = p.a; b = p.b; c = p.c; d = p.d; return *this;}
};

// Vergleichsoperatoren
inline bool operator == (const fmPlane& a, const fmPlane& b) {if(a.a != b.a) return false; if(a.b != b.b) return false; if(a.c != b.c) return false; return a.d == b.d;}
inline bool operator != (const fmPlane& a, const fmPlane& b) {if(a.a != b.a) return true; if(a.b != b.b) return true; if(a.c != b.c) return true; return a.d != b.d;}

// ******************************************************************
// Funktionen deklarieren
inline fmPlane	fmPlaneNormalize(const fmPlane& p)													{const float fLength = fmVector3Length(p.n); return fmPlane(p.n / fLength, p.d / fLength);}
inline float	fmPlaneDotNormal(const fmPlane& p, const fmVector3& v)								{return p.a * v.x + p.b * v.y + p.c * v.z;}
inline float	fmPlaneDotCoords(const fmPlane& p, const fmVector3& v)								{return p.a * v.x + p.b * v.y + p.c * v.z + p.d;}
inline fmPlane	fmPlaneFromPointNormal(const fmVector3& p, const fmVector3& n)						{return fmPlane(n, -n.x * p.x - n.y * p.y - n.z * p.z);}
inline fmPlane	fmPlaneFromPoints(const fmVector3& v1, const fmVector3& v2, const fmVector3& v3)	{return fmPlaneFromPointNormal(v1, fmVector3Cross(v3 - v2, v1 - v2));}

// ******************************************************************