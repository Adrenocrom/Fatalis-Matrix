/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmShadowVolume.h
===============================
*/

#pragma once

struct FATALISMATRIX_API fmShadowVertex
{
	fmVector3		vPosition;
	float			fRHW;
	D3DCOLOR		Color;
	static DWORD	dwFVF;
};

struct FATALISMATRIX_API fmEdge
{
	DWORD dwPointA;					// Struktur für die Dreiecksseiten
	DWORD dwPointB;
};

class FATALISMATRIX_API fmShadowVolume
{
private:
	// Variablen
	fmMesh*    m_pModel;				// Das Model
	fmVector3* m_pvTriangleCenters;		// Mittelpunkt der Dreiecke
	fmEdge*	   m_pEdges;				// Liste der Dreiecksseiten
	DWORD	   m_dwNumEdges;			// Anzahl der Dreiecksseiten
	fmEffect*  m_pShadowVolumeEffect;
	fmEffect*  m_pShadowEffect;
	fmVector3* m_pvVertices;			// Die Vertizes für das Schattenvolumen
	DWORD	   m_dwNumVertices;			// Anzahl der Vertizes

public:

	// Konstruktor und Destruktor
	fmShadowVolume();
	~fmShadowVolume() {Exit();}

	// Methoden
	fmResult Init(fmMesh* pModel);
	fmResult Exit();

	void		AddEdge(const DWORD dwPointA, const DWORD dwPointB);																					// Fügt eine Dreiecksseite hinzu
	fmResult	ComputeVolume(const fmMatrix& mInvModelMatrix, const D3DLIGHT9& Light, const float fLength = 1000.0f, const BOOL bNormalize = FALSE);	// Berechnung des Schattenvolumens
	fmResult	RenderVolume(const fmMatrix& mModelMatrix);																								// Rendern des Schattenvolumens
	fmResult	RenderShadow(const fmColor& ShadowColor = fmColor(0.0f, 0.0f, 0.0f, 0.5f));																// Rendern des Schattens

	// Inline-Methoden
	fmMesh*	GetModel()					{return m_pModel;}
	fmEffect*	GetShadowVolumeEffect()	{return m_pShadowVolumeEffect;}
	fmEffect*	GetShadowEffect()		{return m_pShadowEffect;}
	fmVector3*	GetVertices()			{return m_pvVertices;}
};