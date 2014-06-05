/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmSimpleMesh.cpp
===============================
*/

#include <FatalisMatrix.h>

fmSimpleMesh::fmSimpleMesh()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmSimpleMesh));

	m_pMesh	   = NULL;
	m_pTexture = NULL;
	m_pBumpMap = NULL;
	m_ppEnvMap = NULL;
}

fmResult fmSimpleMesh::Init(char* pcFilename,
							char* pcTextureFilename,
							char* pcBumpMapFilename,
						    PDIRECT3DCUBETEXTURE9* pEnvMap)
{
	// Alles zurücksetzen
	Exit();

	// Das Modelladen laden
	if(FAILED(D3DXLoadMeshFromX(pcFilename,
		                        D3DXMESH_MANAGED,
								fmDirect3D::Instance().GetDevice(),
								NULL,
								NULL,
								NULL,
								NULL,
								&m_pMesh)))
	{
		// Fehler!
		FM_ERROR_B("D3DXLoadMeshFromX", FM_ERROR);
	}

	// Die Textur für das Mesh laden
	if(FAILED(D3DXCreateTextureFromFileEx(fmDirect3D::Instance().GetDevice(),	// Device
										  pcTextureFilename,					// Dateiname
										  D3DX_DEFAULT,							// Breite
										  D3DX_DEFAULT,							// Höhe
										  D3DX_DEFAULT,							// MIP-Maps
										  0,									// Verwendungszweck
										  D3DFMT_UNKNOWN,						// Format
										  D3DPOOL_MANAGED,						// Speicherklasse
										  D3DX_FILTER_NONE,						// Filter
										  D3DX_DEFAULT,							// MIP-Map-Filter
										  1,									// Color-Key
										  NULL,									// Unwichtig
										  NULL,									// Unwichtig
										  &m_pTexture)))						// Die Textur
	{
		// Fehler!
		FM_ERROR_B("D3DXCreateTextureFromFileEx", FM_ERROR);
	}

	// Die BumpMap laden
	if(FAILED(D3DXCreateTextureFromFileEx(fmDirect3D::Instance().GetDevice(),	// Device
										  pcTextureFilename,					// Dateiname
										  D3DX_DEFAULT,							// Breite
										  D3DX_DEFAULT,							// Höhe
										  D3DX_DEFAULT,							// MIP-Maps
										  0,									// Verwendungszweck
										  D3DFMT_UNKNOWN,						// Format
										  D3DPOOL_MANAGED,						// Speicherklasse
										  D3DX_FILTER_NONE,						// Filter
										  D3DX_DEFAULT,							// MIP-Map-Filter
										  1,									// Color-Key
										  NULL,									// Unwichtig
										  NULL,									// Unwichtig
										  &m_pBumpMap)))						// Die Textur
	{
		// Fehler!
		FM_ERROR_B("D3DXCreateTextureFromFileEx", FM_ERROR);
	}

	m_ppEnvMap = pEnvMap;


	return FM_OK;
}

fmResult fmSimpleMesh::Render()
{
	fmDirect3D&	D3D = fmDirect3D::Instance();
	D3DMATERIAL9	Material;

	Material.Diffuse	= fmColor(0.25f, 0.25f, 0.25f);
	Material.Ambient	= fmColor(0.25f, 0.25f, 0.25f);
	Material.Emissive	= fmColor(0.0f, 0.0f, 0.0f);
	D3D->SetMaterial(&Material);

	// Die Oberflächentextur
	D3D->SetTexture(0, m_pTexture);
	D3D->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	D3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	D3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	// Die Bump-Map: Hier setzen wir als Bump-Map-Matrix die Identitätsmatrix ein.
	// Die Du- und Dv-Werte werden also ohne Veränderung aus der Bump-Map übernommen.
	D3D->SetTexture(1, m_pBumpMap);
	D3D->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	D3D->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
	D3D->SetTextureStageState(1, D3DTSS_BUMPENVMAT00, F2DW(1.0f));
	D3D->SetTextureStageState(1, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
	D3D->SetTextureStageState(1, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
	D3D->SetTextureStageState(1, D3DTSS_BUMPENVMAT11, F2DW(1.0f));

	// Die Environment-Map wird so wie immer eingestellt.
	// Die Reflexion wird zur Textur hinzugefügt (D3DTOP_ADD).
	// Möglich wäre auch z.B. D3DTOP_MODULATE.
	D3D->SetTexture(2, *m_ppEnvMap);
	D3D->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
	D3D->SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
	D3D->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);
	D3D->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_CURRENT);
	D3D->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	m_pMesh->DrawSubset(0);

	return FM_OK;
}

fmResult fmSimpleMesh::Exit()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmSimpleMesh));

	FM_SAFE_RELEASE(m_pTexture);
	FM_SAFE_RELEASE(m_pBumpMap);
	FM_SAFE_RELEASE(m_pMesh);

	return FM_OK;
}

