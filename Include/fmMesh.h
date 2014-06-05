/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmMesh.h
===============================
*/

#pragma once

// ******************************************************************
// Vertexstruktur
struct FATALISMATRIX_API fmMeshVertex
{
	fmVector3 vPosition;
	fmVector3 vNormal;
	DWORD	dwColor;
	float	tu, tv;
};

// ******************************************************************
// Klasse für 3D Meshs von DirectX 9
class FATALISMATRIX_API fmMesh
{
	friend class fmShadowVolume;

private:
	// Variablen
	LPD3DXMESH              m_pMesh;					// Meshvariable
	D3DMATERIAL9*           m_pMeshMaterials;			
	LPDIRECT3DTEXTURE9*     m_pMeshTextures;
	DWORD                   m_dwNumMaterials;

	DWORD					m_dwNumVertices;			// Anzahl der Vertices
	DWORD					m_dwNumTriangles;			// Anzahl der Dreiecke
	DWORD					m_dwNumIndices;				// Anzahl der Indices

	BOOL					m_bExtraData;				// Extra Daten Erstellen
	BOOL					m_bExtraDataOnly;			// Nur Extra Daten
	BOOL					m_bBumpMap;					// Mit BumpMap
	
	fmVector3*				m_pvVectors;				// Positionsvektoren der Vertizes
	DWORD*					m_pdwIndices;				// Alle Indizes
	fmPlane*				m_pTrianglePlanes;			// Ebenen der Dreiecke (4 pro Dreieck)

	fmMeshVertex*			m_pVertices;				// Vertex
	
	// BumbMap

	PDIRECT3DCUBETEXTURE9*  m_ppEnvMap;					// EnvMap
	PDIRECT3DTEXTURE9		m_pBumpMap;					// BumpMap

public:
	// Konstruktor und Destruktor
	fmMesh();
	~fmMesh() {Exit();}

	// Methoden
	// Aus Datei Initialisieren
	/*
	fmResult  Init(char* pcFilename,
				   char* pcTexturPrefix = "",
				   char* pcTexturPostfix = "",
				   BOOL bGenerateExtraData = TRUE,
				   BOOL bExtraDataOnly = FALSE);*/

	fmResult  Init(char* pcFilename,
				   PDIRECT3DCUBETEXTURE9* pEnvMap = NULL,
				   char* pcBumpMapFilename = "",
				   BOOL	bBumpMap = FALSE,
				   char* pcTexturPrefix = "",
				   char* pcTexturPostfix = "",
				   BOOL bGenerateExtraData = TRUE,
				   BOOL bExtraDataOnly = FALSE);


	fmResult Render();
	fmResult UpdateExtraData();
	fmResult Exit();


	fmResult GetIndexBuffer(PDIRECT3DINDEXBUFFER9* pIndexBuffer);
	fmResult GetVertexBuffer(PDIRECT3DVERTEXBUFFER9* pVertexBuffer);

	// Inline Methoden
	DWORD	GetNumVertices()	{ return m_dwNumVertices; }
	DWORD	GetNumTriangles()	{ return m_dwNumTriangles; }	
	DWORD	GetNumIndices()		{ return m_dwNumIndices; }
	BOOL	ExtraData()			{ return m_bExtraData; }
	BOOL    ExtraDataOnly()     { return m_bExtraDataOnly; }
	inline  DWORD F2DW(float f) {return *((DWORD*)(&f));}
};

// ******************************************************************