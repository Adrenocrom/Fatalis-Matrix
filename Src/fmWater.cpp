/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmWater.cpp
===============================
*/

#include <FatalisMatrix.h>

fmWater::fmWater()
{
	m_iNumVertices = 0;
	m_iNumIndices  = 0;

	m_iWidth = 0;
	m_iLength = 0;

    m_pWaterVB = 0;
	m_pWaterIB = 0;
	m_pWaterEffect = NULL;
}

fmWater::~fmWater()
{
	FM_SAFE_RELEASE(m_pWaterVB);
	FM_SAFE_RELEASE(m_pWaterIB);

	FM_SAFE_RELEASE(m_apWaterTextures[0]);
	FM_SAFE_RELEASE(m_apWaterTextures[1]);
	FM_SAFE_RELEASE(m_apWaterTextures[2]);

	FM_SAFE_RELEASE(m_pWaterEffect);
}


fmResult fmWater::Create(int iWidth,
						 int iLength,
						 float fHeight,
						 char* pcEffect)
{
	fmDirect3D& D3D = fmDirect3D::Instance();
	int i, j;

	m_iWidth = iWidth;
	m_iLength = iLength;

	m_iNumVertices = m_iWidth * m_iLength;
	m_iNumIndices  = (m_iWidth-1)*(m_iLength-1) * 2 * 3;

	m_wIndexArray  = new WORD[m_iNumIndices];

	int k = 0;

	for(i = 0; i < m_iLength-1; i++)
    {
        for(j = 0; j < m_iWidth-1; j++)
        {
            m_wIndexArray[k++] = m_iWidth * (i + 1) + j;
            m_wIndexArray[k++] = m_iWidth *  i + j;
            m_wIndexArray[k++] = m_iWidth * (i + 1) + j + 1;
            m_wIndexArray[k++] = m_iWidth *  i + j;
            m_wIndexArray[k++] = m_iWidth *  i + j + 1;
            m_wIndexArray[k++] = m_iWidth * (i + 1) + j + 1;
        }
    }

	D3D->CreateIndexBuffer(m_iNumIndices * sizeof(WORD),
                           D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
                           D3DPOOL_MANAGED, &m_pWaterIB, NULL );

	// Indexbuffer mit Daten füllen:
    WORD* wVar = NULL;
    m_pWaterIB->Lock(0, 0, (VOID**)&wVar, 0);
    memcpy(wVar, m_wIndexArray, m_iNumIndices * sizeof(WORD));
    m_pWaterIB->Unlock();

	D3D->CreateVertexBuffer(m_iNumVertices * sizeof(WATER_VERTEX),
                            D3DUSAGE_WRITEONLY, D3DFVF_WATERVERTEX,
                            D3DPOOL_MANAGED, &m_pWaterVB, NULL );
	
	m_pWaterVB->Lock(0, 0, (VOID**)&m_pVertices, 0);


	// und mit Werten belegen
    for(i = 0; i < m_iLength; i++)
    {
        // der Farbwert des Pixels bestimmt die Hoehe
        for(j = 0; j < m_iWidth; j++)
        {
			// Position
            m_pVertices[i*m_iWidth+j].vPosition.x = static_cast<float>(j);
            m_pVertices[i*m_iWidth+j].vPosition.y = static_cast<float>(fHeight);
            m_pVertices[i*m_iWidth+j].vPosition.z = static_cast<float>(m_iLength - i);

			// Normal
			m_pVertices[i*m_iWidth+j].vNormal = m_pVertices[i*m_iWidth+j].vPosition;

            // Texturkoordinaten vergeben            
            m_pVertices[i*m_iWidth+j].tu1 = static_cast<float>(j);
            m_pVertices[i*m_iWidth+j].tv1 = static_cast<float>(i);

			// Texturkoordinaten vergeben            
            m_pVertices[i*m_iWidth+j].tu2 = static_cast<float>(j) / m_iWidth;
            m_pVertices[i*m_iWidth+j].tv2 = static_cast<float>(i) / m_iLength;
        }
    }

	m_pWaterVB->Unlock();

	// **********************************************************************
	// Effect laden und deren Inhalte benutzen
	D3DXEFFECT_DESC		EffectDesc;
	D3DXTECHNIQUE_DESC	TecDesc;
	D3DXHANDLE			hTechnique;
	const char*			pcString;
	char				acTexture[256];

	// ------------------------------------------------------------------

	// Nun laden wir den Effekt. 
	if(FAILED(D3DXCreateEffectFromFile(D3D.GetDevice(),
		                               pcEffect,
									   NULL,
									   NULL,
									   0,
									   NULL,
									   &m_pWaterEffect,
									   NULL)))
	{
		// Fehler! Womöglich ein Syntaxfehler in der Effektdatei!
		FM_ERROR_B("Konnte Effect nicht Laden!", FM_ERROR);
	}

	// ------------------------------------------------------------------

	// Die drei Texturen abfragen
	for(int iTex = 0; iTex < 3; iTex++)
	{
		// Den Dateinamen abfragen
		sprintf(acTexture, "TextureFilename%d", iTex + 1);
		if(SUCCEEDED(m_pWaterEffect->GetString(acTexture, &pcString)))
		{
			// Die Datei laden
			D3DXCreateTextureFromFile(D3D.GetDevice(), pcString, &m_apWaterTextures[iTex]);
		}
	}

	// ------------------------------------------------------------------

	// Die drei Texturvariablen des Effekts ausfüllen
	m_pWaterEffect->SetTexture("Texture1", m_apWaterTextures[0]);
	m_pWaterEffect->SetTexture("Texture2", m_apWaterTextures[1]);
	m_pWaterEffect->SetTexture("Texture3", m_apWaterTextures[2]);

	// ------------------------------------------------------------------

	// Effekteigenschaften abfragen
	m_pWaterEffect->GetDesc(&EffectDesc);

	// Alle Techniken durchgehen und prüfen.
	// Wenn eine gültige gefunden wird, nehmen wir sie.
	for(unsigned int uiTec = 0; uiTec < EffectDesc.Techniques; uiTec++)
	{
		// Handle der Technik abfragen
		hTechnique = m_pWaterEffect->GetTechnique(uiTec);

		// Ist die Technik gültig?
		if(SUCCEEDED(m_pWaterEffect->ValidateTechnique(hTechnique)))
		{
			// Ja, gültig! Aktivieren und abbrechen!
			m_pWaterEffect->SetTechnique(hTechnique);
			break;
		}
		else if(uiTec == EffectDesc.Techniques - 1)
		{
			// Dies war die letzte Technik! Da man sich auf die Ergebnisse
			// von ValidateTechnique leider nicht immer verlassen kann,
			// wenden wir einfach die erste Technik an - in der Hoffnung,
			// dass sie funktioniert.
			// SetTechnique muss nicht aufgerufen werden, da die erste
			// Technik immer von Anfang an aktiviert ist.
		}
	}

	// Eigenschaften der aktiven Technik abfragen
	m_pWaterEffect->GetTechniqueDesc(NULL, &TecDesc);

	return FM_OK;
}

