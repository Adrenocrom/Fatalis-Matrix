/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmSkyBox.h
===============================
*/

#pragma once

struct SSkyboxVertex
{
	fmVector3 vPosition;
	fmVector3 vTexture;

	static const DWORD dwFVF;
};



class FATALISMATRIX_API fmSkyBox
{
private:
	// Variablen
	PDIRECT3DCUBETEXTURE9 m_pEnvMap;			// Die Skyboxtextur
	SSkyboxVertex		  m_aSkyboxVertex[8];	// Vertexarray
	unsigned short		  m_ausSkyboxIndex[36]; // Indexarray

public:
	// Konstruktor und Destruktor
	fmSkyBox();
	~fmSkyBox();

	fmResult Create(char* pcFilename);
	fmResult Render();
	fmResult Update();

	inline PDIRECT3DCUBETEXTURE9* GetSkyBoxTexture() {return &m_pEnvMap;}
};