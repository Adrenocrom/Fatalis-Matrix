/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmShadowVolume.cpp
===============================
*/

#include <FatalisMatrix.h>

DWORD fmShadowVertex::dwFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

fmShadowVolume::fmShadowVolume()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmShadowVolume));
}

fmResult fmShadowVolume::Init(fmMesh* pModel)
{
	// Die Klasseninstanz zurücksetzen.
	// Damit wird ermöglicht, dass der Init-Aufruf mehrere Male mit
	// derselben Instanz funktioniert. 
	Exit();

	// Model prüfen
	if(!pModel) return FM_ERROR;

	// Zeiger auf das Modell kopieren
	m_pModel = pModel;

	// Effekt für das Schattenvolumen erstellen
	m_pShadowVolumeEffect = new fmEffect;
	if(m_pShadowVolumeEffect->Init(// Diese beiden Techniken rufen oft unerklärliche Probleme hervor.
								   // Seltsamerweise funktionieren sie nur dann, wenn die Geräteschnittstelle
								   // mit D3DCREATE_PUREDEVICE erzeugt wurde oder wenn Direct3D im Debug-Modus läuft.
								   /*"TECHNIQUE T1\n"
								   "{\n"
								   "	PASS P1\n"
								   "	{\n"
								   "		ZEnable				= True;\n"
								   "		ZWriteEnable		= False;\n"
								   "		ShadeMode			= Flat;\n"
								   "        FogEnable			= False;\n"
								   "		ColorOp[0]			= SelectArg1;\n"
								   "		ColorArg1[0]		= Current;\n"
								   "		ColorOp[1]			= Disable;\n"
								   "		Lighting			= False;\n"
								   "		StencilEnable		= True;\n"
								   "		StencilFunc			= Always;\n"
								   "		StencilFail			= Keep;\n"
								   "		StencilZFail		= Keep;\n"
								   "		StencilPass			= Decr;\n"
								   "        StencilMask			= 0xFFFFFFFF;\n"
								   "        StencilWriteMask	= 0xFFFFFFFF;\n"
								   "		TwoSidedStencilMode	= True;\n"
								   "		CCW_StencilFunc		= Always;\n"
								   "		CCW_StencilFail		= Keep;\n"
								   "		CCW_StencilZFail	= Keep;\n"
								   "		CCW_StencilPass		= Incr;\n"
								   "		CullMode			= None;\n"
								   "		ColorWriteEnable	= 0;\n"
								   "	}\n"
								   "}\n"
								   "\n"
								   "TECHNIQUE T2\n"
								   "{\n"
								   "	PASS P1\n"
								   "	{\n"
								   "		ZEnable				= True;\n"
								   "		ZWriteEnable		= False;\n"
								   "		ShadeMode			= Flat;\n"
								   "        FogEnable			= False;\n"
								   "		ColorOp[0]			= SelectArg1;\n"
								   "		ColorArg1[0]		= Current;\n"
								   "		ColorOp[1]			= Disable;\n"
								   "		Lighting			= False;\n"
								   "		StencilEnable		= True;\n"
								   "		StencilFunc			= Always;\n"
								   "		StencilFail			= Keep;\n"
								   "		StencilZFail		= Keep;\n"
								   "		StencilPass			= Decr;\n"
								   "        StencilMask			= 0xFFFFFFFF;\n"
								   "        StencilWriteMask	= 0xFFFFFFFF;\n"
								   "		TwoSidedStencilMode	= True;\n"
								   "		CCW_StencilFunc		= Always;\n"
								   "		CCW_StencilFail		= Keep;\n"
								   "		CCW_StencilZFail	= Keep;\n"
								   "		CCW_StencilPass		= Incr;\n"
								   "		CullMode			= None;\n"
								   "		AlphaBlendEnable	= True;\n"
								   "		SrcBlend			= Zero;\n"
								   "		DestBlend			= One;\n"
								   "	}\n"
								   "}\n"
								   "\n"*/
								   "TECHNIQUE T3\n"
								   "{\n"
								   "	PASS P1\n"
								   "	{\n"
								   "		ZEnable				= True;\n"
								   "		ZWriteEnable		= False;\n"
								   "		ShadeMode			= Flat;\n"
								   "        FogEnable			= False;\n"
								   "		ColorOp[0]			= SelectArg1;\n"
								   "		ColorArg1[0]		= Current;\n"
								   "		ColorOp[1]			= Disable;\n"
								   "		Lighting			= False;\n"
								   "		StencilEnable		= True;\n"
								   "		StencilFunc			= Always;\n"
								   "		StencilFail			= Keep;\n"
								   "		StencilZFail		= Keep;\n"
								   "		StencilPass			= Decr;\n"
								   "        StencilMask			= 0xFFFFFFFF;\n"
								   "        StencilWriteMask	= 0xFFFFFFFF;\n"
								   "		CullMode			= CCW;\n"
								   "		ColorWriteEnable	= 0;\n"
								   "	}\n"
								   "\n"
								   "	PASS P2\n"
								   "	{\n"
								   "		StencilPass			= Incr;\n"
								   "		CullMode			= CW;\n"
								   "	}\n"
								   "}\n"
								   "\n"
								   "TECHNIQUE T4\n"
								   "{\n"
								   "	PASS P1\n"
								   "	{\n"
								   "		ZEnable				= True;\n"
								   "		ZWriteEnable		= False;\n"
								   "		ShadeMode			= Flat;\n"
	 							   "        FogEnable			= False;\n"
								   "		ColorOp[0]			= SelectArg1;\n"
								   "		ColorArg1[0]		= Current;\n"
								   "		ColorOp[1]			= Disable;\n"
								   "		Lighting			= False;\n"
								   "		StencilEnable		= True;\n"
								   "		StencilFunc			= Always;\n"
								   "		StencilFail			= Keep;\n"
								   "		StencilZFail		= Keep;\n"
								   "		StencilPass			= Decr;\n"
								   "        StencilMask			= 0xFFFFFFFF;\n"
								   "        StencilWriteMask	= 0xFFFFFFFF;\n"
								   "		CullMode			= CCW;\n"
								   "		AlphaBlendEnable	= True;\n"
								   "		SrcBlend			= Zero;\n"
								   "		DestBlend			= One;\n"
								   "	}\n"
								   "\n"
								   "	PASS P2\n"
								   "	{\n"
								   "		StencilPass			= Incr;\n"
								   "		CullMode			= CW;\n"
								   "	}\n"
								   "}\n", -1))
	{
		// Fehler!
		return FM_ERROR;
	}

	// Effekt für den Schatten erstellen
	m_pShadowEffect = new fmEffect;
	if(m_pShadowEffect->Init("TECHNIQUE T1\n"
							 "{\n"
							 "	  PASS P1\n"
							 "	  {\n"
							 "		  ZEnable			= False;\n"
							 "		  ZWriteEnable		= False;\n"
							 "		  StencilEnable		= True;\n"
							 "		  StencilFunc		= NotEqual;\n"
							 "		  StencilRef		= 0;\n"
						     "		  StencilFail		= Keep;\n"
						     "		  StencilZFail		= Keep;\n"
						     "		  StencilPass		= Keep;\n"
							 "        StencilMask		= 0xFFFFFFFF;\n"
							 "		  ShadeMode			= Flat;\n"
 							 "        FogEnable			= False;\n"
							 "		  ColorOp[0]		= SelectArg1;\n"
							 "		  ColorArg1[0]		= Current;\n"
							 "		  ColorOp[1]		= Disable;\n"
							 "        AlphaOp[0]		= SelectArg1;\n"
							 "		  AlphaArg1[0]		= Current;\n"
							 "		  Lighting			= False;\n"
							 "		  ColorVertex		= True;\n"
							 "		  AlphaBlendEnable	= True;\n"
							 "		  SrcBlend			= SrcAlpha;\n"
							 "		  DestBlend			= InvSrcAlpha;\n"
							 "	  }\n"
						     "}\n", -1))
	{
		// Fehler!
		return FM_ERROR;
	}

	// Speicher für die Dreiecksseiten reservieren
	m_pEdges = (fmEdge*)(malloc(m_pModel->m_dwNumIndices * sizeof(fmEdge)));
	if(!m_pEdges) return FM_ERROR;

	// Speicher für die Mittelpunkte der Dreiecke reservieren
	m_pvTriangleCenters = (fmVector3*)(malloc(m_pModel->m_dwNumIndices / 3 * sizeof(fmVector3)));
	if(!m_pvTriangleCenters) return FM_ERROR;

	// Speicher für die Vertizes des Schattenvolumens reservieren
	m_pvVertices = (fmVector3*)(malloc(m_pModel->m_dwNumIndices * 6 * sizeof(fmVector3)));
	if(!m_pvVertices)  return FM_ERROR;

	// ------------------------------------------------------------------

	// Die Mittelpunkte aller Dreiecke berechnen
	for(DWORD t = 0; t < m_pModel->m_dwNumIndices / 3; t++)
	{
		// Mittelwert aus den drei Vektoren dieses Dreiecks berechnen
		m_pvTriangleCenters[t] = (m_pModel->m_pvVectors[m_pModel->m_pdwIndices[t * 3]] +
			                      m_pModel->m_pvVectors[m_pModel->m_pdwIndices[t * 3 + 1]] +
								  m_pModel->m_pvVectors[m_pModel->m_pdwIndices[t * 3 + 2]]) / 3.0f;
	}

	return FM_OK;
}

