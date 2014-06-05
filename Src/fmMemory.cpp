/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmMemory.cpp
===============================
*/

#include <FatalisMatrix.h>

// ******************************************************************
// Interne Struktur f�r einen Speicherbereich
struct fmMemListEntry
{
	void*	pMemory;		// Zeiger auf den gesamten Speicherbereich
	void*	pUserMemory;	// Zeiger auf den Benutzerspeicher
	int		iSize;			// Gr��e des Speicherbereichs
	char	acCheck[8];		// Checkcode (= "TriBase")
};

// ******************************************************************
// Interne Variablen
fmList<fmMemListEntry>*	g_pMemoryList = NULL;	// Speicherliste
int						g_iByteCounter = 0;		// Speicherz�hler

// ******************************************************************
// Interne Funktion zum Abfragen der Datenstruktur eines Speicherbereichs
fmResult MemGetEntry(void* pMemory,
					 fmListEntry<fmMemListEntry>** ppOut)
{
	// Parameter pr�fen
	if(!pMemory)	return FM_ERROR;
	if(!ppOut)		return FM_ERROR;


	// Speicherbereich pr�fen
	if(!fmMemIsValid(pMemory)) return FM_ERROR;

	// Vom angegebenen Zeiger muss die Gr��e eines Zeigers auf eine
	// fmMemListEntry-Struktur abgezogen werden.
	pMemory = (BYTE*)(pMemory) - sizeof(fmListEntry<fmMemListEntry>*);

	// pMemory zeigt jetzt auf einen Zeiger auf die zum Speicher
	// geh�rige Listenstruktur - dieser wird geliefert.
	*ppOut = *((fmListEntry<fmMemListEntry>**)(pMemory));

	return FM_OK;
}

// ******************************************************************
// Interne Funktion zum Hinzuf�gen eines Speicherbereichs zur Liste
fmResult MemAddToList(void* pMemory,
					  int iSize)
{
	fmMemListEntry					NewEntry;				// Neuer Listeneintrag
	fmListEntry<fmMemListEntry>*	pNewMemoryListEntry;	// Neuer Speicherlisteneintrag


	// Parameter pr�fen
	if(!pMemory)	return FM_ERROR;
	if(iSize < 0)	return FM_ERROR;


	// Neuen Listeneintrag ausf�llen
	NewEntry.pMemory = pMemory;
	NewEntry.pUserMemory = (BYTE*)(pMemory) + sizeof(fmListEntry<fmMemListEntry>*);
	NewEntry.iSize = iSize;
	strcpy(NewEntry.acCheck, "TriBase");

	// Eintrag zur Speicherliste hinzuf�gen
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
	// Alle Speicherbereiche l�schen
	fmMemFreeAll();

	// Liste l�schen
	FM_SAFE_DELETE(g_pMemoryList);

	return FM_OK;
}

// ******************************************************************
// Funktion zum Reservieren von Speicher
FATALISMATRIX_API void* fmMemAlloc(int iSize)
{
	void* pMemory;

	// Parameter pr�fen
	if(iSize < 0) return NULL;


	// Speicher reservieren und leeren. Beim Reservieren werden ein
	// paar Bytes mehr gebraucht, damit der Zeiger auf den Listeneintrag
	// mitgespeichert werden kann.
	pMemory = ::operator new(iSize + sizeof(fmListEntry<fmMemListEntry>*));
	if(!pMemory) return NULL;

	g_iByteCounter += iSize + sizeof(fmListEntry<fmMemListEntry>*);
	ZeroMemory(pMemory, iSize + sizeof(fmListEntry<fmMemListEntry>*));

	// Zur Liste hinzuf�gen
	if(MemAddToList(pMemory, iSize)) return NULL;

	// Zeiger zur�ckliefern
	return (BYTE*)(pMemory) + sizeof(fmListEntry<fmMemListEntry>*);
}

// ******************************************************************
// Funktion zum Vergr��ern eines Speicherbereichs
FATALISMATRIX_API void* fmMemReAlloc(void* pMemory,
							   int iNewSize)
{
	int		iSize;
	void*	pNewMemory;


	// Gr��e des Speicherbereichs abfragen
	iSize = fmMemGetSize(pMemory);
	if(iSize == -1) return fmMemAlloc(abs(iNewSize));

	// Bei negativer neuer Gr��e wird der Speicherbereich vergr��ert.
	if(iNewSize < 0) iNewSize = iSize - iNewSize;

	// Neuen Speicherbereich reservieren
	pNewMemory = fmMemAlloc(iNewSize);
	if(!pNewMemory) return NULL;

	// Speicher hin�berkopieren und den alten Speicherbereich l�schen
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


	// Parameter pr�fen
	if(!pMemory) return FM_ERROR;


	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		return FM_ERROR;
	}

	// Speicher und Listeneintrag l�schen
	Entry = pMemoryListEntry->Data;
	g_iByteCounter -= Entry.iSize + sizeof(fmListEntry<fmMemListEntry>*);
	::operator delete(Entry.pMemory);
	g_pMemoryList->DeleteEntry(pMemoryListEntry);

	return FM_OK;
}

// ******************************************************************
// Pr�fen der G�ltigkeit eines Speicherbereichs
FATALISMATRIX_API BOOL fmMemIsValid(void* pMemory)
{
	fmListEntry<fmMemListEntry>* pEntry;

	// Nullzeiger gelten als ung�ltig!
	if(!pMemory) return FALSE;


	// Zeiger auf die Struktur des Listeneintrags abfragen. Wenn
	// in der Struktur ein Nullzeiger ist, ist der Bereich ung�ltig.
	pEntry = *((fmListEntry<fmMemListEntry>**)((BYTE*)(pMemory) - sizeof(fmListEntry<fmMemListEntry>*)));
	if(!pEntry->Data.pMemory || !pEntry->Data.pUserMemory) return FALSE;

	// Gr��e und "TriBase"-Signatur pr�fen
	if(pEntry->Data.iSize < 0) return FALSE;
	if(strncmp(pEntry->Data.acCheck, "TriBase", 8)) return FALSE;

	// Der Speicherbereich scheint in Ordnung zu sein.
	return TRUE;
}

// ******************************************************************
// Abfragen der Gr��e eines Speicherbereichs
FATALISMATRIX_API int fmMemGetSize(void* pMemory)
{
	fmListEntry<fmMemListEntry>* pMemoryListEntry;

	// Parameter pr�fen
	if(!pMemory) return -1;

	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		return -1;
	}

	// Gr��e liefern
	return pMemoryListEntry->Data.iSize;
}

// ******************************************************************
// L�schen aller Speicherbereiche
FATALISMATRIX_API fmResult fmMemFreeAll()
{
	fmListEntry<fmMemListEntry>* pEntry;

	// pEntry zeigt zuerst auf den ersten Eintrag.
	pEntry = g_pMemoryList->GetFirstEntry();

	// Die gesamte Liste durchgehen, bis das Ende erreicht ist
	while(pEntry)
	{
		// Aktuellen Speicherbereich l�schen
		fmMemFree(pEntry->Data.pUserMemory);

		// Neuen ersten Eintrag abfragen
		pEntry = g_pMemoryList->GetFirstEntry();
	}

	return FM_OK;
}

// ******************************************************************