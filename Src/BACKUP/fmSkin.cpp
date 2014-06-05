/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmSkin.cpp
===============================
*/

#include <FatalisMatrix.h>

// ******************************************************************
// Konstruktor der Klasse fmSkin
fmSkin::fmSkin()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmSkin));

	m_dwNumJoints = NULL;
	m_dwNumBones  = NULL;

	m_bNewSkin = FALSE;
}

// ******************************************************************
// Init-Funktion der Klasse fmSkin
fmResult fmSkin::Init()
{
	// Die Klasseninstanz zurücksetzen.
	// Damit wird ermöglicht, dass der Init-Aufruf mehrere Male mit
	// derselben Instanz funktioniert. 
	Exit();


	return FM_OK;
}

// ******************************************************************
// Render-Funktion der Klasse fmSkin
fmResult fmSkin::Render(float fTime)
{
	

	return FM_OK;
}

// ******************************************************************
// Mit diesr Funktion wird ein neuer Skin Initiert
SKRESULT fmSkin::FMNewSkin(DWORD dwNumJoints, DWORD dwNumBones, BOOL bNewSkin)
{
	// Instancevariablen zurücksetzten
	Exit();

	m_bNewSkin = bNewSkin;
	
	m_dwNumJoints = dwNumJoints;
	m_dwNumBones  = dwNumBones;

	m_pJoint = (LPJOINT)(malloc(m_dwNumJoints * sizeof(JOINT_S)));
	if(m_pJoint == NULL)
	{
		FM_ERROR_A("You haven't enough space!");
		return SK_NOT_FOUND;
	}

	m_pBone = (LPBONE)(malloc(m_dwNumBones * sizeof(BONE_S)));
	if(m_pBone == NULL)
	{
		FM_ERROR_A("You haven't enough space!");
		return SK_NOT_FOUND;
	}

	return SK_OK;
}

// ******************************************************************
// Mit dieser Funktion ist es möglich Joints zu setzten
SKRESULT fmSkin::FMSetJoint(DWORD dwIDJoint,
							char* pcJointname,
							unsigned int uiID,
							fmVector3 vPosition,
							fmVector3 vRotation)
{
	// Überbrüfungen
	if(dwIDJoint >= m_dwNumJoints)
	{
		FM_ERROR_A("This Joint have no Space");
		return SK_NOT_FOUND;
	}

	// Joint Informationen Kopieren
	strcpy(m_pJoint[dwIDJoint].cName, pcJointname); // Namen übergeben
	m_pJoint[dwIDJoint].uiID = uiID;				 // ID übergeben
	m_pJoint[dwIDJoint].vPosition = vPosition;		 // Position übergeben
	m_pJoint[dwIDJoint].vRotation = vRotation;		 // Rotation übergeben

	return SK_OK;
}

// ******************************************************************
// Mit dieser Funktion ist es möglich Bones zu setzten
SKRESULT fmSkin::FMSetBone(DWORD dwIDBone,
						   char* pcBonename,
						   char* pcJointParentname,
						   char* pcJointChildrenname)
{
	// Überbrüfungen
	if(dwIDBone >= m_dwNumBones)
	{
		FM_ERROR_A("This Bone have no Space");
		return SK_NOT_FOUND;
	}

	strcpy(m_pBone[dwIDBone].cName, pcBonename);

	for(DWORD i = 0; i < m_dwNumJoints; i++)
	{
		if((strcmp(m_pJoint[i].cName, pcJointParentname)) == 0)
		{
			m_pBone[dwIDBone].pJointParent = &m_pJoint[i];
		}
		if((strcmp(m_pJoint[i].cName, pcJointChildrenname)) == 0)
		{
			m_pBone[dwIDBone].pJointChildren = &m_pJoint[i];
		}
	}

	return SK_OK;
}

// ******************************************************************
// Diese Funktion läd die Hierarchie aus einer Datei
SKRESULT fmSkin::FMLoadHierarchyFromFile(char* pcFilename)
{
	// Variablen
	FILE* fIn;

	// Öffnen der Datei
	if((fIn = fopen(pcFilename, "rb")) == NULL)
	{
		FM_ERROR_A("The Hierarchy couldn't be found!");
		return SK_NOT_FOUND;
	}
		// Head von der Datei Laden
		m_pFilehead = (LPFILEHEAD)(malloc(sizeof(FILEHEAD_S)));
		if(m_pFilehead == NULL)
		{
			FM_ERROR_A("You haven't enough space!");
			return SK_NOT_FOUND;
		}
		fread(&m_pFilehead, sizeof(_FILEHEAD), 1, fIn);

		// Header kopieren
		m_dwNumJoints = m_pFilehead->dwNumJoints;	// Anzahl der Joints
		m_dwNumBones  = m_pFilehead->dwNumBones;	// Anzahl der Bones

		// Speicher für Joints allocieren
		m_pJoint = (LPJOINT)(malloc(m_dwNumJoints * sizeof(JOINT_S)));
		if(m_pJoint == NULL)
		{
			FM_ERROR_A("You haven't enough space!");
			return SK_NOT_FOUND;
		}
		m_pBone  = (LPBONE)(malloc(m_dwNumBones * sizeof(BONE_S)));
		if(m_pBone == NULL)
		{
			FM_ERROR_A("You haven't enough space!");
			return SK_NOT_FOUND;
		}

		// Die einzelnen Joints laden
		for(DWORD i = 0; i < m_dwNumJoints; i++)
		{
			fread(&m_pJoint[i], sizeof(JOINT_S), 1, fIn);
		}

		// Die einzelnen Bones laden
		for(i = 0; i < m_dwNumBones; i++)
		{
			fread(&m_pBone[i], sizeof(BONE_S), 1, fIn);
		}

		FM_SAFE_MEMFREE(m_pFilehead);

	fclose(fIn);

	return SK_OK;
}

// ******************************************************************
// Diese Funktion ermöglicht das Abspeichern der Hierarchy
SKRESULT fmSkin::FMSaveHierarchyToFile(char* pcFilename)
{
	FILE* fOut;

	if((fOut = fopen(pcFilename, "wb")) == NULL)
	{
		FM_ERROR_A("The Hierarchy can't be saved!");
		return SK_NOT_FOUND;
	}

		m_pFilehead = (LPFILEHEAD)(malloc(sizeof(FILEHEAD_S)));
		if(m_pFilehead == NULL)
		{
			FM_ERROR_A("You haven't enough space!");
			return SK_ERROR;
		}
	
		// Filehead mit Informationen füllen
		m_pFilehead->dwNumJoints = m_dwNumJoints;
		m_pFilehead->dwNumBones  = m_dwNumBones;

		fwrite(m_pFilehead, sizeof(_FILEHEAD), 1, fOut);

		// Alle Joints abspeichern
		fwrite(m_pJoint, sizeof(_JOINT), m_dwNumJoints, fOut);

		// Alle Bones abspeichern
		fwrite(m_pBone, sizeof(_BONE), m_dwNumBones, fOut);

		FM_SAFE_MEMFREE(m_pFilehead);

	fclose(fOut);

	return SK_OK;
}

// ******************************************************************
// Exit-Funktion der Klasse fmSkin
fmResult fmSkin::Exit()
{
	m_dwNumJoints = NULL;
	m_dwNumBones  = NULL;

	FM_SAFE_MEMFREE(m_pJoint);
	FM_SAFE_MEMFREE(m_pBone);
	FM_SAFE_MEMFREE(m_pVertex);

	// Alles zurücksetzen
	ZeroMemory(this, sizeof(fmSkin));

	return FM_OK;
}
// ******************************************************************