fmResult fmShadowVolume::Exit()
{

	// Effekte löschen
	FM_SAFE_DELETE(m_pShadowVolumeEffect);
	FM_SAFE_DELETE(m_pShadowEffect);

	// Speicher freigeben
	FM_SAFE_MEMFREE(m_pEdges);
	FM_SAFE_MEMFREE(m_pvTriangleCenters);
	FM_SAFE_MEMFREE(m_pvVertices);

	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmShadowVolume));

	return FM_OK;
}

void fmShadowVolume::AddEdge(const DWORD dwPointA, const DWORD dwPointB)
{
	// Gibt es vielleicht schon eine Seite mit diesen beiden Punkten?
	for(DWORD dwEdge = 0; dwEdge < m_dwNumEdges; dwEdge++)
	{
		if((m_pEdges[dwEdge].dwPointA == dwPointA && m_pEdges[dwEdge].dwPointB == dwPointB) ||
		   (m_pEdges[dwEdge].dwPointA == dwPointB && m_pEdges[dwEdge].dwPointB == dwPointA))
		{
			// Ja! Dann muss diese Seite entfernt werden.
			if(m_dwNumEdges > 1) m_pEdges[dwEdge] = m_pEdges[m_dwNumEdges - 1];
			m_dwNumEdges--;
			return;
		}
	}

	// Diese Seite existiert noch nicht!
	// Wir fertigen einen neuen Listeneintrag an.
	m_pEdges[m_dwNumEdges].dwPointA = dwPointA;
	m_pEdges[m_dwNumEdges].dwPointB = dwPointB;
	m_dwNumEdges++;
}

