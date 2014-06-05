/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmEffect.h
===============================
*/

#pragma once

// Globale Variablen
extern LPD3DXEFFECTPOOL fm_g_pEffectPool;


class FATALISMATRIX_API fmEffect
{
private:
	// Variablen
	LPD3DXEFFECT	m_pEffect;			// Die Effektschnittstelle
	D3DXEFFECT_DESC	m_Desc;				// Effektbeschreibung
	BOOL			m_bStateSaved;		// Wurde der Status gespeichert?
	BOOL			m_bCaptureOnFinish;	// Capture nach dem Effekt aufrufen?
	int				m_iLastPass;
public:
	// Konstruktor und Destruktor
	fmEffect();
	~fmEffect() {Exit();}

	// Methoden
	fmResult	Init(char* pcCode, int iSize);	// Aus String erstellen
	fmResult	Init(char* pcFilename);			// Aus Datei erstellen
	fmResult	Exit();
	fmResult	SetTechnique(int iTechnique = -1);									// Wählt eine Technik
	int			Begin(BOOL bSaveState = TRUE, BOOL bCaptureOnFinish = TRUE);		// Effekt starten
	fmResult	End();																// Effekt beenden
	fmResult	Pass(int iPass);													// Durchgang aktivieren

	// Inline-Methoden
	LPD3DXEFFECT	GetEffect()			{return m_pEffect;}
	int				GetNumTechniques()	{return m_Desc.Techniques;}
};
