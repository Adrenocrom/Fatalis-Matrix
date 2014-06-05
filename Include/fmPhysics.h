/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmPhysics.h
===============================
*/

#pragma once

extern fmVector3	g_vGravity;
extern fmVector3    g_vBuoyanceAir;
extern fmVector3	g_vBuoyanceWater;
extern float		g_fSealevel;

extern fmVector3	g_vTemp1;
extern fmVector3	g_vTemp2;
extern fmVector3	g_vTemp3;
extern fmVector3	g_vTemp4;
extern fmVector3	g_vTemp5;
extern fmVector3	g_vTemp6;
extern float		g_fTemp1;
extern float		g_fTemp2;

// **************************************************************************
class fmMassPoint
{
public:
	// Variablen
	fmVector3	m_vWorldPos;
	fmVector3	m_vModelPos;
	fmVector3   m_vAcceleration;
	fmVector3   m_vAccelerationLF;
	fmVector3	m_vVelocity;
	fmVector3	m_vDistance;
	fmVector3	m_vForce;
	float m_fRadius;
	float m_fMass;
	// Konstruktor und Destruktor
	fmMassPoint()
	{
		m_vWorldPos       = fmVector3(0.0f, 0.0f, 0.0f);
		m_vModelPos       = fmVector3(0.0f, 0.0f, 0.0f);
		m_vAcceleration   = fmVector3(0.0f, 0.0f, 0.0f);
		m_vAccelerationLF = fmVector3(0.0f, 0.0f, 0.0f);
		m_vVelocity       = fmVector3(0.0f, 0.0f, 0.0f);
		m_vDistance       = fmVector3(0.0f, 0.0f, 0.0f);
		m_vForce	      = fmVector3(0.0f, 0.0f, 0.0f);
		m_fRadius = 0.0f;
		m_fMass   = 0.0f;
	}
	// Methoden
	void Move(float fTimeStep, float fBuoyanceFactor)
	{
		// Alte Beschleunigung zwischen speichern
		g_vTemp1 = m_vVelocity;

		if(m_fRadius == 0.0f)
		{
			if(m_vWorldPos.y > g_fSealevel)
				m_vAcceleration = (m_vForce / m_fMass) + g_vGravity + (g_vBuoyanceAir * fBuoyanceFactor);
			else
				m_vAcceleration = (m_vForce / m_fMass) + g_vGravity + (g_vBuoyanceWater * fBuoyanceFactor);
		}
		else
		{
			if(m_vWorldPos.y > g_fSealevel)
				m_vAcceleration = (m_vForce / m_fMass) + g_vGravity + (g_vBuoyanceAir * fBuoyanceFactor);
			else
				m_vAcceleration = (m_vForce / m_fMass) + g_vGravity + (g_vBuoyanceWater * fBuoyanceFactor);
		}

		// Neue geschwindigkeit mit der neuen beschleunigung
		g_vTemp2 = m_vVelocity + m_vAcceleration * fTimeStep;

		// Alte geschwindigkeit neu berrechnen
		g_vTemp3 = g_vTemp2 - m_vAccelerationLF * fTimeStep;

		// Geschwindigkeitskorrektur
		g_vTemp4 = g_vTemp1 + g_vTemp3;
		g_vTemp4 *= 0.5f;

		m_vVelocity = g_vTemp4 + m_vAcceleration * fTimeStep;
		
		m_vWorldPos += m_vVelocity * fTimeStep;

		m_vAccelerationLF = m_vAcceleration;

		m_vForce = fmVector3(0.0f, 0.0f, 0.0f);
	}
};

class fmBonding
{
public:
	// Variablen
	DWORD		m_dwNrOfBondedMassPoint1;
	DWORD		m_dwNrOfBondedMassPoint2;

	fmVector3   m_vNormalBondDistance;
	fmVector3   m_vElongationForceDirection;
	fmVector3	m_vForceOnBondedMassPoint1;
	fmVector3   m_vForceOnBondedMassPoint2;

	float		m_fForceConstant;
	float		m_fNormalDistance;
	float		m_fElongation;

