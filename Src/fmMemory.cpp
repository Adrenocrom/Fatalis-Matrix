/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmMemory.cpp
===============================
*/

#include <FatalisMatrix.h>

// ******************************************************************
// Interne Struktur für einen Speicherbereich
struct fmMemListEntry
{
	void*	pMemory;		// Zeiger auf den gesamten Speicherbereich
	void*	pUserMemory;	// Zeiger auf den Benutzerspeicher
	int		iSize;			// Größe des Speicherbereichs
	char	acCheck[8];		// Checkcode (= "TriBase")
};

// ******************************************************************
// Interne Variablen
fmList<fmMemListEntry>*	g_pMemoryList = NULL;	// Speicherliste
int						g_iByteCounter = 0;		// Speicherzähler

// ******************************************************************
// Interne Funktion zum Abfragen der Datenstruktur eines Speicherbereichs
fmResult MemGetEntry(void* pMemory,
					 fmListEntry<fmMemListEntry>** ppOut)
{
	// Parameter prüfen
	if(!pMemory)	return FM_ERROR;
	if(!ppOut)		return FM_ERROR;


	// Speicherbereich prüfen
	if(!fmMemIsValid(pMemory)) return FM_ERROR;

	// Vom angegebenen Zeiger muss die Größe eines Zeigers auf eine
	// fmMemListEntry-Struktur abgezogen werden.
	pMemory = (BYTE*)(pMemory) - sizeof(fmListEntry<fmMemListEntry>*);

	// pMemory zeigt jetzt auf einen Zeiger auf die zum Speicher
	// gehörige Listenstruktur - dieser wird geliefert.
	*ppOut = *((fmListEntry<fmMemListEntry>**)(pMemory));

	return FM_OK;
}

// ******************************************************************
// Interne Funktion zum Hinzufügen eines Speicherbereichs zur Liste
fmResult MemAddToList(void* pMemory,
					  int iSize)
{
	fmMemListEntry					NewEntry;				// Neuer Listeneintrag
	fmListEntry<fmMemListEntry>*	pNewMemoryListEntry;	// Neuer Speicherlisteneintrag


	// Parameter prüfen
	if(!pMemory)	return FM_ERROR;
	if(iSize < 0)	return FM_ERROR;


	// Neuen Listeneintrag ausfüllen
	NewEntry.pMemory = pMemory;
	NewEntry.pUserMemory = (BYTE*)(pMemory) + sizeof(fmListEntry<fmMemListEntry>*);
	NewEntry.iSize = iSize;
	strcpy(NewEntry.acCheck, "TriBase");

	// Eintrag zur Speicherliste hinzufügen
	pNewMemoryListEntry = g_pMemoryList->AddEntry(&NewEntry);
	if(!pNewMemoryListEntry) return FM_ERROR;

	// Zeiger auf den Speicherlisteneintrag in den Speicher schreiben
	*((fmListEntry<fmMemListEntry>**)(pMemory)) = pNewMemoryListEntry;

	return FM_OK;
}

// ******************************************************************
// Initialisieren des Speichermanagers
FATALISMATRIX_API fmResult fmMemInit()
{
	// Speicherliste erstellen
	g_pMemoryList = new fmList<fmMemListEntry>;
	if(!g_pMemoryList) return FM_ERROR;
	
	return FM_OK;
}

// ******************************************************************
// Herunterfahren des Speichermanagers
FATALISMATRIX_API fmResult fmMemExit()
{
	// Alle Speicherbereiche löschen
	fmMemFreeAll();

	// Liste löschen
	FM_SAFE_DELETE(g_pMemoryList);

	return FM_OK;
}

// ******************************************************************
// Funktion zum Reservieren von Speicher
FATALISMATRIX_API void* fmMemAlloc(int iSize)
{
	void* pMemory;

	// Parameter prüfen
	if(iSize < 0) return NULL;


	// Speicher reservieren und leeren. Beim Reservieren werden ein
	// paar Bytes mehr gebraucht, damit der Zeiger auf den Listeneintrag
	// mitgespeichert werden kann.
	pMemory = ::operator new(iSize + sizeof(fmListEntry<fmMemListEntry>*));
	if(!pMemory) return NULL;

	g_iByteCounter += iSize + sizeof(fmListEntry<fmMemListEntry>*);
	ZeroMemory(pMemory, iSize + sizeof(fmListEntry<fmMemListEntry>*));

	// Zur Liste hinzufügen
	if(MemAddToList(pMemory, iSize)) return NULL;

	// Zeiger zurückliefern
	return (BYTE*)(pMemory) + sizeof(fmListEntry<fmMemListEntry>*);
}

