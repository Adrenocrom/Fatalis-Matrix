/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmTerrain.cpp
===============================
*/

#include <FatalisMatrix.h>

fmTerrain::fmTerrain()
{
    m_pVB = NULL;
    m_pIB = NULL;

    m_pTexture       = NULL;
    m_pDetailTexture = NULL;

    m_iWidth  = 0;
    m_iLength = 0;
}

fmTerrain::~fmTerrain()
{
	for(int i=0; i < m_iLength; i++)
    {
        FM_SAFE_DELETE_ARRAY(m_fHeight[i]);
    }

	FM_SAFE_DELETE_ARRAY(m_fHeight);

	FM_SAFE_RELEASE(m_pTexture);
	FM_SAFE_RELEASE(m_pDetailTexture);

	FM_SAFE_RELEASE(m_pVB);
	FM_SAFE_RELEASE(m_pIB);
}

fmResult fmTerrain::Create(char* pcHeightmap,
						   char* pcTerrainTexture,
						   char* pcDetailTexture)
{
	fmDirect3D& D3D = fmDirect3D::Instance();

	// Zaehlvariablen fuer die Schleifen und Indizes
    int i, j, k;

	PDIRECT3DSURFACE9 pSurface = NULL;
    
    D3DXIMAGE_INFO ImgInfo;
    
    // Informationen ueber die Heightmap holen    
    D3DXGetImageInfoFromFile(pcHeightmap, &ImgInfo);
    
    // Breite und Laenge der Landschaft speichern    
    m_iWidth  = ImgInfo.Width;
    m_iLength = ImgInfo.Height;

    // Oberflaeche fuer die Heightmap anlegen
    D3D->CreateOffscreenPlainSurface(m_iWidth, 
                                     m_iLength, 
                                     D3DFMT_L8, 
                                     D3DPOOL_SCRATCH, 
                                     &pSurface, 0);

    // Heightmap laden
    D3DXLoadSurfaceFromFile(pSurface, 0, 0, pcHeightmap, 0, D3DX_DEFAULT, 0, 0);

    // Anzahl der Vertices und Indizes berechnen
    int iSizeVertices = m_iWidth * m_iLength;
    int iSizeIndices  = (m_iWidth-1)*(m_iLength-1)*2*3;

    // Vertex und Index Buffer anlegen
    D3D->CreateVertexBuffer(iSizeVertices * sizeof(TERRAIN_VERTEX), 
                            0, D3DFVF_TERRAINVERTEX, 
                            D3DPOOL_DEFAULT, 
                            &m_pVB, 
                            0);

    D3D->CreateIndexBuffer(iSizeIndices * 2, 
                           0, 
                           D3DFMT_INDEX16, 
                           D3DPOOL_DEFAULT, 
                           &m_pIB, 
                           0);

    if(FAILED(D3DXCreateTextureFromFile(D3D.GetDevice(), pcTerrainTexture, &m_pTexture)))
    {
		FM_ERROR_B("Fehler beim Laden der Textur", FM_ERROR);
    }

    if(FAILED(D3DXCreateTextureFromFile(D3D.GetDevice(), pcDetailTexture, &m_pDetailTexture)))
    {
		FM_ERROR_B("Fehler beim Laden der Detail-Textur", FM_ERROR);
    }


    

    // Oberflaeche sperren
    D3DLOCKED_RECT LockedRect;
    
    pSurface->LockRect(&LockedRect, 0, 0);

    // und einen Zeiger auf die Pixel anlegen
    BYTE* lpHeights = (BYTE*) LockedRect.pBits;

    int nPitch = LockedRect.Pitch;

    // die Vertices anlegen

    TERRAIN_VERTEX* Vertices = new TERRAIN_VERTEX[iSizeVertices];

    // und mit Werten belegen
    for(i=0;i<m_iLength;i++)
    {
        // der Farbwert des Pixels bestimmt die Hoehe
        for(j=0;j<m_iWidth;j++)
        {
            Vertices[i*m_iWidth+j].x = static_cast<float>(j) * 6.0f;

            Vertices[i*m_iWidth+j].y = static_cast<float>(lpHeights[i*nPitch+j]) / 5.0f;

            Vertices[i*m_iWidth+j].z = static_cast<float>(m_iLength - i) * 6.0f;

            // Texturkoordinaten vergeben            
            Vertices[i*m_iWidth+j].tu1 = static_cast<float>(j) / m_iWidth;
            Vertices[i*m_iWidth+j].tv1 = static_cast<float>(i) / m_iLength;

            Vertices[i*m_iWidth+j].tu2 = static_cast<float>(j) / m_iWidth * 50.0f;
            Vertices[i*m_iWidth+j].tv2 = static_cast<float>(i) / m_iLength * 50.0f;
        }
    }
    // Array fuer die Indizes anlegen
    unsigned short* Indices = new unsigned short[iSizeIndices];

    // und die Indizes der Dreiecke erzeugen
    k = 0;

    for(i = 0; i < m_iLength - 1; i++)
    {
        for(j = 0; j < m_iWidth - 1; j++)
        {
            Indices[k++] = m_iWidth * (i + 1) + j;
            Indices[k++] = m_iWidth *  i + j;
            Indices[k++] = m_iWidth * (i + 1) + j + 1;
            Indices[k++] = m_iWidth *  i + j;
            Indices[k++] = m_iWidth *  i + j + 1;
            Indices[k++] = m_iWidth * (i + 1) + j + 1;
        }
    }

    // Vertices in den Vertex Buffer kopieren
    void* pVertices;

    m_pVB->Lock(0, iSizeVertices * sizeof(TERRAIN_VERTEX), &pVertices, 0);

    memcpy(pVertices, Vertices, iSizeVertices * sizeof(TERRAIN_VERTEX));

    m_pVB->Unlock();

    // Indizes in den Index Buffer kopieren
    void* pIndices;

    m_pIB->Lock(0, iSizeIndices * sizeof(unsigned short), &pIndices, 0);
    
    memcpy(pIndices, Indices, iSizeIndices*sizeof(unsigned short));

    m_pIB->Unlock();



	m_fHeight = new float*[m_iLength];

    for(i = 0; i < m_iLength; i++)
    {
        m_fHeight[i] = new float[m_iWidth];
    }

    for(i = 0; i < m_iLength; i++)
    {
        for(int j=0;j<m_iWidth;j++)
        {
            m_fHeight[(m_iLength-1)-i][j] = static_cast<float>(lpHeights[i * nPitch + j]) / 30.0f;
        }
    }

	// Speicher freigeben
	FM_SAFE_DELETE_ARRAY(Vertices);
	FM_SAFE_DELETE_ARRAY(Indices);

    // Oberflaeche freigeben
	FM_SAFE_RELEASE(pSurface);

	return FM_OK;
}

