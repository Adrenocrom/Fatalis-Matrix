/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmPhysics.h
===============================
*/

#pragma once

class fmPhysics
{
public:
	// Variablen
	char	b_cBuffer[200];

	fmVector3 b_vGravity;
	fmVector3 b_vBuoyanceAir;
	fmVector3 b_vBuoyanceWater;
	float	  b_fSealevel;

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

class fmMassPoint : public fmPhysics
{
public:
	// Variablen
	fmVector3	m_vModelPos;
	fmVector3   m_vWorldPos;
	fmVector3	m_vDistance;
	fmVector3	m_vVelocity;
	fmVector3	m_vForce;
	float		m_fRadius;
	float		m_fMass;
	float		m_fInvMass;
	// Konstructor und Destructor
	fmMassPoint()
	{
		m_vModelPos = fmVector3(0.0f, 0.0f, 0.0f);
		m_vWorldPos = fmVector3(0.0f, 0.0f, 0.0f);
		m_vDistance = fmVector3(0.0f, 0.0f, 0.0f);
		m_vVelocity = fmVector3(0.0f, 0.0f, 0.0f);
		m_vForce    = fmVector3(0.0f, 0.0f, 0.0f);
		m_fRadius   = 0.0f;
		m_fMass     = 0.0f;
		m_fInvMass	= 0.0f;
	}

	fmResult Load(FILE* pFile)
	{
		// Masse
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_fMass);
		// InvMass
		m_fInvMass = 1.0f / m_fMass;
		// Kollisionsradius (Quadrat)
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_fRadius);

		m_fRadius *= m_fRadius;
		// Modelkoordinaten
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vModelPos.x);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vModelPos.y);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vModelPos.z);

		return FM_OK;
	}

	void Move(float fTimeStep, float fBouyanceFactor)
	{
		fmVector3 vAcceleration = fmVector3(0.0f, 0.0f, 0.0f);

		// wenn keine der Massepunkt Kollisionsfrei ist
		if(m_fRadius == 0.0f)
		{
			if(m_vWorldPos.y > b_fSealevel)
				vAcceleration = m_vForce * m_fInvMass + b_vGravity + (b_vBuoyanceAir * fBouyanceFactor);
			else
				vAcceleration = m_vForce * m_fInvMass + b_vGravity + (b_vBuoyanceWater * fBouyanceFactor);
		}
		else
		{
			if(m_vWorldPos.y > b_fSealevel)
				vAcceleration = m_vForce * m_fInvMass + b_vGravity + (b_vBuoyanceAir * fBouyanceFactor);
			else
				vAcceleration = m_vForce * m_fInvMass + b_vGravity + (b_vBuoyanceWater * fBouyanceFactor);
		}

		m_vVelocity += vAcceleration * fTimeStep;

		m_vWorldPos += m_vVelocity * fTimeStep;

		m_vForce = fmVector3(0.0f, 0.0f, 0.0f);
	}
};

class fmBonding : public fmPhysics
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

	// Methoden
	fmResult Load(FILE* pFile)
	{
		// MassPointNr1
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%d", &m_dwNrOfBondedMassPoint1);
		// MassPointNr2
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%d", &m_dwNrOfBondedMassPoint2);
		// Kraft konstante
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_fForceConstant);

		return FM_OK;
	}
};

class fmPhysicObject : public fmPhysics
{
public:
	// Variablen
	DWORD	     m_dwNumMassPoints;
	fmMassPoint* m_pMassPoint;
	DWORD		 m_dwNumBondings;
	fmBonding*   m_pBonding;

	fmVector3	 m_vModelCenterOfMass;
	fmVector3	 m_vWorldCenterOfMass;
	fmVector3	 m_vVelocity;
	float	     m_fMass;
	float		 m_fRadius;
	float		 m_fBouyanceFactor;

	fmMatrix	 m_mTransformation;

