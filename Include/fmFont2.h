/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmFont2.h
===============================
*/

#pragma once

class FATALISMATRIX_API fmFont2
{
	private:

		LPD3DXFONT			m_lpD3DFont;

	public:

		fmFont2();
		~fmFont2() {Exit();}


		fmResult Init(LPCSTR			pcFont2, 
					  INT				iFont2Height);
		fmResult Draw(LPCSTR Text, int x, int y, fmColor TextColor);
		fmResult Exit();
};