/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmModel.cpp
===============================
*/

#include <FatalisMatrix.h>

fmModel::fmModel()
{

}

fmResult fmModel::Init(char* pcFilename, BOOL bExtraData, BOOL bExtraDataOnly)
{
	fmDirect3D& D3D = fmDirect3D::Instance();
	FILE* pFile;
	char  strBuffer[200];

	m_bExtraData = bExtraData;
	m_bExtraDataOnly = bExtraDataOnly;

	if((pFile = fopen(pcFilename, "r")) == NULL)
	{
		FM_ERROR_B("Konnte das Model nicht finden!", FM_NOT_FOUND);
		fclose(pFile);
	}

		fscanf(pFile, "%s", strBuffer);	// Text ausblenden
		fscanf(pFile, "%d", &m_dwNumVertices);

		if(m_bExtraData)
		{
			m_pvVectors = new fmVector3[ m_dwNumVertices ];
		}

		D3D->CreateVertexBuffer(m_dwNumVertices * sizeof(fmModelVertex),
							    D3DUSAGE_WRITEONLY,
								D3DFVF_FMMODELVERTEX,
								D3DPOOL_MANAGED,
								&m_pVertexBuffer,
								NULL);

		m_pVertexBuffer->Lock(0, 0, (void**)(&m_pVertices), D3DLOCK_NOSYSLOCK);

			fscanf(pFile, "%s", strBuffer);	// Text ausblenden
			// Position
			for(unsigned int i = 0; i < m_dwNumVertices; i++)
			{
				fscanf(pFile, "%f", &m_pVertices[i].vPosition.x);
				fscanf(pFile, "%f", &m_pVertices[i].vPosition.y);
				fscanf(pFile, "%f", &m_pVertices[i].vPosition.z);

				m_pVertices[i].dwColor = fmColor(1.0f, 1.0f, 1.0f);

				if(m_bExtraData)
				{
					m_pvVectors[i] = m_pVertices[i].vPosition;
				}
			}

			fscanf(pFile, "%s", strBuffer);	// Text ausblenden
			// Normalen
			for(i = 0; i < m_dwNumVertices; i++)
			{
				fscanf(pFile, "%f", &m_pVertices[i].vNormal.x);
				fscanf(pFile, "%f", &m_pVertices[i].vNormal.y);
				fscanf(pFile, "%f", &m_pVertices[i].vNormal.z);
			}

			fscanf(pFile, "%s", strBuffer);	// Text ausblenden
			// Textur 1
			for(i = 0; i < m_dwNumVertices; i++)
			{
				fscanf(pFile, "%f", &m_pVertices[i].tu1);
				fscanf(pFile, "%f", &m_pVertices[i].tu1);
			}

			fscanf(pFile, "%s", strBuffer);	// Text ausblenden
			// Textur 2
			for(i = 0; i < m_dwNumVertices; i++)
			{
				fscanf(pFile, "%f", &m_pVertices[i].tu2);
				fscanf(pFile, "%f", &m_pVertices[i].tu2);
			}	
	
		m_pVertexBuffer->Unlock();

		fscanf(pFile, "%s", strBuffer);	// Text ausblenden
		fscanf(pFile, "%d", &m_dwNumIndices);

		D3D->CreateIndexBuffer(m_dwNumIndices * sizeof(WORD),
							   D3DUSAGE_WRITEONLY,
							   D3DFMT_INDEX16,
							   D3DPOOL_MANAGED,
							   &m_pIndexBuffer,
							   NULL);

		m_pIndexBuffer->Lock(0, 0, (void**)(&m_pdwIndices), D3DLOCK_NOSYSLOCK);

			for(i = 0; i < m_dwNumIndices; i++)
			{
				fscanf(pFile, "%d", &m_pdwIndices[i]);
			}

		m_pIndexBuffer->Unlock();

		m_dwNumTriangles = m_dwNumIndices / 3;
		
	fclose(pFile);

	if( m_bExtraDataOnly )
	{
		FM_SAFE_RELEASE(m_pVertexBuffer);
		FM_SAFE_RELEASE(m_pIndexBuffer);
	}

	if(m_bExtraData)
	{
		fmVector3 vTriA;
		fmVector3 vTriB;
		fmVector3 vTriC;
		fmVector3 vTemp;

		m_pTrianglePlanes = new fmPlane[ m_dwNumIndices / 3 * 4 ];

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
	}

	return FM_OK;
}

fmResult fmModel::Render()
{
	fmDirect3D& D3D = fmDirect3D::Instance();

	D3D->SetFVF(D3DFVF_FMMODELVERTEX);
	D3D->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(fmModelVertex));
	D3D->SetIndices(m_pIndexBuffer);

	HRESULT hResult;

	hResult = D3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,	// Dreiecksliste
						                0,					// Basisvertexindex
										0,					// Der kleinste Index
										m_dwNumVertices,	// Diff. zw. größtem u. kleinstem Index
										0,					// Von Anfang an zeichnen
										m_dwNumTriangles);	// Anzahl der Dreiecke

	return FM_OK;
}

fmResult fmModel::Exit()
{
	if( m_bExtraData )
	{
		FM_SAFE_DELETE_ARRAY(m_pvVectors);
		FM_SAFE_DELETE_ARRAY(m_pTrianglePlanes);
	}

	FM_SAFE_RELEASE(m_pVertexBuffer);
	FM_SAFE_RELEASE(m_pIndexBuffer);

	return FM_OK;
}

		

	