	// Konstruktor und Destruktor
	fmBonding()
	{
		m_dwNrOfBondedMassPoint1 = 0;
		m_dwNrOfBondedMassPoint2 = 0;
		m_vNormalBondDistance		= fmVector3(0.0f, 0.0f, 0.0f);
		m_vElongationForceDirection = fmVector3(0.0f, 0.0f, 0.0f);
		m_vForceOnBondedMassPoint1	= fmVector3(0.0f, 0.0f, 0.0f);
		m_vForceOnBondedMassPoint2	= fmVector3(0.0f, 0.0f, 0.0f);
		m_fForceConstant  = 0.0f;
		m_fNormalDistance = 0.0f;
		m_fElongation	  = 0.0f;
	}

	// Inline Methoden
	inline void Update(fmVector3* pvActualBondDistance)
	{
		float fLength = fmVector3Length(*pvActualBondDistance);
		m_fElongation = fLength - m_fNormalDistance;

		m_vElongationForceDirection = *pvActualBondDistance / fLength;

		m_vForceOnBondedMassPoint1 = -m_fElongation * m_fForceConstant * m_vElongationForceDirection;
		m_vForceOnBondedMassPoint2 =  m_fElongation * m_fForceConstant * m_vElongationForceDirection;
	}
};

class fmPhysicObject
{
public:
	// Variablen
	fmMatrix		m_mTransformation;
	fmVector3		m_vCenterOfMassWorld;
	fmVector3		m_vCenterOfMassModel;
	fmVector3		m_vVelocity;
	float			m_fMass;
	float			m_fRadius;
	float			m_fSqrtRadius;
	float			m_fBuoyanceFactor;

	DWORD			m_dwNumMassPoints;
	fmMassPoint*	m_pMassPoint;

	DWORD			m_dwNumBondings;
	fmBonding*		m_pBonding;
	// Konstruktor und Destruktor
	fmPhysicObject()
	{
		m_mTransformation = fmMatrixIdentity();
		m_vCenterOfMassWorld = fmVector3(0.0f, 0.0f, 0.0f);
		m_vCenterOfMassModel = fmVector3(0.0f, 0.0f, 0.0f);
		m_vVelocity			 = fmVector3(0.0f, 0.0f, 0.0f);
		m_fMass   = 0.0f;
		m_fRadius = 0.0f;
		m_fBuoyanceFactor = 0.0f;

		m_dwNumMassPoints = 0;
		m_dwNumBondings	  = 0;
		m_pMassPoint = NULL;
		m_pBonding   = NULL;
	}

	~fmPhysicObject()
	{
		FM_SAFE_DELETE_ARRAY(m_pMassPoint);
		FM_SAFE_DELETE_ARRAY(m_pBonding);
	}
	// Methoden
	void Move(float fTimeStep)
	{
		if(m_dwNumBondings != 0)
		{
			for(unsigned int a = 0; a < m_dwNumBondings; a++)
			{
				g_vTemp1 = m_pMassPoint[m_pBonding[a].m_dwNrOfBondedMassPoint1].m_vWorldPos -
						   m_pMassPoint[m_pBonding[a].m_dwNrOfBondedMassPoint2].m_vWorldPos;

				m_pBonding[a].Update(&g_vTemp1);

				m_pMassPoint[m_pBonding[a].m_dwNrOfBondedMassPoint1].m_vForce += m_pBonding[a].m_vForceOnBondedMassPoint1;
				m_pMassPoint[m_pBonding[a].m_dwNrOfBondedMassPoint2].m_vForce += m_pBonding[a].m_vForceOnBondedMassPoint2;
			}
		}

		for(unsigned int b = 0; b < m_dwNumMassPoints; b++)
		{
			m_pMassPoint[b].Move(fTimeStep, m_fBuoyanceFactor);
		}
	}

