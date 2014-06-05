/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmSkyBox.cpp
===============================
*/

#include <FatalisMatrix.h>

const DWORD SSkyboxVertex::dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

fmSkyBox::fmSkyBox()
{
	m_pEnvMap = NULL;
}

fmSkyBox::~fmSkyBox()
{
	FM_SAFE_RELEASE(m_pEnvMap);
}

fmResult fmSkyBox::Create(char* pcFilename)
{
	fmDirect3D& D3D = fmDirect3D::Instance();
	HRESULT hResult;


	if(FAILED(hResult = D3DXCreateCubeTextureFromFileEx(D3D.GetDevice(),
													    pcFilename,
													    D3DX_DEFAULT,
													    1,
													    0,
													    D3DFMT_UNKNOWN,
													    D3DPOOL_MANAGED,
													    D3DX_FILTER_NONE,
													    D3DX_DEFAULT,
													    0,
													    NULL,
													    NULL,
													    &m_pEnvMap)))
	{
		FM_ERROR_B("Konnte SkyBoxTexture nicht Laden...", FM_ERROR);
	}


	// ------------------------------------------------------------------

	// Die Vertizes der Sky-Box erstellen
	m_aSkyboxVertex[0].vPosition = fmVector3(-1.0f,  1.0f,  1.0f);
	m_aSkyboxVertex[1].vPosition = fmVector3( 1.0f,  1.0f,  1.0f);
	m_aSkyboxVertex[2].vPosition = fmVector3( 1.0f,  1.0f, -1.0f);
	m_aSkyboxVertex[3].vPosition = fmVector3(-1.0f,  1.0f, -1.0f);
	m_aSkyboxVertex[4].vPosition = fmVector3(-1.0f, -1.0f,  1.0f);
	m_aSkyboxVertex[5].vPosition = fmVector3( 1.0f, -1.0f,  1.0f);
	m_aSkyboxVertex[6].vPosition = fmVector3( 1.0f, -1.0f, -1.0f);
	m_aSkyboxVertex[7].vPosition = fmVector3(-1.0f, -1.0f, -1.0f);

	// Die Texturkoordinaten brauchen wir nicht per Hand einzutragen: sie entsprechen
	// ganz einfach den Positionsangaben.
	for(int iVertex = 0; iVertex < 8; iVertex++)
	{
		m_aSkyboxVertex[iVertex].vTexture = m_aSkyboxVertex[iVertex].vPosition;
	}

	// Nun tragen wir die Indizes ein.
	unsigned short ausIndex[36] = {7, 3, 0,   4, 7, 0,	// Vorderseite
								   5, 1, 2,   6, 5, 2,	// Hinterseite
								   4, 0, 1,   5, 4, 1,	// Linke Seite
								   6, 2, 3,   7, 6, 3,	// Rechte Seite
								   2, 1, 0,   3, 2, 0,	// Oberseite
								   4, 5, 6,   7, 4, 6};	// Unterseite

	// In das globale Array kopieren
	memcpy(m_ausSkyboxIndex, ausIndex, 36 * sizeof(unsigned short));
	

	return FM_OK;
}

fmResult fmSkyBox::Render()
{
	fmDirect3D& D3D = fmDirect3D::Instance();
	fmMatrix mWorld;
	// ------------------------------------------------------------------

	// Zuerst zeichnen wir die Sky-Box. Zu Beginn muss das Vertexformat gesetzt werden.
	D3D->SetFVF(SSkyboxVertex::dwFVF);

	// Die Beleuchtung und Z-Buffering schalten wir ab.
	D3D->SetRenderState(D3DRS_LIGHTING, FALSE);
	D3D->SetRenderState(D3DRS_ZENABLE, FALSE);
	D3D->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// In die erste Texturschicht setzen wir die Umgebungstextur.
	D3D->SetTexture(0, m_pEnvMap);
	D3D->SetTexture(1, NULL);
	D3D->SetTexture(2, NULL);
	D3D->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	D3D->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// Jetzt zeichnen wir die Sky-Box mit DrawIndexedPrimitiveUP.
	if(FAILED(D3D->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,			// Dreiecksliste
		                                  0,							// Beginn bei 0
										  8,							// Größter Index
										  12,					    	// Anz. Dreiecke
		                                  m_ausSkyboxIndex,		    	// Indizes
										  D3DFMT_INDEX16,			    // 16 Bits
										  m_aSkyboxVertex,			    // Vertizes
										  sizeof(SSkyboxVertex))))		// Vertexgröße
	{
		// Fehler beim Zeichnen!
		FM_ERROR_B("DrawIndexedPrimitiveUP", FM_ERROR);
	}

	// Die Einstellungen wieder zurücksetzen
	D3D->SetRenderState(D3DRS_LIGHTING, TRUE);
	D3D->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return FM_OK;
}

fmResult fmSkyBox::Update()
{
	return FM_OK;
}