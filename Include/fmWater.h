/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmWater.h
===============================
*/

#pragma once

#define D3DFVF_WATERVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1| D3DFVF_TEX2)

struct WATER_VERTEX
{
	fmVector3 vPosition;
	fmVector3 vNormal;
	float	  tu1, tv1;
	float	  tu2, tv2;
};

class FATALISMATRIX_API fmWater
{
protected:
	// Variablen
	
	int m_iNumVertices;
	int m_iNumIndices;

	// Breite und Laenge des Wassers
    int m_iWidth;
    int m_iLength;

	// Index Array
	WORD* m_wIndexArray;

	// Vertices
	WATER_VERTEX* m_pVertices;

	// der Vertexbuffer enthaelt die Vertices
    PDIRECT3DVERTEXBUFFER9 m_pWaterVB;

    // der Indexbuffer beschreibt die Dreiecke
    PDIRECT3DINDEXBUFFER9 m_pWaterIB;


	// Wasserdesign
	LPD3DXEFFECT		m_pWaterEffect;		  // Wassereffect
	PDIRECT3DTEXTURE9	m_apWaterTextures[3]; // Wassertexturen

	float				m_fTime;			  // Zeitkomponente

public:
	// Kunstruktor und Destruktor
	fmWater();
	~fmWater();

	fmResult Create(int iWidth,
					int iLength,
					float fHeight,
					char* pcEffect);

	fmResult Render();
	fmResult Update(float fTime);

	fmVector3 PositionProc(fmVector3 v);
	fmVector3 NormalProc(fmVector3 v);
	fmVector2 TextureProc(fmVector3 v);

	inline DWORD F2DW(float f) {return *((DWORD*)(&f));}
};