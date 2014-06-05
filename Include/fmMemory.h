/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmMemory.h
===============================
*/

#pragma once

// Funktionen deklarieren
FATALISMATRIX_API fmResult	fmMemInit();								// Initialisierung des Speichermanagers
FATALISMATRIX_API fmResult	fmMemExit();								// Herunterfahren des Speichermanagers
FATALISMATRIX_API void*		fmMemAlloc(int iSize);						// Speicher reservieren
FATALISMATRIX_API void*		fmMemReAlloc(void* pMemory, int iNewSize);	// Speicherbereich vergrößern/verkleinern
FATALISMATRIX_API fmResult	fmMemFree(void* pMemory);					// Speicherbereich freigeben
FATALISMATRIX_API BOOL		fmMemIsValid(void* pMemory);				// Gültigkeit eines Speicherbereichs prüfen
FATALISMATRIX_API int		fmMemGetSize(void* pMemory);				// Größe eines Speicherbereichs abfragen
FATALISMATRIX_API fmResult	fmMemFreeAll();								// Alle Speicherbereiche freigeben