	// Konstruktor und Destruktor
	fmPhysicObject()
	{
		m_dwNumMassPoints = 0;
		m_dwNumBondings   = 0;
		m_pMassPoint = NULL;
		m_pBonding   = NULL;
		m_vModelCenterOfMass = fmVector3(0.0f, 0.0f, 0.0f);
		m_vWorldCenterOfMass = fmVector3(0.0f, 0.0f, 0.0f);
		m_vVelocity			 = fmVector3(0.0f, 0.0f, 0.0f);
		m_fMass           = 0.0f;
		m_fRadius         = 0.0f;
		m_fBouyanceFactor = 0.0f;
		m_mTransformation = fmMatrixIdentity();
	}

	~fmPhysicObject()
	{
		FM_SAFE_DELETE_ARRAY(m_pMassPoint);
		FM_SAFE_DELETE_ARRAY(m_pBonding);
	}

	fmResult Load(char* pcFilename)
	{
		FILE* pFile;

		if((pFile = fopen(pcFilename, "r")) == NULL)
		{
			fclose(pFile);
			FM_ERROR_B(pcFilename, FM_ERROR);
		}
		
		fscanf(pFile, "%s", b_cBuffer);

		// Position
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vModelCenterOfMass.x);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vModelCenterOfMass.y);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vModelCenterOfMass.z);
		// Geschwindigkeit
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vVelocity.x);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vVelocity.y);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vVelocity.z);
		// Kollisionsradius (Quadrat)
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_fRadius);

		m_fRadius *= m_fRadius;
		// Auftrieb
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_fBouyanceFactor);
		// Anzahl an Massepunkten
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%d", &m_dwNumMassPoints);

		if(m_dwNumMassPoints != 0)
		{
			m_pMassPoint = new fmMassPoint[m_dwNumMassPoints];

			for(unsigned int i = 0; i < m_dwNumMassPoints; i++)
				m_pMassPoint[i].Load(pFile);

			// Anzahl der Verbindungen
			fscanf(pFile, "%s", b_cBuffer);
			fscanf(pFile, "%d", &m_dwNumBondings);

			if(m_dwNumBondings != 0)
			{
				m_pBonding = new fmBonding[m_dwNumBondings];

				for(i = 0; i < m_dwNumBondings; i++)
				{
					m_pBonding[i].Load(pFile);

					// Fehlende Daten berechnen
					m_pBonding[i].m_vNormalBondDistance = m_pMassPoint[m_pBonding[i].m_dwNrOfBondedMassPoint1].m_vModelPos -
														  m_pMassPoint[m_pBonding[i].m_dwNrOfBondedMassPoint1].m_vModelPos;

					m_pBonding[i].m_fNormalDistance = fmVector3Length(m_pBonding[i].m_vNormalBondDistance);
				}
			}
		}

		// Fehlende Daten berechnen
		for(unsigned int i = 0; i < m_dwNumMassPoints; i++)
		{
			m_fMass += m_pMassPoint[i].m_fMass;
			m_vModelCenterOfMass += m_pMassPoint[i].m_vModelPos * m_pMassPoint[i].m_fMass;
		}

		m_vModelCenterOfMass /= m_fMass;

		m_vWorldCenterOfMass += m_vModelCenterOfMass;

		// Fehlende Daten der Massepunkte berrechnen
		for(i = 0; i < m_dwNumMassPoints; i++)
		{
			m_pMassPoint[i].m_vVelocity = m_vVelocity;
			m_pMassPoint[i].m_vDistance = m_pMassPoint[i].m_vModelPos - m_vModelCenterOfMass;
			m_pMassPoint[i].m_vWorldPos = m_pMassPoint[i].m_vDistance + m_vWorldCenterOfMass;
		}

		fclose(pFile);

		return FM_OK;
	}

	void Move(float fTimeStep)
	{
		fmVector3 vTemp1;

		if(m_dwNumMassPoints == 1)	// Object verhält sich wie ein Massepunkt
		{
			m_pMassPoint[0].Move(fTimeStep, m_fBouyanceFactor);
		}
		else
		{
			for(unsigned int i = 0; i < m_dwNumBondings; i++)
			{
				vTemp1 = m_pMassPoint[m_pBonding[i].m_dwNrOfBondedMassPoint1].m_vWorldPos -
						 m_pMassPoint[m_pBonding[i].m_dwNrOfBondedMassPoint2].m_vWorldPos;

				m_pBonding[i].Update(&vTemp1);

				m_pMassPoint[m_pBonding[i].m_dwNrOfBondedMassPoint1].m_vForce += m_pBonding[i].m_vForceOnBondedMassPoint1;
				m_pMassPoint[m_pBonding[i].m_dwNrOfBondedMassPoint2].m_vForce += m_pBonding[i].m_vForceOnBondedMassPoint2;
			}
			
			for(i = 0; i < m_dwNumMassPoints; i++)
			{
				m_pMassPoint[i].Move(fTimeStep, m_fBouyanceFactor);
			}
		}
	}

	void SetTransformation(float fNumSecsPassed)
	{
		fmMatrix  mRotation;
		fmMatrix  mTranslation;
		fmVector3 vTemp1;
		fmVector3 vTemp2;
		

		// ------------------------------------------------------------------
		// Translationsmatrix bestimmen
		vTemp1 = m_vWorldCenterOfMass;

		// Schwerpunkt Position und Masse neu bestimmen
		m_vWorldCenterOfMass = fmVector3(0.0f, 0.0f, 0.0f);

		for(unsigned int i = 0; i < m_dwNumMassPoints; i++)
			m_vWorldCenterOfMass += m_pMassPoint[i].m_vWorldPos * m_pMassPoint[i].m_fMass;

		m_vWorldCenterOfMass /= m_fMass;

		for(i = 0; i < m_dwNumMassPoints; i++)
		{
			m_pMassPoint[i].m_vDistance = m_pMassPoint[i].m_vWorldPos - m_vWorldCenterOfMass;
		}

		m_vVelocity = (m_vWorldCenterOfMass - vTemp1) / fNumSecsPassed;

		mTranslation = fmMatrixTranslation(m_vWorldCenterOfMass);

		// ------------------------------------------------------------------
		// Rotationsmatrix bestimmen
		
		mRotation = fmMatrixIdentity();

		if(m_dwNumMassPoints == 2)
		{
			
			
		}
		else if(m_dwNumMassPoints > 6)
		{
			fmVector3 vNormal1;
			fmVector3 vNormal2;
			fmVector3 vNormal3;

			// Normale in x-Richtung (in der Anfangsorientierung):
			vTemp1 = m_pMassPoint[2].m_vWorldPos - m_pMassPoint[6].m_vWorldPos;
			vTemp2 = m_pMassPoint[5].m_vWorldPos - m_pMassPoint[6].m_vWorldPos;

			vNormal1 = fmVector3Cross(vTemp1, vTemp2);
			vNormal1 = fmVector3Normalize(vNormal1);

			// Normale in y-Richtung (in der Anfangsorientierung):
			vTemp1 = m_pMassPoint[4].m_vWorldPos - m_pMassPoint[0].m_vWorldPos;
			vTemp2 = m_pMassPoint[1].m_vWorldPos - m_pMassPoint[0].m_vWorldPos;

			vNormal2 = fmVector3Cross(vTemp1, vTemp2);
			vNormal2 = fmVector3Normalize(vNormal2);

			// Normale in z-Richtung (in der Anfangsorientierung):
			vTemp1 = m_pMassPoint[7].m_vWorldPos - m_pMassPoint[4].m_vWorldPos;
			vTemp2 = m_pMassPoint[5].m_vWorldPos - m_pMassPoint[4].m_vWorldPos;

			vNormal3 = fmVector3Cross(vTemp1, vTemp2);
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

		m_mTransformation = mRotation * mTranslation;
	}
};

