/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmDirect3D.cpp
===============================
*/
#include <FatalisMatrix.h>

// ******************************************************************
// Callback der anwendung
LRESULT WINAPI WindowProc(HWND hWindow,
						  unsigned int uiMessage,
						  WPARAM WParam,
						  LPARAM LParam)
{
	switch(uiMessage)
	{
	case WM_CREATE:
		break;

	case WM_ACTIVATE:
		// Aktivierungsstatus der Anwendung setzen
		fm_g_bAppActive = LOWORD(WParam) != WA_INACTIVE;
		break;

	case WM_CLOSE:
		DestroyWindow(hWindow);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWindow, uiMessage, WParam, LParam);
	}

	return TRUE;
}

// ******************************************************************
// Zugriffsmethode für Singleton
fmDirect3D& fmDirect3D::Instance()
{
	static fmDirect3D TheOneAndOnly;
	return TheOneAndOnly;
}

// ******************************************************************
// Initialisierungsmethode
fmResult fmDirect3D::Init(fmConfig* pConfig, char* pcWindowTitle)
{
	HRESULT					hResult;
	D3DPRESENT_PARAMETERS	PresentParams;

	WNDCLASSEX wndClass =
    {
        sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0, 0,
		fm_g_hInstance,
        LoadIcon(NULL, IDI_WINLOGO),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH),
        NULL,
		"FatalisMatrix Window Class",
		LoadIcon(NULL, IDI_WINLOGO)
    };

	if(!pConfig) FM_ERROR_B("pConfig", FM_ERROR);

	FM_INFO("Die Direct3D-Komponente wird initialisiert...");

	// Wenn kein Fenster Vorhanden ist wird ein neues erstellt
	if(!fm_g_hWindow)
	{
		if(!RegisterClassEx(&wndClass))
		{
			FM_ERROR_B("Fensterklasse konnte nicht registriert werden!", FM_ERROR);
		}

		int iWidth  = pConfig->Direct3D.VideoMode.Width;
		int iHeight = pConfig->Direct3D.VideoMode.Height;

		m_hWindow = CreateWindowEx(NULL, 
                       "FatalisMatrix Window Class", 
                       pcWindowTitle,
                       WS_OVERLAPPEDWINDOW |
                       WS_VISIBLE, 
                       0, 0,        
                       iWidth, iHeight,
                       NULL,  
                       NULL,  
                       fm_g_hInstance,   
                       NULL);

		if(!m_hWindow)
		{
			FM_ERROR_B("Fenster konnte nicht erstellt werden!", FM_ERROR);
		}

		fm_g_hWindow = m_hWindow;
	}
	else
	{
		m_hWindow = fm_g_hWindow;
	}

	FM_INFO("Initialisierung von DirectX");



	// Schnittstelle erzeugen
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_pD3D)
	{
		FM_ERROR_B("Fehler bei der Initialisierung der Schnittstelle", FM_ERROR);
	}

	// Präsentationsstruktur ausfüllen
	ZeroMemory(&PresentParams, sizeof(D3DPRESENT_PARAMETERS));
	PresentParams.BackBufferWidth				= pConfig->Direct3D.VideoMode.Width;
	PresentParams.BackBufferHeight				= pConfig->Direct3D.VideoMode.Height;
	PresentParams.BackBufferFormat				= pConfig->Direct3D.BackBufferFormat;
	PresentParams.BackBufferCount				= 1;
	PresentParams.MultiSampleType				= pConfig->Direct3D.MultiSamplingType;
	PresentParams.MultiSampleQuality			= pConfig->Direct3D.dwMultiSamplingQuality;
	PresentParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	PresentParams.hDeviceWindow					= m_hWindow;
	PresentParams.Windowed						= pConfig->Direct3D.bWindowed;
	PresentParams.EnableAutoDepthStencil		= TRUE;
	PresentParams.AutoDepthStencilFormat		= pConfig->Direct3D.ZStencilBufferFormat;
	PresentParams.Flags							= pConfig->Direct3D.ZStencilBufferFormat != D3DFMT_D16_LOCKABLE ? D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL : 0;
	PresentParams.FullScreen_RefreshRateInHz	= pConfig->Direct3D.bWindowed ? D3DPRESENT_RATE_DEFAULT : pConfig->Direct3D.VideoMode.RefreshRate;
	PresentParams.PresentationInterval			= pConfig->Direct3D.bVSync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

	// Und nun die Geräteschnittstelle generieren
	if(FAILED(hResult = m_pD3D->CreateDevice(pConfig->Direct3D.iAdapter,
								             pConfig->Direct3D.DeviceType,
											 m_hWindow,
											 pConfig->Direct3D.dwFlags,
											 &PresentParams,
											 &m_pD3DDevice)))
	{
		// Fehler beim Generieren der Schnittstelle!
		FM_ERROR_B("Fehler beim Generieren der Schnittstelle", FM_ERROR);
	}

	// Parameter eintragen
	m_PresentParams = PresentParams;

	// Die Gerätefähigkeiten abfragen und den aktuellen Status speichern
	m_pD3DDevice->GetDeviceCaps(&m_Caps);

	// Cursor im Vollbildmodus ausblenden
	if(!pConfig->Direct3D.bWindowed) ShowCursor(FALSE);

	// Standart einstellungen:
	m_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	// Dithering verwenden - verbessert die grafische Qualität der
	// Darstellung:
	m_pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);

	// Anisptropischer Filter
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, m_Caps.MaxAnisotropy);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

	// Bildschirmauflösung übergeben
	m_vScreenSize.x = (float)(PresentParams.BackBufferWidth);
	m_vScreenSize.y = (float)(PresentParams.BackBufferHeight);

	m_PresentResult = FM_OK;
	m_bInitialized = TRUE;

	return FM_OK;
}

fmResult fmDirect3D::Exit()
{
	m_bInitialized = FALSE;

	// Schnittstellen freigeben
	FM_SAFE_RELEASE(m_pD3D);
	FM_SAFE_RELEASE(m_pD3DDevice);

	// Windows Klasse unregist.
	UnregisterClass("FatalisMatrix Window Class", fm_g_hInstance);

	FM_INFO("Direct3D wurde Heruntergefahren!");

	return FM_OK;
}

fmResult fmDirect3D::Present()
{
	HRESULT hResult = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hResult))
	{
		m_PresentResult = FM_ERROR;
		FM_ERROR_B("Fehler beim Blitten des Bildbuffers!", FM_ERROR);
	}
	else
	{
		m_PresentResult = FM_OK;
	}
		
	return FM_OK;
}