fmResult fmTerrain::Render()
{
	fmDirect3D& D3D = fmDirect3D::Instance();

	// Vertex-Format setzen
    D3D->SetFVF(D3DFVF_TERRAINVERTEX);

    // Vertex Buffer angeben
    D3D->SetStreamSource(0, m_pVB, 0, sizeof(TERRAIN_VERTEX));
    
    // Index Buffer angeben
    D3D->SetIndices(m_pIB);

    // Landschaftstextur angeben
    D3D->SetTexture(0, m_pTexture);

    // Detailtextur angeben
    D3D->SetTexture(1, m_pDetailTexture);

    // Methode, mit der die Texturen gemischt werden sollen
    D3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    D3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    D3D->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    D3D->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
    D3D->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);

	D3D->SetTexture(2, NULL);
	
	// speculare Lichtanteile bei der Beleuchtung berücksichtigen:
	D3D->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	// Beleuchtung einschalten:
	D3D->SetRenderState(D3DRS_LIGHTING, FALSE);

		// z-Buffer einschalten:
	D3D->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	// Ambiente Beleuchtung einschalten
	D3D->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(120, 120, 120) );


    // Landschaft rendern
    D3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
                              0, 0, 
                              m_iLength * m_iWidth, 
                              0, 
                              (m_iWidth-1)*(m_iLength-1) * 2);
    
    D3D->SetTexture(0, NULL);
    D3D->SetTexture(1, NULL);

	return FM_OK;
}