	void SetTransformation()
	{
		fmMatrix  mTranslation;
		fmMatrix  mScaling;
		fmMatrix  mRotation;

		mScaling	 = fmMatrixIdentity();
		mTranslation = fmMatrixIdentity();
		mRotation    = fmMatrixIdentity();

		// Translation berrechnen
		for(unsigned int a = 0; a < m_dwNumMassPoints; a++)
		{
			m_vCenterOfMassWorld += m_pMassPoint[a].m_vWorldPos * m_pMassPoint[a].m_fMass;
		}

		m_vCenterOfMassWorld /= m_fMass;

		for(a = 0; a < m_dwNumMassPoints; a++)
		{
			m_pMassPoint[a].m_vDistance = m_pMassPoint[a].m_vWorldPos - m_vCenterOfMassWorld;
		}

		

		// mTranslation = fmMatrixTranslation(vPosition);
		mTranslation.m41 = m_vCenterOfMassWorld.x;
		mTranslation.m42 = m_vCenterOfMassWorld.y;
		mTranslation.m43 = m_vCenterOfMassWorld.z;

		// Scalierung berrechnen
		mScaling.m11 = m_fSqrtRadius;
		mScaling.m22 = m_fSqrtRadius;
		mScaling.m33 = m_fSqrtRadius;

		mRotation = fmMatrixIdentity();
		
		// Rotation berrechnen
		if(m_dwNumMassPoints == 2)
		{
			
			
		}
		else if(m_dwNumMassPoints > 7)
		{
			fmVector3 vNormal1;
			fmVector3 vNormal2;
			fmVector3 vNormal3;

			// Normale in x-Richtung (in der Anfangsorientierung):
			g_vTemp1 = m_pMassPoint[2].m_vWorldPos - m_pMassPoint[6].m_vWorldPos;
			g_vTemp2 = m_pMassPoint[5].m_vWorldPos - m_pMassPoint[6].m_vWorldPos;

			vNormal1 = fmVector3Cross(g_vTemp1, g_vTemp2);
			vNormal1 = fmVector3Normalize(vNormal1);

			// Normale in y-Richtung (in der Anfangsorientierung):
			g_vTemp1 = m_pMassPoint[4].m_vWorldPos - m_pMassPoint[0].m_vWorldPos;
			g_vTemp2 = m_pMassPoint[1].m_vWorldPos - m_pMassPoint[0].m_vWorldPos;

			vNormal2 = fmVector3Cross(g_vTemp1, g_vTemp2);
			vNormal2 = fmVector3Normalize(vNormal2);

			// Normale in z-Richtung (in der Anfangsorientierung):
			g_vTemp1 = m_pMassPoint[7].m_vWorldPos - m_pMassPoint[4].m_vWorldPos;
			g_vTemp2 = m_pMassPoint[5].m_vWorldPos - m_pMassPoint[4].m_vWorldPos;

			vNormal3 = fmVector3Cross(g_vTemp1, g_vTemp2);
			vNormal3 = fmVector3Normalize(vNormal3);

			mRotation.m11 = vNormal1.x;
			mRotation.m21 = vNormal2.x;
			mRotation.m31 = vNormal3.x;
			mRotation.m41 = 0.0f;

			mRotation.m12 = vNormal1.y;
			mRotation.m22 = vNormal2.y;
			mRotation.m32 = vNormal3.y;
			mRotation.m42 = 0.0f;

			mRotation.m13 = vNormal1.z;
			mRotation.m23 = vNormal2.z;
			mRotation.m33 = vNormal3.z;
			mRotation.m43 = 0.0f;

			mRotation.m14 = 0.0f;
			mRotation.m24 = 0.0f;
			mRotation.m34 = 0.0f;
			mRotation.m44 = 1.0f;
		}

		m_mTransformation = mScaling * mRotation * mTranslation;
	}
	// Inline Methoden
};

// **************************************************************************
class fmPhysics
{
public:	
	// Variablen
	char			m_cBuffer[200];

	DWORD			m_dwNumPhysicObjects;
	fmPhysicObject* m_pPhysicObject;
	// Konstruktor und Destruktor
	fmPhysics()
	{
		m_dwNumPhysicObjects = 0;
		m_pPhysicObject = NULL;
	}