fmResult fmWater::Render()
{
	fmDirect3D& D3D = fmDirect3D::Instance();
	unsigned int	uiNumPasses;

	// Vertex-Format setzen
    D3D->SetFVF(D3DFVF_WATERVERTEX);

    // Vertex Buffer angeben
    D3D->SetStreamSource(0, m_pWaterVB, 0, sizeof(WATER_VERTEX));
    
    // Index Buffer angeben
    D3D->SetIndices(m_pWaterIB);

	// speculare Lichtanteile bei der Beleuchtung berücksichtigen:
	D3D->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	// Beleuchtung einschalten:
	D3D->SetRenderState(D3DRS_LIGHTING, FALSE);

	// z-Buffer einschalten:
	D3D->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	// Ambiente Beleuchtung einschalten
	D3D->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(120, 120, 120) );

	// Beginnen und die Anzahl der Durchgänge abfragen. Wir geben 0 für den
	// zweiten Parameter an, was dazu führt, dass alle Einstellungen gespeichert
	// werden (Render-States, Sampler-States, Texturen und so weiter).
	m_pWaterEffect->Begin(&uiNumPasses, 0);
	
	// Für jeden Durchgang...
	for(unsigned int uiPass = 0; uiPass < uiNumPasses; uiPass++)
	{
		// Einstellungen aktivieren
		m_pWaterEffect->BeginPass(uiPass);

		// Wasser rendern
		D3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
								  0, 0, 
								  m_iLength * m_iWidth, 
								  0, 
								  (m_iWidth-1)*(m_iLength-1) * 2);
	
		m_pWaterEffect->EndPass();
	}

	// Ende - Einstellungen werden wiederhergestellt
	m_pWaterEffect->End();

	return FM_OK;
}

