/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmDirect3D.h
===============================
*/
#pragma once

// ******************************************************************
// Die fmDirect3D - Klasse
// Ermöglicht eine einfache und schnelle Handhabung von Direct3D
class FATALISMATRIX_API fmDirect3D
{
private:
	// Variablen
	BOOL					m_bInitialized;		// Ist Direct3D Initialisiert?
	HWND					m_hWindow;			// Fensterhandle
	PDIRECT3D9				m_pD3D;				// IDirect3D9-Schnittstelle
	PDIRECT3DDEVICE9		m_pD3DDevice;		// Geräteschnittstelle IDirect3DDevice9
	D3DCAPS9				m_Caps;				// Fähigkeiten der Hardware
	D3DPRESENT_PARAMETERS	m_PresentParams;	// Präsentationsstruktur für Direct3D
	fmVector2				m_vScreenSize;		// Bildschirmauflösung
	fmResult				m_PresentResult;	// Rückgabewet der Present Methode
	//fmConfig*				m_pStandartConfig;	// Standart Config

	// Konstruktor (private)
	fmDirect3D() {};
public:
	// Destruktor
	~fmDirect3D() {Exit();}
	
	// Methoden
	fmResult	Init(fmConfig* pConfig, char* pcWindowTitle);
	fmResult	Exit();
	fmResult	Present();

	// Überladen des "->" Operators
	PDIRECT3DDEVICE9 operator -> () { return m_pD3DDevice; }


	// Inline Methoden
	HWND							GetWindow()			{ return m_hWindow; }
	PDIRECT3D9					    GetD3D()			{ return m_pD3D; }
	PDIRECT3DDEVICE9			    GetDevice()			{ return m_pD3DDevice; }
	const D3DCAPS9&					GetCaps()			{ return m_Caps;}
	const D3DPRESENT_PARAMETERS&	GetPresentParams()	{ return m_PresentParams; }
	const fmVector2&				GetScreenSize()		{ return m_vScreenSize; }
	float							GetAspect()			{ return m_vScreenSize.x / m_vScreenSize.y; }
	fmResult						GetPresentResult()	{ return m_PresentResult; }

	// Singleton-Daten
	static fmDirect3D&	Instance();
	static BOOL			IsInitialized()	{return Instance().m_bInitialized;}
};

// ******************************************************************