/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmMesh.cpp
===============================
*/

#include <FatalisMatrix.h>

// ******************************************************************
// Konstruktor der fmMesh-Klasse
fmMesh::fmMesh()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmMesh));
}
/*
// ******************************************************************
// Laden aus einer Datei
fmResult fmMesh::Init(char* pcFilename,
				      char* pcTexturPrefix,
				      char* pcTexturPostfix,
				      BOOL bGenerateExtraData,
				      BOOL bExtraDataOnly)
{
	m_bExtraData = bGenerateExtraData;
	m_bExtraDataOnly = bExtraDataOnly;

    LPD3DXBUFFER mD3DXMtrlBuffer;
	
	// Laden des Meshs aus Externer Datei
	if( FAILED( D3DXLoadMeshFromX( pcFilename,
								   D3DXMESH_SYSTEMMEM,
                                   fmDirect3D::Instance().GetDevice(),
								   NULL,
                                   &mD3DXMtrlBuffer,
								   NULL,
								   &m_dwNumMaterials,
                                   &m_pMesh ) ) )
    {
            FM_ERROR_B("Fehler beim Erstellen des Models!", FM_ERROR);
    }


	// Material erstellen
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)mD3DXMtrlBuffer->GetBufferPointer();
    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	// Texturen Laden
	for( DWORD i=0; i<m_dwNumMaterials; i++ )
    {
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

        m_pMeshTextures[i] = NULL;

        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
			if(FAILED( D3DXCreateTextureFromFileEx(fmDirect3D::Instance().GetDevice(),
												   d3dxMaterials[i].pTextureFilename,
												   D3DX_DEFAULT,
												   D3DX_DEFAULT,
												   D3DX_DEFAULT,
												   0,	
												   D3DFMT_UNKNOWN,	
												   D3DPOOL_MANAGED,	
												   D3DX_DEFAULT,
												   D3DX_DEFAULT,
												   0,	
												   NULL,
												   NULL,
												   &m_pMeshTextures[i])))
														
            {
					FM_ERROR_A(d3dxMaterials[i].pTextureFilename);
                    FM_ERROR_B("Textur nicht gefunden!", FM_ERROR);
            }
        }
    }

	FM_SAFE_RELEASE(mD3DXMtrlBuffer);
	
	// Informationen über das Mesh gewinnen
	m_dwNumVertices		= m_pMesh->GetNumVertices();
	m_dwNumTriangles    = m_pMesh->GetNumFaces();
	m_dwNumIndices		= m_dwNumTriangles * 3;

	// Sollen zusatzinformationen erzeugt werden
	if(m_bExtraData)
	{
		// Speicher für Positionsvectoren Reservieren
		m_pvVectors = (fmVector3*)(malloc(m_dwNumVertices * sizeof(fmVector3)));
		if(!m_pvVectors) FM_ERROR_B("Es steht nicht genügend Arbeitsspeicher zur Verfügung!", FM_ERROR);

		// Nun für die Indizes
		m_pdwIndices = (DWORD*)(malloc(m_dwNumIndices * sizeof(DWORD)));
		if(!m_pdwIndices) FM_ERROR_B("Es steht nicht genügend Arbeitsspeicher zur Verfügung!", FM_ERROR);

		// Speicher für die Ebenen der Dreiecke reservieren
		m_pTrianglePlanes = (fmPlane*)(malloc(m_dwNumIndices / 3 * 4 * sizeof(fmPlane)));
		if(!m_pTrianglePlanes) FM_ERROR_B("Es steht nicht genügend Arbeitsspeicher zur Verfügung!", FM_ERROR);
	
		UpdateExtraData();
	}

	return FM_OK;
}
*/
// **************************************************************************
// Laden aus einer Datei, mit BumpMap
fmResult  fmMesh::Init(char* pcFilename,
					   PDIRECT3DCUBETEXTURE9* pEnvMap,
					   char* pcBumpMapFilename,
					   BOOL	bBumpMap,				   
					   char* pcTexturPrefix,
					   char* pcTexturPostfix,
				       BOOL bGenerateExtraData,
					   BOOL bExtraDataOnly)
{
	m_bExtraData = bGenerateExtraData;
	m_bExtraDataOnly = bExtraDataOnly;
	m_bBumpMap = bBumpMap;

    LPD3DXBUFFER mD3DXMtrlBuffer;
	
	// Laden des Meshs aus Externer Datei
	if( FAILED( D3DXLoadMeshFromX( pcFilename,
								   D3DXMESH_SYSTEMMEM,
                                   fmDirect3D::Instance().GetDevice(),
								   NULL,
                                   &mD3DXMtrlBuffer,
								   NULL,
								   &m_dwNumMaterials,
                                   &m_pMesh ) ) )
    {
            FM_ERROR_B("Fehler beim Erstellen des Models!", FM_ERROR);
    }


	// Material erstellen
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)mD3DXMtrlBuffer->GetBufferPointer();
    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	// Texturen Laden
	for( DWORD i=0; i<m_dwNumMaterials; i++ )
    {
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

        m_pMeshTextures[i] = NULL;

        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
			if(FAILED( D3DXCreateTextureFromFileEx(fmDirect3D::Instance().GetDevice(),
												   d3dxMaterials[i].pTextureFilename,
												   D3DX_DEFAULT,
												   D3DX_DEFAULT,
												   D3DX_DEFAULT,
												   0,	
												   D3DFMT_UNKNOWN,	
												   D3DPOOL_MANAGED,	
												   D3DX_DEFAULT,
												   D3DX_DEFAULT,
												   0,	
												   NULL,
												   NULL,
												   &m_pMeshTextures[i])))
														
            {
					FM_ERROR_A(d3dxMaterials[i].pTextureFilename);
                    FM_ERROR_B("Textur nicht gefunden!", FM_ERROR);
            }
        }
    }

	if(m_bBumpMap)
	{
		m_ppEnvMap = pEnvMap;

		if(FAILED( D3DXCreateTextureFromFileEx(fmDirect3D::Instance().GetDevice(),
											   pcBumpMapFilename,
											   D3DX_DEFAULT,
											   D3DX_DEFAULT,
											   D3DX_DEFAULT,
											   0,	
											   D3DFMT_UNKNOWN,	
											   D3DPOOL_MANAGED,	
											   D3DX_DEFAULT,
											   D3DX_DEFAULT,
											   0,	
											   NULL,
											   NULL,
											   &m_pBumpMap)))
														
            {
					FM_ERROR_A(pcBumpMapFilename);
                    FM_ERROR_B("Textur nicht gefunden!", FM_ERROR);
            }

	}

	FM_SAFE_RELEASE(mD3DXMtrlBuffer);
	
	// Informationen über das Mesh gewinnen
	m_dwNumVertices		= m_pMesh->GetNumVertices();
	m_dwNumTriangles    = m_pMesh->GetNumFaces();
	m_dwNumIndices		= m_dwNumTriangles * 3;

	// Sollen zusatzinformationen erzeugt werden
	if(m_bExtraData)
	{
		// Speicher für Positionsvectoren Reservieren
		m_pvVectors = (fmVector3*)(malloc(m_dwNumVertices * sizeof(fmVector3)));
		if(!m_pvVectors) FM_ERROR_B("Es steht nicht genügend Arbeitsspeicher zur Verfügung!", FM_ERROR);

		// Nun für die Indizes
		m_pdwIndices = (DWORD*)(malloc(m_dwNumIndices * sizeof(DWORD)));
		if(!m_pdwIndices) FM_ERROR_B("Es steht nicht genügend Arbeitsspeicher zur Verfügung!", FM_ERROR);

		// Speicher für die Ebenen der Dreiecke reservieren
		m_pTrianglePlanes = (fmPlane*)(malloc(m_dwNumIndices / 3 * 4 * sizeof(fmPlane)));
		if(!m_pTrianglePlanes) FM_ERROR_B("Es steht nicht genügend Arbeitsspeicher zur Verfügung!", FM_ERROR);
	
		UpdateExtraData();
	}

	return FM_OK;
}

