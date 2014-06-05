/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmTerrain.h
===============================
*/

#pragma once


// Vertex-Format definieren
// momentan werden nur Koordinaten und Farben als
// Elemente fuer die Vertices verwendet
#define D3DFVF_TERRAINVERTEX (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEX2)

struct TERRAIN_VERTEX
{
    float x, y, z;
    float tu1, tv1;
    float tu2, tv2;
};


class FATALISMATRIX_API fmTerrain
{
protected:
	// Variablen
	PDIRECT3DTEXTURE9 m_pTexture;
    PDIRECT3DTEXTURE9 m_pDetailTexture;

    // Breite und Laenge des Terrain
    int m_iWidth;
    int m_iLength;

    // der Vertexbuffer enthaelt die Vertices
    PDIRECT3DVERTEXBUFFER9 m_pVB;

    // der Indexbuffer beschreibt die Dreiecke
    PDIRECT3DINDEXBUFFER9 m_pIB;

	// Position und Höhe
	float** m_fHeight;

public:

	fmTerrain();
	~fmTerrain();

	// Terrain aus der uebergebenen Heightmap erzeugen
    fmResult Create(char* pcHeightmap,
					char* pcTerrainTexture,
					char* pcDetailTexture);

    // Terrain rendern
    fmResult Render();

	float HeightAtMap(fmVector3 vPos);

	float PointLine(fmVector2 VecA, fmVector2 VecB, fmVector2 PointToTest);

};