	~fmPhysics() {Exit();}
	// Methoden
	fmResult Init(char* pcFilename)
	{
		FILE* pFile;
		FILE* pLog;
		FILE* pPhyFile;

		if((pFile = fopen(pcFilename, "r")) == NULL)
		{
			fclose(pFile);
			FM_ERROR_B(pcFilename, FM_ERROR);
		}

		// Einlesen (Dateikopf)
		fscanf(pFile, "%s", m_cBuffer);	// Objectlist1...

		if((pLog = fopen("PhysicsLog.txt", "w")) == NULL)
		{
			fclose(pLog);
			FM_ERROR_B("Can't open Log to write!", FM_ERROR);
		}

		// Gravitation
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vGravity.x);
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vGravity.y);
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vGravity.z);

			// Login
			fprintf(pLog, "Gravity: (%f;%f;%f)", g_vGravity.x,
											     g_vGravity.y,
												 g_vGravity.z);

		// Sealevel
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_fSealevel);

			// Login
			fprintf(pLog, "\nSealevel: (%f)", g_fSealevel);

		// BuoyanceAir
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vBuoyanceAir.x);
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vBuoyanceAir.y);
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vBuoyanceAir.z);

			// Login
			fprintf(pLog, "\nBuoyanceAir: (%f;%f;%f)", g_vBuoyanceAir.x,
										    		   g_vBuoyanceAir.y,
													   g_vBuoyanceAir.z);

		// BuoyanceWater
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vBuoyanceWater.x);
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vBuoyanceWater.y);
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%f", &g_vBuoyanceWater.z);

			// Login
			fprintf(pLog, "\nBuoyanceWater: (%f;%f;%f)", g_vBuoyanceWater.x,
													     g_vBuoyanceWater.y,
												         g_vBuoyanceWater.z);

		//	Anzahl der Physikobjekte
		fscanf(pFile, "%s", m_cBuffer);
		fscanf(pFile, "%d", &m_dwNumPhysicObjects);
		
			// Login
			fprintf(pLog, "\nNumPhysicObjects: (%d)", m_dwNumPhysicObjects);

		if(m_dwNumPhysicObjects != 0)
		{
			m_pPhysicObject = new fmPhysicObject[m_dwNumPhysicObjects];

			if(m_pPhysicObject == NULL)
			{
				fclose(pLog);
				fclose(pFile);
				FM_ERROR_B("m_pPhysicObject", FM_ERROR);
			}

			// Laden der einzelnen Physikobjekte
			for(unsigned int a = 0; a < m_dwNumPhysicObjects; a++)
			{
				FM_INFO("Lade PhysikObjekt...");

				// Dateipfad
				fscanf(pFile, "%s", m_cBuffer);

					// Login
					fprintf(pLog, "\nFilename: %s", m_cBuffer);

				if((pPhyFile = fopen(m_cBuffer, "r")) == NULL)
				{
					fclose(pLog);
					fclose(pFile);
					fclose(pPhyFile);
					FM_ERROR_B(m_cBuffer, FM_ERROR);
				}

					// Laden eines Physikobjektes
					fscanf(pPhyFile, "%s", m_cBuffer);	// Bezeichner

					// Position im Weltenkoordinatensystem
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_vCenterOfMassWorld.x);
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_vCenterOfMassWorld.y);
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_vCenterOfMassWorld.z);

						// Login
						fprintf(pLog, "\nPosition PhyO: (%f;%f;%f)", m_pPhysicObject[a].m_vCenterOfMassWorld.x,
																	 m_pPhysicObject[a].m_vCenterOfMassWorld.y,
																	 m_pPhysicObject[a].m_vCenterOfMassWorld.z);

					// Startgeschwindigkeit
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_vVelocity.x);
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_vVelocity.y);
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_vVelocity.z);

						// Login
						fprintf(pLog, "\nVelocity PhyO: (%f;%f;%f)", m_pPhysicObject[a].m_vVelocity.x,
																	 m_pPhysicObject[a].m_vVelocity.y,
																	 m_pPhysicObject[a].m_vVelocity.z);

					// Kollisionsradius
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_fRadius);

						// Login
						fprintf(pLog, "\nCollisionsradius: (%f)", m_pPhysicObject[a].m_fRadius);

					// Wird im Quadrat gespeichert
					m_pPhysicObject[a].m_fSqrtRadius = m_pPhysicObject[a].m_fRadius;
					m_pPhysicObject[a].m_fRadius *= m_pPhysicObject[a].m_fRadius;

					// BuoyanceFaktor
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_fBuoyanceFactor);

						// Login
						fprintf(pLog, "\nBuoyanceFactor: (%f)", m_pPhysicObject[a].m_fBuoyanceFactor);

					// Anzahl der Massepunkte
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%d", &m_pPhysicObject[a].m_dwNumMassPoints);

						// Login
						fprintf(pLog, "\nNumMassPoints: (%d)", m_pPhysicObject[a].m_dwNumMassPoints);

						if(m_pPhysicObject[a].m_dwNumMassPoints != 0)
						{
							FM_INFO("Lade Massepunkte...");

							m_pPhysicObject[a].m_pMassPoint = new fmMassPoint[m_pPhysicObject[a].m_dwNumMassPoints];

							if(m_pPhysicObject[a].m_pMassPoint == NULL)
							{
								fclose(pPhyFile);
								fclose(pFile);
								fclose(pLog);
								FM_ERROR_B("m_pMassPoint", FM_ERROR);
							}

							for(unsigned int b = 0; b < m_pPhysicObject[a].m_dwNumMassPoints; b++)
							{
								// Masse Laden
								fscanf(pPhyFile, "%s", m_cBuffer);
								fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_pMassPoint[b].m_fMass);

									// Login
									fprintf(pLog, "\nMassePunktMasse: (%f)", m_pPhysicObject[a].m_pMassPoint[b].m_fMass);

								// Radius Laden
								fscanf(pPhyFile, "%s", m_cBuffer);
								fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_pMassPoint[b].m_fRadius);

									// Login
									fprintf(pLog, "\nCollisionsRadius: (%f)", m_pPhysicObject[a].m_pMassPoint[b].m_fRadius);

								// Radius im Quadrat speichern
								m_pPhysicObject[a].m_pMassPoint[b].m_fRadius *= m_pPhysicObject[a].m_pMassPoint[b].m_fRadius;

								// Position im Modelkoordinatensystem
								fscanf(pPhyFile, "%s", m_cBuffer);
								fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_pMassPoint[b].m_vModelPos.x);
								fscanf(pPhyFile, "%s", m_cBuffer);
								fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_pMassPoint[b].m_vModelPos.y);
								fscanf(pPhyFile, "%s", m_cBuffer);
								fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_pMassPoint[b].m_vModelPos.z);

									// Login
									fprintf(pLog, "\nPosition MassP: (%f;%f;%f)", m_pPhysicObject[a].m_pMassPoint[b].m_vModelPos.x,
																				  m_pPhysicObject[a].m_pMassPoint[b].m_vModelPos.y,
																			      m_pPhysicObject[a].m_pMassPoint[b].m_vModelPos.z);
							}
						}

					// Anzahl der Bindungen laden
					fscanf(pPhyFile, "%s", m_cBuffer);
					fscanf(pPhyFile, "%d", &m_pPhysicObject[a].m_dwNumBondings);

						// Login
						fprintf(pLog, "\nNumBondings: (%d)", m_pPhysicObject[a].m_dwNumBondings);

						if(m_pPhysicObject[a].m_dwNumBondings != 0)
						{
							FM_INFO("Lade Verbindungen...");

							m_pPhysicObject[a].m_pBonding = new fmBonding[m_pPhysicObject[a].m_dwNumBondings];

							if(m_pPhysicObject[a].m_pBonding == NULL)
							{
								fclose(pPhyFile);
								fclose(pFile);
								fclose(pLog);
								FM_ERROR_B("m_pBonding", FM_ERROR);
							}

							for(unsigned int b = 0; b < m_pPhysicObject[a].m_dwNumBondings; b++)
							{
								// Zu bindender Massepunkt 1
								fscanf(pPhyFile, "%s", m_cBuffer);
								fscanf(pPhyFile, "%d", &m_pPhysicObject[a].m_pBonding[b].m_dwNrOfBondedMassPoint1);

									// Login
									fprintf(pLog, "\nMassP1: (%d)", m_pPhysicObject[a].m_pBonding[b].m_dwNrOfBondedMassPoint1);
							
								// Zu bindender Massepunkt 2
								fscanf(pPhyFile, "%s", m_cBuffer);
								fscanf(pPhyFile, "%d", &m_pPhysicObject[a].m_pBonding[b].m_dwNrOfBondedMassPoint2);

									// Login
									fprintf(pLog, "\nMassP1: (%d)", m_pPhysicObject[a].m_pBonding[b].m_dwNrOfBondedMassPoint2);

								// Federkonstante
								fscanf(pPhyFile, "%s", m_cBuffer);
								fscanf(pPhyFile, "%f", &m_pPhysicObject[a].m_pBonding[b].m_fForceConstant);

									// Login
									fprintf(pLog, "\nMassP1: (%f)", m_pPhysicObject[a].m_pBonding[b].m_fForceConstant);
	
								// Berrechnung der Parameter
								m_pPhysicObject[a].m_pBonding[b].m_vNormalBondDistance = m_pPhysicObject[a].m_pMassPoint[m_pPhysicObject[a].m_pBonding[b].m_dwNrOfBondedMassPoint1].m_vModelPos -
																					     m_pPhysicObject[a].m_pMassPoint[m_pPhysicObject[a].m_pBonding[b].m_dwNrOfBondedMassPoint2].m_vModelPos;

								m_pPhysicObject[a].m_pBonding[b].m_fNormalDistance = fmVector3Length(m_pPhysicObject[a].m_pBonding[b].m_vNormalBondDistance);
							}
						}

				fclose(pPhyFile);
				
				// Anhanginformationen
				fscanf(pFile, "%s", m_cBuffer);

				// Zusatz informationen berrechnen
				for(unsigned int b = 0; b < m_pPhysicObject[a].m_dwNumMassPoints; b++)
				{
					m_pPhysicObject[a].m_fMass += m_pPhysicObject[a].m_pMassPoint[b].m_fMass;
					m_pPhysicObject[a].m_vCenterOfMassModel += m_pPhysicObject[a].m_pMassPoint[b].m_vModelPos *
															   m_pPhysicObject[a].m_pMassPoint[b].m_fMass;
				}

				m_pPhysicObject[a].m_vCenterOfMassModel /= m_pPhysicObject[a].m_fMass;

				m_pPhysicObject[a].m_vCenterOfMassWorld += m_pPhysicObject[a].m_vCenterOfMassModel;

				for(b = 0; b < m_pPhysicObject[a].m_dwNumMassPoints; b++)
				{
					m_pPhysicObject[a].m_pMassPoint[b].m_vVelocity = m_pPhysicObject[a].m_vVelocity;

					m_pPhysicObject[a].m_pMassPoint[b].m_vDistance = m_pPhysicObject[a].m_pMassPoint[b].m_vModelPos -
																	 m_pPhysicObject[a].m_vCenterOfMassModel;

					m_pPhysicObject[a].m_pMassPoint[b].m_vWorldPos = m_pPhysicObject[a].m_pMassPoint[b].m_vDistance +
																	 m_pPhysicObject[a].m_vCenterOfMassWorld;
				}
			}
		}

		fclose(pLog);
		fclose(pFile);

		FM_INFO("Die Physikengine ist Initialisiert");

		return FM_OK;
	}

	fmResult Exit()
	{
		FM_SAFE_DELETE_ARRAY(m_pPhysicObject);

		FM_INFO("Die Physikengine wurde Heruntergefahren");

		return FM_OK;
	}

	void Update(float fNumSecsPassed)
	{
		fmPhysicObject* pPhyObject1;
		fmPhysicObject* pPhyObject2;

		float fTimeStep		 = fNumSecsPassed;
		DWORD dwNumTimeSteps = 1;

		// ------------------------------------------------------------------
		// Schritt 1:
		// Berechnung der Zeitschritte
		if(fTimeStep > 0.005f) // Sekunden
		{
			dwNumTimeSteps = 2;
			fTimeStep = 0.5f * fNumSecsPassed;

			if(fTimeStep > 0.005f) // Sekunden
			{
				dwNumTimeSteps = 4;
				fTimeStep = 0.25f * fNumSecsPassed;

				if(fTimeStep > 0.005f) // Sekunden
				{
					dwNumTimeSteps = 6;
					fTimeStep = 0.16666f * fNumSecsPassed;

					if(fTimeStep > 0.005f) // Sekunden
					{
						dwNumTimeSteps = 8;
						fTimeStep = 0.125f * fNumSecsPassed;

						if(fTimeStep > 0.005f) // Sekunden
						{
							dwNumTimeSteps = 10;
							fTimeStep = 0.1f * fNumSecsPassed;

							if(fTimeStep > 0.005f) // Sekunden
							{
								dwNumTimeSteps = 12;
								fTimeStep = 0.083333f * fNumSecsPassed;

								if(fTimeStep > 0.005f) // Sekunden
								{
									dwNumTimeSteps = 14;
									fTimeStep = 0.07143f * fNumSecsPassed;
								}
							}
						}
					}
				}
			}	
		}

		for(unsigned int a = 0; a < m_dwNumPhysicObjects; a++)
		{
			pPhyObject1 = &m_pPhysicObject[a];

			for(unsigned int b = 0; b < m_dwNumPhysicObjects; b++)
			{
				pPhyObject2 = &m_pPhysicObject[b];

				if(a != b)
				{
					// Streckenlänge zwischen Objecten ermitteln
					g_vTemp1 = pPhyObject1->m_vCenterOfMassWorld -
							   pPhyObject2->m_vCenterOfMassWorld;
					g_fTemp1 = fmVector3LengthSq(g_vTemp1);

					g_fTemp2 = pPhyObject1->m_fRadius + pPhyObject2->m_fRadius +
							   2.0f * pPhyObject1->m_fSqrtRadius * pPhyObject2->m_fSqrtRadius;

					if(g_fTemp1 < g_fTemp2)
					{
						// Die Massepunkte paarweise auf Kollision überprüfen
						for(unsigned int i = 0; i < pPhyObject1->m_dwNumMassPoints; i++)
						{
							for(unsigned int j = 0; j < pPhyObject2->m_dwNumMassPoints; j++)
							{
								g_vTemp1 = pPhyObject1->m_pMassPoint[i].m_vWorldPos -
										   pPhyObject2->m_pMassPoint[j].m_vWorldPos;
								g_fTemp1 = fmVector3LengthSq(g_vTemp1);

								if(g_fTemp1 < g_fTemp2)
								{
									// Alte geschwindigkeit zwischen speichern
									g_vTemp2 = pPhyObject1->m_pMassPoint[i].m_vVelocity;
									g_vTemp3 = pPhyObject2->m_pMassPoint[j].m_vVelocity;
									
									Impulse(&pPhyObject1->m_pMassPoint[i].m_vVelocity,
										    &pPhyObject2->m_pMassPoint[j].m_vVelocity,
											pPhyObject1->m_pMassPoint[i].m_fMass,
											pPhyObject2->m_pMassPoint[j].m_fMass,
											&g_vTemp1);

									// Position im nächsten Frame berrechnen
									g_vTemp4 = pPhyObject1->m_pMassPoint[i].m_vWorldPos + pPhyObject1->m_pMassPoint[i].m_vVelocity * fNumSecsPassed;
									g_vTemp5 = pPhyObject2->m_pMassPoint[j].m_vWorldPos + pPhyObject2->m_pMassPoint[j].m_vVelocity * fNumSecsPassed;

									g_vTemp6 = g_vTemp4 - g_vTemp5;

									if(g_fTemp2 > fmVector3LengthSq(g_vTemp6))
									{
										pPhyObject1->m_pMassPoint[i].m_vVelocity = g_vTemp2;
										pPhyObject2->m_pMassPoint[j].m_vVelocity = g_vTemp3;
									}
								}
							}
						}
					}
				}
			}
		}

		for(DWORD dwNrOfTimeStep = 0; dwNrOfTimeStep < dwNumTimeSteps; dwNrOfTimeStep++)
		{
			for(a = 0; a < m_dwNumPhysicObjects; a++)
			{
				m_pPhysicObject[a].Move(fTimeStep);
			}
		}
		
		for(a = 0; a < m_dwNumPhysicObjects; a++)
		{
			m_pPhysicObject[a].SetTransformation();
		}
	}
	// Inline Methoden
	inline fmMatrix GetTransformation(DWORD dwPhysicObject) {return m_pPhysicObject[dwPhysicObject].m_mTransformation;}

	// Inline Methoden
	inline void Impulse(fmVector3* pvVelocity1,
						fmVector3* pvVelocity2,
						float&	   fMass1,
						float&	   fMass2,
						fmVector3* pvDifferenceCenterOfMass)
	{
		fmVector3 vCollisionsDirection;
		fmVector3 vCollisionsVertical1;
		fmVector3 vCollisionsVertical2;
		float	  fTemp1;
		float	  fTemp2;
		float	  fTemp3;
		float	  fTemp4;
		float	  fTemp5;
		float	  fTemp6;
		float	  fTemp7;
		float	  fTemp8;
		float	  fTemp9;
		float	  fTemp10;

		vCollisionsDirection = fmVector3Normalize(*pvDifferenceCenterOfMass);

		// Berechnung der Spannvectoren für die Kollisionsebene
		if(*pvVelocity1 != fmVector3(0.0f, 0.0f, 0.0f))
			vCollisionsVertical1 = fmVector3Cross(vCollisionsDirection, *pvVelocity1);
		else
			vCollisionsVertical1 = fmVector3Cross(vCollisionsDirection, *pvVelocity2);

		vCollisionsVertical1 = fmVector3Normalize(vCollisionsVertical1);

		vCollisionsVertical2 = fmVector3Cross(vCollisionsVertical1, vCollisionsDirection);

		
		fTemp1 = fmVector3Dot(*pvVelocity1, vCollisionsVertical1);
		fTemp2 = fmVector3Dot(*pvVelocity1, vCollisionsVertical2);
		fTemp3 = fmVector3Dot(*pvVelocity1, vCollisionsDirection);
		fTemp4 = fmVector3Dot(*pvVelocity2, vCollisionsVertical1);
		fTemp5 = fmVector3Dot(*pvVelocity2, vCollisionsVertical2);
		fTemp6 = fmVector3Dot(*pvVelocity2, vCollisionsDirection);

		// Die Geschwindigkeitsbeträge bezüglich der KollisionsSenkrechten
		// bleiben beim Stoss unverändert. 
		// Nur die beiden Geschwindigkeitsbeträge bezüglich der Kollisionsrichtung verändern sich!
		fTemp9  = fMass1 + fMass2;
		fTemp10 = fMass1 - fMass2;

		// Berechnung der Endgeschwindigkeiten bezüglich der Kollisionsachse
		fTemp7 = (2.0f * fMass2 * fTemp6 + fTemp3 * fTemp10) / fTemp9;
		fTemp8 = (2.0f * fMass1 * fTemp3 - fTemp6 * fTemp10) / fTemp9;

		// Die Entgeschwindigkeiten bezüglich der Kollisionsrichtung
		// und der KollisionsSenkrechten sind berechnet.
		// Jetzt müssen die Endgeschwindigkeiten bezüglich der x-, y- und z-Achse berechnet werden:

		*pvVelocity1 = fTemp1 * vCollisionsVertical1 + fTemp2 * vCollisionsVertical2 + fTemp7 * vCollisionsDirection;
		*pvVelocity2 = fTemp4 * vCollisionsVertical1 + fTemp5 * vCollisionsVertical2 + fTemp8 * vCollisionsDirection;
	}
};

// **************************************************************************