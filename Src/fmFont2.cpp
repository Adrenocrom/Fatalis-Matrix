/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmFont2.cpp
===============================
*/

#include <FatalisMatrix.h>

fmFont2::fmFont2()
{
	m_lpD3DFont	    = NULL;
}

fmResult fmFont2::Init(LPCSTR			    pcFont2, 
			   		   INT				    iFont2Height)
{
    D3DXFONT_DESC    Font = { 0 };

    strcpy(Font.FaceName, pcFont2);
	Font.Height = iFont2Height;

    D3DXCreateFontIndirect(fmDirect3D::Instance().GetDevice(),
                           &Font,
                           &m_lpD3DFont);

	return FM_OK;
}

fmResult fmFont2::Draw(LPCSTR Text, int x, int y, fmColor TextColor)
{
	RECT r = { x, y, 0, 0 };

    m_lpD3DFont->DrawText(NULL, Text, -1, &r, DT_CALCRECT, TextColor);
    m_lpD3DFont->DrawText(NULL, Text, -1, &r, DT_CENTER, TextColor);

	return FM_OK;
}


fmResult fmFont2::Exit()
{
	FM_SAFE_RELEASE(m_lpD3DFont);

	return FM_OK;
}