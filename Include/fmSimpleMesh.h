/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmSimpleMesh.h
===============================
*/

#pragma once

class FATALISMATRIX_API fmSimpleMesh
{
private:
	// Variablen
	LPD3DXMESH				m_pMesh;
	PDIRECT3DTEXTURE9		m_pTexture;
	PDIRECT3DTEXTURE9		m_pBumpMap;
	PDIRECT3DCUBETEXTURE9*  m_ppEnvMap;
public:
	// Konstruktor und Destruktor
	fmSimpleMesh();
	~fmSimpleMesh() {Exit();}

	fmResult Init(char* pcFilename,
				  char* pcTextureFilename,
				  char* pcBumpMapFilename,
				  PDIRECT3DCUBETEXTURE9* pEnvMap);
	fmResult Render();
	fmResult Exit();

	// Inline Methoden
	// float -> DWORD
	inline DWORD F2DW(float f) {return *((DWORD*)(&f));}
};