/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmList.h
===============================
*/

#pragma once

// Template-Struktur f�r einen Eintrag einer verketteten Liste
template <typename Type> struct fmListEntry
{
	fmListEntry<Type>*	pPrevEntry;	// Zeiger auf den vorherigen Eintrag
	fmListEntry<Type>*	pNextEntry;	// Zeiger auf den n�chsten Eintrag
	Type				Data;		// Die eigentlichen Listendaten
};

// ******************************************************************
// Template-Klasse f�r eine verkettete Liste
template <typename Type> class fmList
{
private:
	// Variablen
	fmListEntry<Type>*	m_pFirstEntry;	// Erster Listeneintrag
	fmListEntry<Type>*	m_pLastEntry;	// Letzter Listeneintrag
	int					m_iNumEntries;	// Anzahl der Listeneintr�ge

public:
	// Konstruktur und Destruktor
	fmList();
	~fmList();

	// Methoden
	fmListEntry<Type>*	AddEntry(Type* pData);									// Eintrag hinzuf�gen
	fmResult			FindEntry(Type* pData, fmListEntry<Type>** ppOut);		// Eintrag suchen
	fmResult			DeleteEntry(fmListEntry<Type>* pEntry);					// Eintrag l�schen
	fmResult			Clear();												// Liste leeren
	fmResult			Traverse(fmResult (* pCallback)(fmListEntry<Type>*));	// Liste durchlaufen

	// Inline-Methoden
	Type*				GetEntryData(fmListEntry<Type>* pEntry)	{return &pEntry->Data;}
	fmListEntry<Type>*	GetFirstEntry()							{return m_pFirstEntry;}
	fmListEntry<Type>*	GetLastEntry()							{return m_pLastEntry;}
	int					GetNumEntries()							{return m_iNumEntries;}
};

// ******************************************************************
// Konstruktor der fmList-Klasse
template <typename Type> fmList<Type>::fmList()
{
	// Alles zur�cksetzen
	ZeroMemory(this, sizeof(fmList<Type>));
}

// ******************************************************************
// Destruktor der fmList-Klasse
template <typename Type> fmList<Type>::~fmList()
{
	// Liste leeren
	Clear();
}

// ******************************************************************
// Diese Methode f�gt einen neuen Eintrag zur Liste hinzu.
template <typename Type> fmListEntry<Type>* fmList<Type>::AddEntry(Type* pData)
{
	fmListEntry<Type>* pNewEntry;

	// Parameter pr�fen
	if(!pData) return NULL;


	// Speicher f�r die Listeneintragsstruktur reservieren
	pNewEntry = new fmListEntry<Type>;
	if(!pNewEntry) return NULL;

	// Der neue Eintrag steht an letzter Stelle der Liste.
	// Daher gibt es keinen n�chsten Eintrag und der vorherige Eintrag
	// ist der urspr�ngliche letzte Listeneintrag.
	pNewEntry->pPrevEntry = m_pLastEntry;
	if(m_pLastEntry) m_pLastEntry->pNextEntry = pNewEntry;
	pNewEntry->pNextEntry = NULL;
	m_pLastEntry = pNewEntry;

	// Wenn die Liste noch ganz leer ist, dann ist dieser Eintrag auch
	// gleichzeitig der erste Eintrag.
	if(!m_pFirstEntry) m_pFirstEntry = pNewEntry;

	// Daten kopieren
	memcpy(&pNewEntry->Data, pData, sizeof(Type));

	// Eintragsz�hler erh�hen
	m_iNumEntries++;

	// Zeiger auf die Listeneintragsstruktur zur�ckliefern
	return pNewEntry;
}

