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
	char b_cBuffer;

	// Physics
	fmVector3 b_vGravity;
	float	  b_fSealevel;

	fmVector3 b_vBuoyanceAir;
	fmVector3 b_vBuoyanceWater;

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
	fmVector3	m_vModelPos;
	fmVector3	m_vWorldPos;
	fmVector3	m_vDistance;
	fmVector3	m_vVelocity;
	fmVector3	m_vForce;
	float		m_fMass;
	float		m_fRadius;
};

class fmBonding : public fmPhysics
{
public:
	DWORD		m_dwNumOfBondedMassPoint1;
	DWORD		m_dwNumOfBondedMassPoint2;
	fmVector3   m_vNormalBondDistance;
	fmVector3   m_vElongationForceDirection;
	fmVector3	m_vForceOnBondedMassPoint1;
	fmVector3   m_vForceOnBondedMassPoint2;
	float		m_fForceConstant;
	float		m_fNormalDistance;
	float		m_fElongation;

	inline void Update(fmVector3* pvActualBondDistance)
	{
		m_fElongation = fmVector3Length(*pvActualBondDistance) - m_fNormalDistance;
		m_vElongationForceDirection = fmVector3Normalize(*pvActualBondDistance);

		m_vForceOnBondedMassPoint1 = -m_fElongation * m_fForceConstant * m_vElongationForceDirection;
		m_vForceOnBondedMassPoint2 =  m_fElongation * m_fForceConstant * m_vElongationForceDirection;
	}
};

class fmPhysicObject : public fmPhysics
{
public:
	// Variablen
	DWORD			m_dwNumMassPoints;
	fmMassPoint*	m_pMassPoint;

	DWORD			m_dwNumBondings;
	fmBonding*		m_pBonding;

	fmVector3	m_vModelCenterOfMass;
	fmVector3	m_vWorldCenterOfMass;
	fmVector3	m_vVelocity;
	float		m_fMass;
	float		m_fRadius;
	float		m_fBouyanceFactor;

	// Konstruktor und Destruktor
	fmPhysicObject()
	{

	}

	~fmPhysicObject()
	{
		FM_SAFE_DELETE_ARRAY(m_pMassPoint);
	}

	fmResult Load(char* pcFilename)
	{
		FILE* pFile;

		if((pFile = fopen(pcFilename, "r")) == 0)
		{
			fclose(pFile);
			FM_ERROR_B(pcFilename, FM_ERROR);
		}

		fscanf(pFile, "%s", b_cBuffer);

		// Position
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vWorldCenterOfMass.x);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vWorldCenterOfMass.y);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vWorldCenterOfMass.z);
		// Geschwindigkeit
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vVelocity.x);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vVelocity.y);
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_vVelocity.z);
		// Kollisionsradius
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%f", &m_fRadius);
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
			{
				// Masse
				fscanf(pFile, "%s", b_cBuffer);
				fscanf(pFile, "%f", &m_pMassPoint[i].m_fMass);
				// Kollisionsradius
				fscanf(pFile, "%s", b_cBuffer);
				fscanf(pFile, "%f", &m_pMassPoint[i].m_fRadius);
				// Position(Modelkoordinaten)
				// Masse
				fscanf(pFile, "%s", b_cBuffer);
				fscanf(pFile, "%f", &m_pMassPoint[i].m_vModelPos.x);
				fscanf(pFile, "%s", b_cBuffer);
				fscanf(pFile, "%f", &m_pMassPoint[i].m_vModelPos.y);
				fscanf(pFile, "%s", b_cBuffer);
				fscanf(pFile, "%f", &m_pMassPoint[i].m_vModelPos.z);
			}
		}

		// Anzahl an Verbindungen
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%d", &m_dwNumBondings);

		if(m_dwNumBondings != 0)
		{
			m_pBonding = new fmBonding[m_dwNumBondings];

			for(unsigned int i = 0; i < m_dwNumBondings; i++)
			{
				// Gebundene Massepunkte (2)
				fscanf(pFile, "%s", b_cBuffer);
				fscanf(pFile, "%d", &m_pBonding[i].m_dwNumOfBondedMassPoint1);
				fscanf(pFile, "%s", b_cBuffer);
				fscanf(pFile, "%d", &m_pBonding[i].m_dwNumOfBondedMassPoint2);
				// Force Konstante
				fscanf(pFile, "%s", b_cBuffer);
				fscanf(pFile, "%f", &m_pBonding[i].m_fForceConstant);

				m_pBonding[i].m_vNormalBondDistance = m_pMassPoint[m_pBonding[i].m_dwNumOfBondedMassPoint1].m_vModelPos -
													  m_pMassPoint[m_pBonding[i].m_dwNumOfBondedMassPoint1].m_vModelPos;

				m_pBonding[i].m_fNormalDistance = fmVector3Length(m_pBonding[i].m_vNormalBondDistance);
			}
		}

		// Fehlende Daten des PhysikObjektes berechnen
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
};

class fmSimulatePhysic : public fmPhysics
{
public:
	// Variablen
	DWORD			m_dwNumPhysicObjects;
	fmPhysicObject* m_pPhysicObject;

	// Konstruktor und Destruktor
	fmSimulatePhysic()
	{

	}

	~fmSimulatePhysic()
	{
		FM_SAFE_DELETE_ARRAY(m_pPhysicObject);
	}

	fmResult Init(char* pcFilename)
	{
		FILE* pFile;

		if((pFile = fopen(pcFilename, "r")) == 0)
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
		// NumPhysicObjects
		fscanf(pFile, "%s", b_cBuffer);
		fscanf(pFile, "%d", &m_dwNumPhysicObjects);
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
	

		if(m_dwNumPhysicObjects != 0)
		{
			m_pPhysicObject = new fmPhysicObject[m_dwNumPhysicObjects];

			for(unsigned int i = 0; i < m_dwNumPhysicObjects; i++)
			{
				fscanf(pFile, "%s", b_cBuffer);
				m_pPhysicObject[i].Load(&b_cBuffer);
				fscanf(pFile, "%s", b_cBuffer);
			}
		}

		fclose(pFile);

		return FM_OK;
	}