fmResult fmWater::Update(float fTime)
{
	m_fTime = fTime;

	WATER_VERTEX Vertex;
	fmVector3    vPosition;

	m_pWaterVB->Lock(0, 0, (VOID**)&m_pVertices, 0);

	// Jeden Vertex durchgehen
	for(WORD x = 0; x < m_iLength; x++)
	{
		for(WORD y = 0; y < m_iWidth; y++)
		{
			// Den Vertex generieren. Dazu wird die Position des Vertex berechnet
			// und dann als Parameter für die verschiedenen Rückruffunktionen verwendet.
			vPosition.x = ((float)(x) / (float)(m_iWidth - 1) - 0.5f) * 100.0f;
			vPosition.y = 0.0f;
			vPosition.z = ((float)(-y) / (float)(m_iLength - 1) + 0.5f) * 100.0f;

			// Rückruffunktionen aufrufen
			Vertex.vPosition = PositionProc(vPosition);
			Vertex.vNormal = NormalProc(vPosition);
			Vertex.tu1 = TextureProc(vPosition).u;
			Vertex.tv1 = TextureProc(vPosition).v;

			m_pVertices[x*m_iWidth+y] = Vertex;
		}
	}

	m_pWaterVB->Unlock();

	return FM_OK;
}

// ******************************************************************
// Rückruffunktion für die Vertexposition
fmVector3 fmWater::PositionProc(fmVector3 v)
{
	float fHeight;

	// Die Höhe berechnet sich mit verschiedenen Sinusfunktionen,
	// die abhängig von der Position des Vertex und abhängig von der Zeit sind.
	fHeight = (sinf(v.x * 0.2f + m_fTime * 3.0f) * cosf(v.z * 0.2f + m_fTime * 3.0f)) *
		      sinf(m_fTime + v.x * 0.1f + v.z * 0.1f) * 2.0f +
			  sinf(m_fTime) * 0.5f;

	// Positionsvektor liefern
	return fmVector3(v.x, fHeight, v.z);
}

// ******************************************************************
// Rückruffunktion für den Vertexnormalenvektor
fmVector3 fmWater::NormalProc(fmVector3 v)
{
	fmVector3 m;

	// Steigung berechnen
	m.x = (PositionProc(v + fmVector3(0.01f, 0.0f, 0.0f)).y - PositionProc(v).y) / 0.01f;
	m.z = (PositionProc(v + fmVector3(0.0f, 0.0f, 0.01f)).y - PositionProc(v).y) / 0.01f;

	// In Normalenvektor umwandeln
	return fmVector3Cross(fmVector3Normalize(fmVector3(1.0f, m.x, 0.0f)),
						  fmVector3Normalize(fmVector3(0.0f, m.z, 1.0f)));
}

// ******************************************************************
// Rückruffunktion für die Texturkoordinaten
fmVector2 fmWater::TextureProc(fmVector3 v)
{
	return fmVector2(v.x * 0.01f + sinf(m_fTime * 0.1f),
		             v.z * 0.01f + cosf(m_fTime * 0.1f));
}