fmResult fmShadowVolume::ComputeVolume(const fmMatrix& mInvModelMatrix,
									   const D3DLIGHT9& Light,
									   const float fLength,		// = 1000.0f
									   const BOOL bNormalize)	// = FALSE
{
	fmVector3 vPointA;
	fmVector3 vPointB;
	fmVector3 vPointC;
	fmVector3 vPointD;


	if(Light.Type == D3DLIGHT_DIRECTIONAL)
	{
		// Ein Richtungslicht kann gesondert behandelt werden.
		// Erst transformieren wir die Richtung des Lichts mit der
		// inversen Transformationsmatrix des Modells.
		fmVector3 vDir(fmVector3TransformNormal(fmVector3NormalizeEx(Light.Direction), mInvModelMatrix));

		// Alle Dreiecke durchgehen und das Punktprodukt aus dem
		// DreiecksNormalenvektor und vDir berechnen. Nur wenn ein Wert größer
		// gleich null herauskommt, ist das Dreieck aus der Sicht des
		// Lichts sichtbar (Culling!).
		m_dwNumEdges = 0;
		for(DWORD t = 0; t < m_pModel->m_dwNumIndices / 3; t++)
		{
			// Punktprodukt berechnen und daraus die Sichtbarkeit bestimmen
			if(fmVector3Dot(m_pModel->m_pTrianglePlanes[t * 4].n, vDir) >= 0.0f)
			{
				// Die drei Seiten dieses Dreiecks hinzufügen
				AddEdge(m_pModel->m_pdwIndices[t * 3], m_pModel->m_pdwIndices[t * 3 + 1]);
				AddEdge(m_pModel->m_pdwIndices[t * 3 + 1], m_pModel->m_pdwIndices[t * 3 + 2]);
				AddEdge(m_pModel->m_pdwIndices[t * 3 + 2], m_pModel->m_pdwIndices[t * 3]);
			}
		}

		vDir *= fLength;
		fmVector3* pVertex = m_pvVertices;

		// Die verbliebenen Seiten durchgehen
		for(fmEdge* pEdge = m_pEdges; pEdge < m_pEdges + m_dwNumEdges; pEdge++)
		{
			// Jetzt fügen wir 6 Vertizes zur Vertexliste hinzu,
			// um die Seite zu verlängern.
			vPointA = m_pModel->m_pvVectors[pEdge->dwPointA];
			vPointB = m_pModel->m_pvVectors[pEdge->dwPointB];
			vPointC = vPointB + vDir;
			vPointD = vPointA + vDir;
			*(pVertex++) = vPointA;
			*(pVertex++) = vPointD;
			*(pVertex++) = vPointC;
			*(pVertex++) = vPointA;
			*(pVertex++) = vPointC;
			*(pVertex++) = vPointB;
		}
	}
	else
	{
		// Bei den anderen Lichttypen hat das Licht eine Position.
		// Das heißt, dass der Richtungsvektor vom Licht zu den Dreiecken
		// bei jedem Dreieck unterschiedlich ist.

		// Die Lichtposition in das Modellkoordinatensystem umrechnen
		fmVector3 vLight(fmVector3TransformCoords(Light.Position, mInvModelMatrix));

		// Alle Dreiecke durchgehen. Wir verfahren fast wie oben, nur dass
		// vDir bei jedem Dreieck neu berechnet wird.
		m_dwNumEdges = 0;
		for(DWORD t = 0; t < m_pModel->m_dwNumIndices / 3; t++)
		{
			// Punktprodukt berechnen und daraus die Sichtbarkeit bestimmen
			if(fmVector3Dot(m_pModel->m_pTrianglePlanes[t * 4].n,
							m_pvTriangleCenters[t] - vLight) >= 0.0f)
			{
				// Die drei Seiten dieses Dreiecks hinzufügen
				AddEdge(m_pModel->m_pdwIndices[t * 3], m_pModel->m_pdwIndices[t * 3 + 1]);
				AddEdge(m_pModel->m_pdwIndices[t * 3 + 1], m_pModel->m_pdwIndices[t * 3 + 2]);
				AddEdge(m_pModel->m_pdwIndices[t * 3 + 2], m_pModel->m_pdwIndices[t * 3]);
			}
		}

		// Die verbliebenen Seiten durchgehen
		if(bNormalize)
		{
			fmVector3* pVertex = m_pvVertices;

			for(fmEdge* pEdge = m_pEdges; pEdge < m_pEdges + m_dwNumEdges; pEdge++)
			{
				// Jetzt fügen wir 6 Vertizes zur Vertexliste hinzu,
				// um diese Seite zu verlängern.
				vPointA = m_pModel->m_pvVectors[pEdge->dwPointA];
				vPointB = m_pModel->m_pvVectors[pEdge->dwPointB];
				vPointC = vPointB + fmVector3NormalizeEx(vPointB - vLight) * fLength;
				vPointD = vPointA + fmVector3NormalizeEx(vPointA - vLight) * fLength;
				*(pVertex++) = vPointA;
				*(pVertex++) = vPointD;
				*(pVertex++) = vPointC;
				*(pVertex++) = vPointA;
				*(pVertex++) = vPointC;
				*(pVertex++) = vPointB;
			}

			// Anzahl der Vertizes eintragen
			m_dwNumVertices = m_dwNumEdges * 6;
		}
		else
		{
			fmEdge* pEdge = m_pEdges;
			fmVector3* pVertex = m_pvVertices;

			for(DWORD dwEdge = 0; dwEdge < m_dwNumEdges; dwEdge++)
			{
				// Jetzt fügen wir 6 Vertizes zur Vertexliste hinzu,
				// um diese Seite zu verlängern.
				vPointA = m_pModel->m_pvVectors[pEdge->dwPointA];
				vPointB = m_pModel->m_pvVectors[pEdge->dwPointB];
				vPointC = vPointB + (vPointB - vLight) * fLength;
				vPointD = vPointA + (vPointA - vLight) * fLength;
				*(pVertex++) = vPointA;
				*(pVertex++) = vPointD;
				*(pVertex++) = vPointC;
				*(pVertex++) = vPointA;
				*(pVertex++) = vPointC;
				*(pVertex++) = vPointB;

				pEdge++;
			}

			// Anzahl der Vertizes eintragen
			m_dwNumVertices = m_dwNumEdges * 6;
		}
	}

	return FM_OK;
}

