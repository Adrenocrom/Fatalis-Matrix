/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmModel.h
===============================
*/

#pragma once

// **************************************************************************
// ModelVertex

struct FATALISMATRIX_API fmModelVertex
{
	fmVector3 vPosition;
	fmVector3 vNormal;
	DWORD	dwColor;
	float tu1, tv1;
	float tu2, tv2;
};

// **************************************************************************
// Vertexformat

#define D3DFVF_FMMODELVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEX2)

// **************************************************************************
// Modelklasse

class FATALISMATRIX_API fmModel
{
	friend class fmShadowVolume;

private:
	
	// Variablen
	PDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;			// Vertex und
	PDIRECT3DINDEXBUFFER9	m_pIndexBuffer;				// Indexbuffer des Models

	DWORD					m_dwNumVertices;			// Anzahl der Vertices
	DWORD					m_dwNumTriangles;			// Anzahl der Dreiecke
	DWORD					m_dwNumIndices;				// Anzahl der Indices

	BOOL					m_bExtraData;				// Extra Daten Erstellen
	BOOL					m_bExtraDataOnly;			// Nur Extra Daten

	fmVector3*				m_pvVectors;					// Positionsvectoren
	DWORD*					m_pdwIndices;				// Alle Indizes
	fmPlane*				m_pTrianglePlanes;			// Ebenen der Dreiecke (4 pro Dreieck)

	fmModelVertex*			m_pVertices;				// Modelverticen

public:

	// Konstruktor und Destruktor
	fmModel();
	~fmModel() {Exit();}

	fmResult Init(char* pcFilename, BOOL bExtraData = TRUE, BOOL bExtraDataOnly = FALSE);
	fmResult Render();
	fmResult Exit();

	// Inline Methoden
	DWORD	GetNumVertices()	{ return m_dwNumVertices; }
	DWORD	GetNumTriangles()	{ return m_dwNumTriangles; }	
	DWORD	GetNumIndices()		{ return m_dwNumIndices; }
	BOOL	ExtraData()			{ return m_bExtraData; }
	BOOL    ExtraDataOnly()     { return m_bExtraDataOnly; }
};

// **************************************************************************