float fmTerrain::HeightAtMap(fmVector3 vPos)
{
	// Berechnung der Position des Spielers in der Heightmap
    int z = static_cast<int>(vPos.z / 6.0f);
    int x = static_cast<int>(vPos.x / 6.0f);

    // nur wenn sich der Spieler innerhalb des Terrains befindet, 
    // wird die Hoehe angepasst
    if(z < 0 || z >= m_iLength -1)
    {
        z = 0;

		return 0.0f;
    }
    else if(x < 0 || x >= m_iWidth -1)
    {
        x = 0;

		return 0.0f;
    }
    else
    {
        // Berechnen, auf welchem Dreieck sich der
        // Spieler befindet
        fmVector2 A(static_cast<float>(x+1), static_cast<float>(z));
        fmVector2 B(static_cast<float>(x),   static_cast<float>(z+1));

        fmVector2 Point(vPos.x / 6.0f, vPos.z / 6.0f);

        // oberes oder unteres Dreieck
        float r = PointLine(A, B, Point);

        float p1;
        float p2;
        float p3;

        // Spieler befindet sich auf unterem Dreieck
        if(r > 0)
        {
            p1 = m_fHeight[z+1][x];
            p2 = m_fHeight[z]  [x];
            p3 = m_fHeight[z  ][x+1];

        }
        // Spieler befindet sich auf oberem Dreieck
        else
        {
            p1 = m_fHeight[z+1][x];
            p2 = m_fHeight[z+1][x+1];
            p3 = m_fHeight[z  ][x+1];
        }

        // die Eckpunkte des Dreiecks berechnen
        D3DXVECTOR3 vTrianlge1(static_cast<float>(x),   p1, static_cast<float>(z+1));
        D3DXVECTOR3 vTrianlge2(static_cast<float>(x),   p2, static_cast<float>(z));
        D3DXVECTOR3 vTrianlge3(static_cast<float>(x+1), p3, static_cast<float>(z));

        D3DXVECTOR3 v1 = vTrianlge3 + vTrianlge2;
        D3DXVECTOR3 v2 = vTrianlge2 + vTrianlge1;

        // die Normale des Dreiecks berechnen
        D3DXVECTOR3 vNormal;

        D3DXVec3Cross(&vNormal, &v1, &v2);

        // eine Ebene definieren, in der das Dreieck liegt
        D3DXPLANE Plane(vNormal.x, vNormal.y, vNormal.z, 0.0f);


        // testen, wo ein Strahl der von der Postion y = 1000 bis y = -1000
        // durch die Ebene geschickt wird auftrifft. Dies ist die Hoehe des
        // Spielers
        D3DXVECTOR3 Out;
        D3DXVECTOR3 V1(vPos.x, 1000.0f, vPos.z);
        D3DXVECTOR3 V2(vPos.x, -1000.0f, vPos.z);

        D3DXPlaneIntersectLine(&Out, &Plane, &V1, &V2);

        return (Out.y + 1.8f + vPos.y) / 2;
    }

	
}

// brechnen, ob der uebergebe Punkt P links oder rechts der Line AB liegt
float fmTerrain::PointLine(fmVector2 VecA, fmVector2 VecB, fmVector2 PointToTest)
{
	fmVector2 v1 = VecB - VecA;
	fmVector2 v2 = PointToTest - VecA;

	return (v1.x * v2.y - v1.y * v2.x);
}
