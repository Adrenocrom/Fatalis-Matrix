/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmSkin.h
===============================
*/

#pragma once

// ******************************************************************
// Statuscodes für das Skinning
enum SKRESULT
{
	SK_OK			= 0,	// Erfolg
	SK_ABORT		= 1,	// Abbruch
	SK_ERROR		= 2,	// Fehler
	SK_NOT_FOUND	= 3,	// Nicht gefunden
};

#define	ID_ROOTJOINT	0
#define ID_JOINT		1
#define ID_BONE			2

// ******************************************************************
// Struktur für ein Joint (Gelenk)
// Ein gelenk wird über ein Ortsvector an eine Position gebunden
// Animation und ausrichtung erfolgt über eine Rotationsmatrix
// ... Auf diese art und weise wird die Hirachie bereits festgelegt,
// denn die Rotation des Folge Joints wird durch den Ortsvektor beeinflusst
// Die AnimationsID beschreibt wiederum die Hirachie 0 ist der Rootjoint
// Alle Childs werden mit höheren IDs identifiziert
typedef struct _JOINT
{
	char		 cName[256];
	unsigned int uiID;
	fmVector3    vPosition;
	fmVector3	 vRotation;
	
} JOINT_S;

typedef JOINT_S*  LPJOINT, PJOINT;

// ******************************************************************
// Struktur für ein Bone (Knochen)
// Ein Bone dient als verbindungselement zwischen den einzelnen Joints
// Er wird für die berechnung der Vertices benötigt.
typedef struct _BONE
{
	char	cName[256];
	LPJOINT pJointParent;
	LPJOINT pJointChildren;
} BONE_S;

typedef BONE_S* LPBONE, PBONE;

// ******************************************************************
// Structur für ein Vertex
typedef struct _VERTEX
{
	fmVector3 vPosition;
	fmVector3 vNormal;

	LPBONE pBone[3];

} VERTEX_S;

typedef VERTEX_S* LPVERTEX, PVERTEX;

// ******************************************************************
// Struktur für Binärdatei (Kopf der Datei)
typedef struct _FILEHEAD
{
	DWORD dwNumJoints;
	DWORD dwNumBones;
} FILEHEAD_S;

typedef FILEHEAD_S* LPFILEHEAD, PFILEHEAD;

// ******************************************************************
// Klasse für Charakteranimation fmSkin
class FATALISMATRIX_API fmSkin
{
private:
	// Variablen
	DWORD m_dwNumJoints;		// Anzahl der Gelenke
	DWORD m_dwNumBones;			// Anzahl der Knochen
	DWORD m_dwNumFaces;			// Anzahl der einzelnen Dreiecke
	DWORD m_dwNumIndices;		// Anzahl der Indices
	DWORD m_dwNumVertices;		// Anzahl der Vertices

	BOOL  m_bNewSkin;			// Neuer Skin

	LPFILEHEAD   m_pFilehead;
	LPJOINT		 m_pJoint;
	LPBONE		 m_pBone;
	LPVERTEX	 m_pVertex;

public:

	// Konstrukter und Destrukter
	fmSkin();
	~fmSkin() {Exit();}

	// Laden von Mesh aus Datei
	fmResult Init();
	fmResult Render(float fTime);
	fmResult Exit();

	// Modelformatfunktionen
	SKRESULT	FMNewSkin(DWORD dwNumJoints, DWORD dwNumBones, BOOL bNewSkin = TRUE);
	SKRESULT	FMSetJoint(DWORD dwIDJoint,
						   char* pcJointname,
						   unsigned int uiID,
						   fmVector3 vPosition,
						   fmVector3 vRotation);
	SKRESULT	FMSetBone(DWORD dwIDBone,
						  char* pcBonename,
						  char* pcJointParentname,
						  char* pcJointChildrenname);
	LPJOINT		FMGetJoint(DWORD dwIDJoint) {return &m_pJoint[dwIDJoint];}
	LPBONE		FMGetBone(DWORD dwIDBone) { return &m_pBone[dwIDBone];}
	SKRESULT	FMLoadHierarchyFromFile(char* pcFilename);
	SKRESULT	FMSaveHierarchyToFile(char* pcFilename);
	// Inline Methoden
	void SetNumJoints(DWORD dwNumJoints) {m_dwNumJoints = dwNumJoints;}
	void SetNumBones(DWORD dwNumBones) {m_dwNumJoints = dwNumBones;}

	BOOL  GetNewSkinInfo() {return m_bNewSkin;}
	DWORD GetNumJoints() {return m_dwNumJoints;}
	DWORD GetNumBones() {return m_dwNumBones;}
	DWORD GetNumFaces() {return m_dwNumFaces;}
	DWORD GetNumIndices() {return m_dwNumIndices;}
	DWORD GetNumVertices() {return m_dwNumVertices;}
};
// ******************************************************************