// ******************************************************************
// Diese Methode sucht einen Eintrag in der Liste mit den angegebenen Daten.
template <typename Type> fmResult fmList<Type>::FindEntry(Type* pData,
														  fmListEntry<Type>** ppOut)
{
	fmListEntry<Type>* pCurrentEntry;

	// Parameter pr�fen
	if(!pData) FM_ERROR_NULL_POINTER("pData", fm_ERROR);


	// Zuerst den allerersten Listeneintrag testen
	pCurrentEntry = m_pFirstEntry;

	while(pCurrentEntry)
	{
		// Die Daten des aktuellen Eintrags mit den angegebenen Daten
		// vergleichen. Falls sie �bereinstimmen, ist die Suche beendet.
		if(!memcmp(&pCurrentEntry->Data, pData, sizeof(Type)))
		{
			// Falls gew�nscht, wird der als Parameter angegebene Doppelzeiger
			// nun auf die Adresse der Listeneintragsstruktur gesetzt.
			if(ppOut) *ppOut = pCurrentEntry;
			return FM_OK;
		}

		// Fortschreiten
		pCurrentEntry = pCurrentEntry->pNextEntry;
	}

	// Es wurde nichts gefunden!
	return FM_NOT_FOUND;
}

// ******************************************************************
// Diese Methode l�scht einen Eintrag in der Liste.
template <typename Type> fmResult fmList<Type>::DeleteEntry(fmListEntry<Type>* pEntry)
{
	// Parameter pr�fen
	if(!pEntry) return FM_ERROR;


	// Beim L�schen entsteht ein Loch in der Liste, welches nun "gestopft"
	// werden muss. Dabei spielt es eine Rolle, ob der Eintrag an erster
	// oder letzter Stelle oder irgendwo in der Mitte der Liste steht.
	if(pEntry == m_pFirstEntry &&
	   pEntry == m_pLastEntry)
	{
		// Der Eintrag ist der erste und einzige.
		m_pFirstEntry = NULL;
		m_pLastEntry = NULL;
	}
	else if(pEntry == m_pFirstEntry)
	{
		// Der Eintrag steht an erster Stelle.
		// Der neue erste Eintrag ist nun der Folgende des zu l�schenden Eintrags.
		m_pFirstEntry = pEntry->pNextEntry;
		m_pFirstEntry->pPrevEntry = NULL;
	}
	else if(pEntry == m_pLastEntry)
	{
		// Der Eintrag steht an letzter Stelle.
		// Der neue letzte Eintrag ist nun der Vorherige des zu l�schenden Eintrags.
		m_pLastEntry = pEntry->pPrevEntry;
		m_pLastEntry->pNextEntry = NULL;
	}
	else
	{
		// Der Eintrag steht irgendwo in der Mitte.
		// Der vorherige und der folgende Eintrag werden nun verkn�pft.
		pEntry->pPrevEntry->pNextEntry = pEntry->pNextEntry;
		pEntry->pNextEntry->pPrevEntry = pEntry->pPrevEntry;
	}

	// Der Speicher f�r sie wurde beim Erstellen eines neuen Eintrags
	// reserviert und kann nun wieder freigegeben werden.
	FM_SAFE_DELETE(pEntry);

	// Eintragsz�hler verringern
	m_iNumEntries--;

	return FM_OK;
}

// ******************************************************************
// Diese Methode l�scht die gesamte Liste.
template <typename Type> fmResult fmList<Type>::Clear()
{
	// Es wird so lange der erste Listeneintrag gel�scht, bis keiner mehr da ist.
	while(m_pFirstEntry) DeleteEntry(m_pFirstEntry);

	return FM_OK;
}

// ******************************************************************
// Durchlaufen der Liste
template <typename Type> fmResult fmList<Type>::Traverse(fmResult (* pCallback)(fmListEntry<Type>*))
{
	fmListEntry<Type>* pCurrentEntry;

	// Parameter pr�fen
	if(!pCallback) FM_ERROR_NULL_POINTER("pCallback", FM_ERROR);


	// Die ganze Liste durchlaufen
	pCurrentEntry = m_pFirstEntry;

	while(pCurrentEntry)
	{
		// R�ckruffunktion aufrufen
		if(pCallback(pCurrentEntry) == fm_STOP) break;

		// Fortfahren
		pCurrentEntry = pCurrentEntry->pNextEntry;
	}

	return FM_OK;
}

// ******************************************************************