	void Update(float fNumSecsPassed)
	{
		fmPhysicObject* pPhyObject1;
		fmPhysicObject* pPhyObject2;
		fmVector3 vAcceleration;
		fmVector3 vTemp1;
		float fTemp1;
		float fTemp2;

		// ------------------------------------------------------------------
		// Schritt 1:
		// Auf Kollision aller Körper paarweise überprüfen
		for(unsigned int i = 0; i < m_dwNumPhysicObjects; i++)
		{
			pPhyObject1 = &m_pPhysicObject[i];

			for(unsigned int j = 0; j < m_dwNumPhysicObjects; j++)
			{
				pPhyObject2 = &m_pPhysicObject[j];

				vTemp1 = pPhyObject1->m_vWorldCenterOfMass - pPhyObject2->m_vWorldCenterOfMass;
				fTemp1 = fmVector3LengthSq(vTemp1); // Betrag der Strecke M1M2

				fTemp2 = pPhyObject1->m_fRadius + pPhyObject2->m_fRadius;
				fTemp2 = (float)(fabs(fTemp2)); // Betrag der addierten Radien

				if(fTemp1 <= fTemp2)
				{
					// ------------------------------------------------------
					// Schritt 2:
					// Kollision der Massepunkte paarweise überprüfen
					for(unsigned int a = 0; a < pPhyObject1->m_dwNumMassPoints; a++)
					{
						for(unsigned int b = 0; b < pPhyObject2->m_dwNumMassPoints; b++)
						{
							vTemp1 = pPhyObject1->m_pMassPoint[a].m_vWorldPos -
									 pPhyObject2->m_pMassPoint[b].m_vWorldPos;
							fTemp1 = fmVector3LengthSq(vTemp1);	// Betrag der Strecke M1M2

							fTemp2 = pPhyObject1->m_pMassPoint[a].m_fRadius +
									 pPhyObject2->m_pMassPoint[b].m_fRadius;
							fTemp2 = (float)(fabs(fTemp2));	// Betrag der addierten Radien

							if(fTemp1 <= fTemp2)
							{
								// ------------------------------------------
								// Schritt 3:
								// Kraft wirkungen berechnen
								Impulse(&pPhyObject1->m_pMassPoint[a].m_vVelocity,
										&pPhyObject2->m_pMassPoint[b].m_vVelocity,
										pPhyObject1->m_pMassPoint[a].m_fMass,
										pPhyObject2->m_pMassPoint[b].m_fMass,
										&vTemp1);

								// Position der Massepunkte anpassen
								pPhyObject1->m_pMassPoint[a].m_vWorldPos += pPhyObject1->m_pMassPoint[a].m_vVelocity *
																			fNumSecsPassed;
								pPhyObject2->m_pMassPoint[b].m_vWorldPos += pPhyObject2->m_pMassPoint[b].m_vVelocity *
																			fNumSecsPassed;
							}
						}
					}					
				}
			}
		}

		// ------------------------------------------------------------------
		// Schritt 4:
		// Natürliche Kräfte berechnen
		for(i = 0; i < m_dwNumPhysicObjects; i++)
		{
			for(unsigned int j = 0; j < m_pPhysicObject[i].m_dwNumMassPoints; j++)
			{
				m_pPhysicObject[i].m_pMassPoint[j].m_vForce = fmVector3(0.0f, 0.0f, 0.0f);
			}
		}

		for(i = 0; i < m_dwNumPhysicObjects; i++)
		{
			pPhyObject1 = &m_pPhysicObject[i];

			// ----------------------------------------------------------
			// Schritt 5:
			// Kraft auf alle Massepunkte des jeweiligen Körpers übertragen				
			for(unsigned int a = 0; a < pPhyObject1->m_dwNumBondings; a++)
			{
				vTemp1 = pPhyObject1->m_pMassPoint[pPhyObject1->m_pBonding[a].m_dwNumOfBondedMassPoint1].m_vModelPos -
						 pPhyObject1->m_pMassPoint[pPhyObject1->m_pBonding[a].m_dwNumOfBondedMassPoint2].m_vModelPos;

				pPhyObject1->m_pBonding[a].Update(&vTemp1);

				pPhyObject1->m_pMassPoint[pPhyObject1->m_pBonding[a].m_dwNumOfBondedMassPoint1].m_vForce += pPhyObject1->m_pBonding[a].m_vForceOnBondedMassPoint1;
				pPhyObject1->m_pMassPoint[pPhyObject1->m_pBonding[a].m_dwNumOfBondedMassPoint2].m_vForce += pPhyObject1->m_pBonding[a].m_vForceOnBondedMassPoint2;
			}

			for(unsigned int j = 0; j < pPhyObject1->m_dwNumMassPoints; j++)
			{	
				if(pPhyObject1->m_pMassPoint[j].m_vWorldPos.y > b_fSealevel)
					vAcceleration = pPhyObject1->m_pMassPoint[j].m_vForce + b_vGravity + (pPhyObject1->m_fBouyanceFactor * b_vBuoyanceAir);
				else
					vAcceleration = pPhyObject1->m_pMassPoint[j].m_vForce + b_vGravity + (pPhyObject1->m_fBouyanceFactor * b_vBuoyanceWater);

				pPhyObject1->m_pMassPoint[j].m_vVelocity += vAcceleration * fNumSecsPassed;
			}
		}
	}
};