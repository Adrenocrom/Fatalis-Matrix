/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: FatalisMatrix.cpp
===============================
*/
#include <FatalisMatrix.h>

// ******************************************************************
// Globale Variablen
HINSTANCE	fm_g_hInstance	= NULL;
HWND		fm_g_hWindow	= NULL;
BOOL		fm_g_bAppActive = TRUE;

// ******************************************************************
// Initieren der Engine
FATALISMATRIX_API fmResult fmInit()
{

    FILE *f;

	f = fopen("Fatalis.log", "w");

		fputs("Fatalis - Logfile:\n", f);
		fputs("----------------------------------------------\n", f);
		fputs("Fatalis Operation1: Initialisiere Engine\n", f);

	fclose(f);

	// Globalen D3DX-Effectpool erstellen
	D3DXCreateEffectPool(&fm_g_pEffectPool);

	return FM_OK;
}

// ******************************************************************
// Herunterfahren der Engine
FATALISMATRIX_API fmResult fmExit()
{

	// Eventuelles entladen des Effektpools
	FM_SAFE_RELEASE(fm_g_pEffectPool);

	if(fmDirect3D::IsInitialized())
	{
		FM_MESSAGE("Direct3D nicht heruntergefahren!\nWird Automatisch erledigt...", "Warning!");
		fmDirect3D::Instance().Exit();
	}

    FILE *f;

	f = fopen("Fatalis.log", "a");

		fputs("\n----------------------------------------------\n", f);
		fputs("Logfile -> End." ,f);

	fclose(f);

	return FM_OK;
}

// ******************************************************************