class fmSimulatePhysic : public fmPhysics
{
public:
	// Varibalen
	DWORD		    m_dwNumPhysicObjects;
	fmPhysicObject* m_pPhysicObject;
	
	// Konstruktor und Destruktor
	fmSimulatePhysic()
	{
		m_dwNumPhysicObjects = 0;
		m_pPhysicObject = NULL;
	}

	~fmSimulatePhysic()
	{
		FM_SAFE_DELETE_ARRAY(m_pPhysicObject);
	}

	// Inline Methoden
	fmMatrix GetTransformation(DWORD dwID)
	{
		return m_pPhysicObject[dwID].m_mTransformation;
	}

	// Methoden
	fmResult Init(char* pcFilename)
	{
		FILE* pFile;

		if((pFile = fopen(pcFilename, "r")) == NULL)
		{
			fclose(pFile);
			FM_ERROR_B(pcFilename, FM_ERROR);
		}

		fscanf(pFile, "%s", b_cBuffer);

		// Gravitation
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vGravity.x);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vGravity.y);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vGravity.z);
		// Seelevel
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_fSealevel);
		// Auftrib in der Luft
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vBuoyanceAir.x);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vBuoyanceAir.y);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vBuoyanceAir.z);
		// Auftrib im Wasser
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vBuoyanceWater.x);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vBuoyanceWater.y);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &b_vBuoyanceWater.z);
		// NumPhysicObjects
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%d", &m_dwNumPhysicObjects);

		if(m_dwNumPhysicObjects != 0)
		{
			m_pPhysicObject = new fmPhysicObject[m_dwNumPhysicObjects];

			for(unsigned int i = 0; i < m_dwNumPhysicObjects; i++)
			{
				fscanf(pFile, "%s", b_cBuffer);
				m_pPhysicObject[i].Load(b_cBuffer);
				fscanf(pFile, "%s", b_cBuffer);
			}
		}

		fclose(pFile);

		return FM_OK;
	}

	fmResult Update(float fNumSecsPassed)
	{
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

		// ------------------------------------------------------------------
		// Schritt 2:
		// Paarweise Kollisionsüberprüfung aller Körper
		fmPhysicObject* pPhyObject1;
		fmPhysicObject* pPhyObject2;
		fmVector3		vTemp1;
		float			fTemp1;
		float			fTemp2;


		for(unsigned int a = 0; a < m_dwNumPhysicObjects; a++)
		{
			pPhyObject1 = &m_pPhysicObject[a];

			for(unsigned int b = 0; b < m_dwNumPhysicObjects; b++)
			{
				pPhyObject2 = &m_pPhysicObject[b];

				if(a != b)
				{
					// Streckenlänge zwischen Objecten ermitteln
					vTemp1 = pPhyObject1->m_vWorldCenterOfMass -
							 pPhyObject2->m_vModelCenterOfMass;
					fTemp1 = fmVector3LengthSq(vTemp1);

					fTemp2 = pPhyObject1->m_fRadius +
							 pPhyObject2->m_fRadius;

					if(fTemp1 <= fTemp2)
					{
						// Die Massepunkte paarweise auf Kollision überprüfen
						for(unsigned int i = 0; i < pPhyObject1->m_dwNumMassPoints; i++)
						{
							for(unsigned int j = 0; j < pPhyObject2->m_dwNumMassPoints; j++)
							{
								vTemp1 = pPhyObject1->m_pMassPoint[i].m_vWorldPos -
										 pPhyObject2->m_pMassPoint[j].m_vWorldPos;
								fTemp1 = fmVector3LengthSq(vTemp1);

								fTemp2 = pPhyObject1->m_pMassPoint[i].m_fRadius +
										 pPhyObject2->m_pMassPoint[j].m_fRadius;

								if(fTemp1 <= fTemp2)
								{
									Impulse(&pPhyObject1->m_pMassPoint[i].m_vVelocity,
										    &pPhyObject2->m_pMassPoint[j].m_vVelocity,
											pPhyObject1->m_pMassPoint[i].m_fMass,
											pPhyObject2->m_pMassPoint[j].m_fMass,
											&vTemp1);
								}
							}
						}
					}
				}
			}
		}
		// ------------------------------------------------------------------
		// Schritt 3:
		// alle Körper bewegen
		for(DWORD dwNrOfTimeStep = 0; dwNrOfTimeStep < dwNumTimeSteps; dwNrOfTimeStep++)
		{
			for(a = 0; a < m_dwNumPhysicObjects; a++)
			{
				m_pPhysicObject[a].Move(fTimeStep);
			}
		}

		// ------------------------------------------------------------------
		// Schritt 4:
		// Erzeugung der Transformationsmatrizen
		for(a = 0; a < m_dwNumPhysicObjects; a++)
		{
			m_pPhysicObject[a].SetTransformation(fNumSecsPassed);
		}

		return FM_OK;
	}
};