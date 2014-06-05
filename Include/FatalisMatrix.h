/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: FatalisMatrix.h
===============================
*/
#ifndef _FATALISMATRIX_H_
#define _FATALISMATRIX_H_

#pragma once

#ifndef FATALISMATRIX_API
#define FATALISMATRIX_API
#endif

// Include
#include <Windows.h>
#include <Stdio.h>
#include <Stdlib.h>
#include <Math.h>
#include <D3D9.h>
#include <D3DX9.h>
#include <D3DX9Anim.h>
#include <DXErr9.h>


// Standart Defines
#define FM_SAFE_DELETE(x)       {if((x)) {delete (x); (x) = NULL;}}										// Sicheres Löschen
#define FM_SAFE_DELETE_ARRAY(x) {if((x)) {delete[] (x); (x) = NULL;}}									// Sicheres Löschen eines Arrays
#define FM_SAFE_RELEASE(x)      {if((x)) {(x)->Release(); (x) = NULL;}}									// Sicheres Releasen eines COM-Objektes
#define FM_SAFE_MEMFREE(x)		{if((x)) {free((x)); (x) = NULL;}}										// Sicheres Freigeben von Speicher
#define FM_MIN(a, b)            ((a) < (b) ? (a) : (b))													// Minimum
#define FM_MAX(a, b)            ((a) > (b) ? (a) : (b))													// Maximum
#define FM_PI					(3.1415926535897932384626433832795f)									// Pi
#define FM_DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)								// Grad -> Bogenmaß
#define FM_RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)								// Bogenmaß -> Grad

// Log Defines

#define FM_INFO(a)	{FILE* pfInfo; pfInfo = fopen("Fatalis.log", "a"); fputs("\nInformation: ", pfInfo); fputs((a), pfInfo); fclose(pfInfo);}

#define FM_ERROR_A(a)		{MessageBox(NULL, (a), "Fehler!", MB_OK | MB_ICONWARNING); FM_INFO((a));}				// Warnung ohne Return
#define FM_ERROR_B(a, b)	{MessageBox(NULL, (a), "Fehler!", MB_OK | MB_ICONWARNING); return (b); FM_INFO((a));}	// Warnung mit Return
#define FM_MESSAGE(a, b)	{MessageBox(NULL, (a), (b), MB_OK ); FM_INFO((a));}										// MessageBox

// ******************************************************************
// Statuscodes
enum fmResult
{
	FM_OK			= 0, // Erfolg
	FM_ERROR		= 1, // Allgemeiner Fehler
	FM_CANCELED		= 2, // Aktion abgebrochen
	FM_NOT_FOUND	= 3, // Nicht gefunden
	FM_STOP			= 4, // Stop, Vorgang abbrechen
};

// ******************************************************************
// Globale Variablen
extern HINSTANCE	fm_g_hInstance;
extern HWND			fm_g_hWindow;
extern BOOL			fm_g_bAppActive;

// ******************************************************************
// Prototyps

FATALISMATRIX_API fmResult fmInit();
FATALISMATRIX_API fmResult fmExit();

// ******************************************************************
// Include

#include "fmUtils.h"
#include "fmList.h"
//#include "fmMemory.h"
#include "fmVector2.h"
#include "fmVector3.h"
#include "fmMatrix.h"
#include "fmPlane.h"
#include "fmColor.h"
#include "fmFont2.h"
#include "fmConfig.h"
#include "fmDirect3D.h"
#include "fmMesh.h"
#include "fmModel.h"
#include "fmSimpleMesh.h"
#include "fmEffect.h"
#include "fmShadowVolume.h"
#include "fmTerrain.h"
#include "fmWater.h"
#include "fmSkyBox.h"
#include "fmAllocateHierarchy.h"
#include "fmSkin.h"
#include "fmPhysics.h"


#endif

// ******************************************************************