// ******************************************************************
// Funktion zum Vergrößern eines Speicherbereichs
FATALISMATRIX_API void* fmMemReAlloc(void* pMemory,
							   int iNewSize)
{
	int		iSize;
	void*	pNewMemory;


	// Größe des Speicherbereichs abfragen
	iSize = fmMemGetSize(pMemory);
	if(iSize == -1) return fmMemAlloc(abs(iNewSize));

	// Bei negativer neuer Größe wird der Speicherbereich vergrößert.
	if(iNewSize < 0) iNewSize = iSize - iNewSize;

	// Neuen Speicherbereich reservieren
	pNewMemory = fmMemAlloc(iNewSize);
	if(!pNewMemory) return NULL;

	// Speicher hinüberkopieren und den alten Speicherbereich löschen
	memcpy(pNewMemory, pMemory, FM_MIN(iSize, iNewSize));
	FM_SAFE_MEMFREE(pMemory);

	// Neuen Zeiger liefern
	return pNewMemory;
}

// ******************************************************************
// Freigeben eines Speicherbereichs
FATALISMATRIX_API fmResult fmMemFree(void* pMemory)
{
	fmListEntry<fmMemListEntry>*	pMemoryListEntry;
	fmMemListEntry					Entry;


	// Parameter prüfen
	if(!pMemory) return FM_ERROR;


	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		return FM_ERROR;
	}

	// Speicher und Listeneintrag löschen
	Entry = pMemoryListEntry->Data;
	g_iByteCounter -= Entry.iSize + sizeof(fmListEntry<fmMemListEntry>*);
	::operator delete(Entry.pMemory);
	g_pMemoryList->DeleteEntry(pMemoryListEntry);

	return FM_OK;
}

// ******************************************************************
// Prüfen der Gültigkeit eines Speicherbereichs
FATALISMATRIX_API BOOL fmMemIsValid(void* pMemory)
{
	fmListEntry<fmMemListEntry>* pEntry;

	// Nullzeiger gelten als ungültig!
	if(!pMemory) return FALSE;


	// Zeiger auf die Struktur des Listeneintrags abfragen. Wenn
	// in der Struktur ein Nullzeiger ist, ist der Bereich ungültig.
	pEntry = *((fmListEntry<fmMemListEntry>**)((BYTE*)(pMemory) - sizeof(fmListEntry<fmMemListEntry>*)));
	if(!pEntry->Data.pMemory || !pEntry->Data.pUserMemory) return FALSE;

	// Größe und "TriBase"-Signatur prüfen
	if(pEntry->Data.iSize < 0) return FALSE;
	if(strncmp(pEntry->Data.acCheck, "TriBase", 8)) return FALSE;

	// Der Speicherbereich scheint in Ordnung zu sein.
	return TRUE;
}

// ******************************************************************
// Abfragen der Größe eines Speicherbereichs
FATALISMATRIX_API int fmMemGetSize(void* pMemory)
{
	fmListEntry<fmMemListEntry>* pMemoryListEntry;

	// Parameter prüfen
	if(!pMemory) return -1;

	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		return -1;
	}

	// Größe liefern
	return pMemoryListEntry->Data.iSize;
}

// ******************************************************************
// Löschen aller Speicherbereiche
FATALISMATRIX_API fmResult fmMemFreeAll()
{
	fmListEntry<fmMemListEntry>* pEntry;

	// pEntry zeigt zuerst auf den ersten Eintrag.
	pEntry = g_pMemoryList->GetFirstEntry();

	// Die gesamte Liste durchgehen, bis das Ende erreicht ist
	while(pEntry)
	{
		// Aktuellen Speicherbereich löschen
		fmMemFree(pEntry->Data.pUserMemory);

		// Neuen ersten Eintrag abfragen
		pEntry = g_pMemoryList->GetFirstEntry();
	}

	return FM_OK;
}

// ******************************************************************