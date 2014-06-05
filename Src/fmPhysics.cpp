/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmPhysics.cpp
===============================
*/

#include <FatalisMatrix.h>

fmVector3	g_vGravity		 = fmVector3(0.0f, 0.0f, 0.0f);
fmVector3   g_vBuoyanceAir	 = fmVector3(0.0f, 0.0f, 0.0f);
fmVector3	g_vBuoyanceWater = fmVector3(0.0f, 0.0f, 0.0f);
float		g_fSealevel = 0.0f;

fmVector3	g_vTemp1 = fmVector3(0.0f, 0.0f, 0.0f);
fmVector3	g_vTemp2 = fmVector3(0.0f, 0.0f, 0.0f);
fmVector3	g_vTemp3 = fmVector3(0.0f, 0.0f, 0.0f);
fmVector3	g_vTemp4 = fmVector3(0.0f, 0.0f, 0.0f);
fmVector3	g_vTemp5 = fmVector3(0.0f, 0.0f, 0.0f);
fmVector3	g_vTemp6 = fmVector3(0.0f, 0.0f, 0.0f);
float		g_fTemp1 = 0.0f;
float		g_fTemp2 = 0.0f;