// ******************************************************************
// Rendern des Schattenvolumens
fmResult fmShadowVolume::RenderVolume(const fmMatrix& mModelMatrix)
{
	if(m_dwNumVertices == 0) return FM_OK;

	fmDirect3D& D3D = fmDirect3D::Instance();

	// Vertexformat setzen
	D3D->SetFVF(D3DFVF_XYZ);

	// Transformationsmatrix setzen
	D3D->SetTransform(D3DTS_WORLD, (D3DMATRIX*)(&mModelMatrix));

	// Schattenvolumen rendern
	int iNumPasses = m_pShadowVolumeEffect->Begin();

	for(int iPass = 0; iPass < iNumPasses; iPass++)
	{
		m_pShadowVolumeEffect->Pass(iPass);

		// Primitiven rendern
		D3D->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		                     m_dwNumVertices / 3,
						     m_pvVertices,
							 sizeof(fmVector3));
	}

	// Effekt beenden
	m_pShadowVolumeEffect->End();

	return FM_OK;
}

// ******************************************************************
// Rendern des Schattens
fmResult fmShadowVolume::RenderShadow(const fmColor& ShadowColor) // = tbColor(0.0f, 0.0f, 0.0f, 0.5f)
{
	fmShadowVertex aVertex[4];
	fmDirect3D& D3D = fmDirect3D::Instance();

	

	// Die vier Vertizes erstellen (Rechteck)
	// Links unten
	aVertex[0].vPosition = fmVector3(0.0f, fmDirect3D::Instance().GetScreenSize().y, 0.5f);
	aVertex[0].fRHW = 1.0f;
	aVertex[0].Color = ShadowColor;

	// Links oben
	aVertex[1].vPosition = fmVector3(0.0f, 0.0f, 0.5f);
	aVertex[1].fRHW = 1.0f;
	aVertex[1].Color = ShadowColor;

	// Rechts unten
	aVertex[2].vPosition = fmVector3(fmDirect3D::Instance().GetScreenSize().x, fmDirect3D::Instance().GetScreenSize().y, 0.5f);
	aVertex[2].fRHW = 1.0f;
	aVertex[2].Color = ShadowColor;

	// Rechts oben
	aVertex[3].vPosition = fmVector3(fmDirect3D::Instance().GetScreenSize().x, 0.0f, 0.5f);
	aVertex[3].fRHW = 1.0f;
	aVertex[3].Color = ShadowColor;

	// Vertexformat setzen
	D3D->SetFVF(fmShadowVertex::dwFVF);

	// Als Dreiecksfolge zeichnen
	int iNumPasses = m_pShadowEffect->Begin();
	for(int iPass = 0; iPass < iNumPasses; iPass++)
	{
		m_pShadowEffect->Pass(iPass);
		D3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, aVertex, sizeof(fmShadowVertex));
	}

	m_pShadowEffect->End();

	return FM_OK;
}