/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmEffect.cpp
===============================
*/

#include <FatalisMatrix.h>

// Globale Variablen
LPD3DXEFFECTPOOL fm_g_pEffectPool = NULL;	// Globaler Effektpool

// ******************************************************************
// Konstruktor der fmEffect-Klasse
fmEffect::fmEffect()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmEffect));
}

// ******************************************************************
// Erstellen aus einem String
fmResult fmEffect::Init(char* pcCode, int iSize)
{
	HRESULT hResult;

	// Die Klasseninstanz zurücksetzen.
	// Damit wird ermöglicht, dass der Init-Aufruf mehrere Male mit
	// derselben Instanz funktioniert.
	Exit();

	// Parameter prüfen und sicherstellen, dass Direct3D initialisiert wurde
	if(!pcCode)							return FM_ERROR;
	if(iSize == 0 || iSize < -1)		return FM_ERROR;


	// Länge anpassen
	if(iSize == -1) iSize = strlen(pcCode);

	// Jetzt den Effekt erstellen
	if(FAILED(hResult = D3DXCreateEffect(fmDirect3D::Instance().GetDevice(),
										 pcCode, iSize, NULL, NULL, 0,
										 fm_g_pEffectPool, &m_pEffect, NULL)))
	{
		// Fehler!
		return FM_ERROR;
	}

	// Effektbeschreibung abfragen
	m_pEffect->GetDesc(&m_Desc);

	// Die erste gültige Technik setzen
	SetTechnique(-1);

	return FM_OK;
}

// ******************************************************************
// Erstellen aus Datei
fmResult fmEffect::Init(char* pcFilename)
{
	HRESULT hResult;

	// Die Klasseninstanz zurücksetzen.
	// Damit wird ermöglicht, dass der Init-Aufruf mehrere Male mit
	// derselben Instanz funktioniert.
	Exit();
	
	if(!pcFilename)	return FM_ERROR;

	// Jetzt den Effekt erstellen
	if(FAILED(hResult = D3DXCreateEffectFromFile(fmDirect3D::Instance().GetDevice(),
												 pcFilename,
												 NULL,
												 NULL,
												 0,
												 fm_g_pEffectPool,
												 &m_pEffect,
												 NULL)))
	{
		return FM_ERROR;
	}

		// Effektbeschreibung abfragen
		m_pEffect->GetDesc(&m_Desc);

		// Die erste gültige Technik setzen
		SetTechnique(-1);

		return FM_OK;
}

// ******************************************************************
// Herunterfahren
fmResult fmEffect::Exit()
{
	// Effekt löschen
	FM_SAFE_RELEASE(m_pEffect);

	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmEffect));

	return FM_OK;
}

// ******************************************************************
// Setzt eine Technik oder sucht die erste gültige Technik heraus (bei Angabe von -1)
fmResult fmEffect::SetTechnique(int iTechnique) // = -1
{
	// Parameter prüfen
	if(iTechnique < -1 || iTechnique >= (int)(m_Desc.Techniques)) return FM_ERROR;


	// Wenn -1 angegeben wurde, dann müssen wir die erste gültige
	// Technik suchen.
	if(iTechnique == -1)
	{
		// Alle Techniken durchgehen und überprüfen
		for(int iTest = 0; iTest < (int)(m_Desc.Techniques); iTest++)
		{
			// Die Technik abfragen und ValidateTechnique aufrufen, um sie zu prüfen
			D3DXHANDLE hTechnique = m_pEffect->GetTechnique(iTest);
			if(SUCCEEDED(m_pEffect->ValidateTechnique(hTechnique)))
			{
				// Die Technik ist OK! Aktivieren und wir sind fertig.
				m_pEffect->SetTechnique(hTechnique);
				return FM_OK;
			}
		}

		// Keine gültige Technik gefunden! Macht aber nichts, die Techniken
		// funktionieren meistens trotzdem.
		// Wir wählen nun die letzte Technik im Effekt. Diese sollte so gewählt sein,
		// dass sie auf möglichst vielen Grafikkarten funktioniert.
		m_pEffect->SetTechnique(m_pEffect->GetTechnique(m_Desc.Techniques - 1));
	}
	else
	{
		// Die angegebene Technik abfragen und aktivieren
		m_pEffect->SetTechnique(m_pEffect->GetTechnique(iTechnique));
	}

	return FM_OK;
}

// ******************************************************************
// Starten eines Effekts
int fmEffect::Begin(BOOL bSaveState,		// = TRUE
					BOOL bCaptureOnFinish)	// = TRUE
{
	HRESULT			hResult;
	unsigned int	uiNumPasses;


	// Die Begin-Methode aufrufen
	if(FAILED(hResult = m_pEffect->Begin(&uiNumPasses,
		                                 bSaveState ? 0 : D3DXFX_DONOTSAVESTATE)))
	{
		// Fehler!
		return -1;
	}

	// Angaben kopieren
	m_bStateSaved = bSaveState;
	m_bCaptureOnFinish = bCaptureOnFinish;

	m_iLastPass = -1;

	// Anzahl der Durchgänge zurückliefern
	return uiNumPasses;
}

// ******************************************************************
// Beenden eines Effekts
fmResult fmEffect::End()
{
	if(m_iLastPass != -1) m_pEffect->EndPass();

	// Die End-Methode aufrufen
	m_pEffect->End();

	return FM_OK;
}

// ******************************************************************
// Aktivieren eines Durchgangs
fmResult fmEffect::Pass(int iPass)
{
	if(m_iLastPass != -1) m_pEffect->EndPass();

	// Weiterleiten...
	if(SUCCEEDED(m_pEffect->BeginPass(iPass)))
	{
		m_iLastPass = iPass;
		return FM_OK;
	}
	else return FM_ERROR;
}

// ******************************************************************