// ******************************************************************
// Model rendern
fmResult fmMesh::Render()
{
	fmDirect3D&	D3D = fmDirect3D::Instance();

	if(!m_bExtraDataOnly)
	{
		for( DWORD i=0; i<m_dwNumMaterials; i++ )
		{
            
			D3D->SetMaterial( &m_pMeshMaterials[i] );
			D3D->SetTexture( 0, m_pMeshTextures[i] );

			// Bumpmap
			if( m_bBumpMap )
			{
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
				D3D->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
				D3D->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_CURRENT);
				D3D->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			}
			else
			{
				D3D->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				D3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
				D3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			
				D3D->SetTexture(1, NULL);
				D3D->SetTexture(2, NULL);
			}

			m_pMesh->DrawSubset( i );
		}
	}

	return FM_OK;
}

// ******************************************************************
// Extra Daten des Models berechnen
fmResult fmMesh::UpdateExtraData()
{
	// Hat das Model platz für Extra daten?
	if(!m_bExtraData) FM_ERROR_B("Das Model wurde ohne unterstützung von Extradaten geladen!", FM_ERROR);

	PDIRECT3DVERTEXBUFFER9 pVertexBuffer;
	D3DVERTEXBUFFER_DESC    VertexDesc;
	void*				   pVBuffer;

	PDIRECT3DINDEXBUFFER9 pIndexBuffer;
	D3DINDEXBUFFER_DESC	   IndexDesc;
	void*				  pIBuffer;
	
	m_pMesh->GetVertexBuffer(&pVertexBuffer);
	m_pMesh->GetIndexBuffer(&pIndexBuffer);

	pVertexBuffer->GetDesc(&VertexDesc);
	pIndexBuffer->GetDesc(&IndexDesc);


	pVBuffer = malloc(VertexDesc.Size);
	if(!pVBuffer)
	{
		FM_ERROR_B("Nicht genügend speicher vorhanden!", FM_ERROR);
	}

	if(!(VertexDesc.Usage & D3DUSAGE_WRITEONLY))
	{
		BYTE* pVBData;
		if(SUCCEEDED(pVertexBuffer->Lock(0, 0,
										(void**)(&pVBData),
										D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY)))
		{
			// Kopieren und entsperren
			memcpy(pVBuffer, pVBData, VertexDesc.Size);
			pVertexBuffer->Unlock();
		}
		else FM_ERROR_B("Vertex-Buffer konnte nicht gesperrt werden!", FM_ERROR);
	}

	BYTE* pVector = (BYTE*)(pVBuffer);

	for(DWORD i = 0; i < m_dwNumVertices; i++)
	{
		m_pvVectors[i] = *((fmVector3*)(pVector));
		pVector += m_pMesh->GetNumBytesPerVertex();
	}

	pIBuffer = malloc(IndexDesc.Size);
	if(!pIBuffer)
	{
		FM_ERROR_B("Nicht genügend speicher vorhanden!", FM_ERROR);
	}

	// Index-Buffer sperren und die Daten kopieren
	if(!(IndexDesc.Usage & D3DUSAGE_WRITEONLY))
	{
		BYTE* pIBData;
		if(SUCCEEDED(pIndexBuffer->Lock(0, 0,
										(void**)(&pIBData),
										D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY)))
		{
			// Kopieren und entsperren
			memcpy(pIBuffer, pIBData, IndexDesc.Size);
			pIndexBuffer->Unlock();
		}
		else FM_ERROR_B("Index-Buffer konnte nicht gesperrt werden!", FM_ERROR);
	}

	// Die Indizes kopieren
	BYTE* pIndex = (BYTE*)(pIBuffer);
	for(i = 0; i < m_dwNumIndices; i++)
	{
		// Index herausfinden
		if(IndexDesc.Format == D3DFMT_INDEX16) m_pdwIndices[i] = *((WORD*)(pIndex));
		else if(IndexDesc.Format == D3DFMT_INDEX32) m_pdwIndices[i] = *((DWORD*)(pIndex));
		else m_pdwIndices[i] = 0;

		if(IndexDesc.Format == D3DFMT_INDEX16)	pIndex += 2;
		if(IndexDesc.Format == D3DFMT_INDEX32)	pIndex += 4;
	}


	FM_SAFE_MEMFREE(pVBuffer);
	FM_SAFE_MEMFREE(pIBuffer);
	FM_SAFE_RELEASE(pVertexBuffer);
	FM_SAFE_RELEASE(pIndexBuffer);

	fmVector3 vTriA;
	fmVector3 vTriB;
	fmVector3 vTriC;
	fmVector3 vTemp;

	// Jeweils vier Ebenen pro Dreieck berechnen
	for(i = 0; i < m_dwNumIndices / 3; i++)
	{
		// Die drei Vektoren des Dreiecks kopieren
		vTriA = m_pvVectors[m_pdwIndices[i * 3]];
		vTriB = m_pvVectors[m_pdwIndices[i * 3 + 1]];
		vTriC = m_pvVectors[m_pdwIndices[i * 3 + 2]];

		// Ebene des Dreiecks berechnen
		m_pTrianglePlanes[i * 4] = fmPlaneNormalize(fmPlaneFromPoints(vTriA, vTriB, vTriC));

		// Die drei Seitenebenen berechnen
		vTemp = fmVector3Normalize(fmVector3Cross(vTriA - vTriB, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 1] = fmPlaneFromPointNormal(vTriA, vTemp);

		vTemp = fmVector3Normalize(fmVector3Cross(vTriB - vTriC, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 2] = fmPlaneFromPointNormal(vTriB, vTemp);

		vTemp = fmVector3Normalize(fmVector3Cross(vTriC - vTriA, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 3] = fmPlaneFromPointNormal(vTriC, vTemp);
	}

	return FM_OK;
}

// ******************************************************************
// Daten wieder freigeben
fmResult fmMesh::Exit()
{
	if(m_bExtraData)
	{
		// Extradaten löschen
		FM_SAFE_MEMFREE(m_pvVectors);
		FM_SAFE_MEMFREE(m_pdwIndices);
		FM_SAFE_MEMFREE(m_pTrianglePlanes);
	}

	if( m_bBumpMap )
	{
		FM_SAFE_RELEASE(m_pBumpMap);
	}

	// Matrialien löschen
	FM_SAFE_DELETE_ARRAY(m_pMeshMaterials);

	if( m_pMeshTextures )
    {
      for( DWORD i = 0; i < m_dwNumMaterials; i++ )
        {
            FM_SAFE_RELEASE(m_pMeshTextures[i]);
        }
        FM_SAFE_DELETE_ARRAY(m_pMeshTextures);
    }

	// Das Mesh freigeben
	FM_SAFE_RELEASE(m_pMesh);

	return FM_OK;
}

// ******************************************************************
fmResult fmMesh::GetIndexBuffer(PDIRECT3DINDEXBUFFER9* pIndexBuffer)
{
	m_pMesh->GetIndexBuffer(pIndexBuffer);

	return FM_OK;
}

// ******************************************************************
fmResult fmMesh::GetVertexBuffer(PDIRECT3DVERTEXBUFFER9* pVertexBuffer)
{
	m_pMesh->GetVertexBuffer(pVertexBuffer);

	return FM_OK;
}

// ******************************************************************