/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmConfig.h
===============================
*/

#pragma once

class FATALISMATRIX_API fmConfig
{
public:
	struct
	{
		int					iAdapter;				// Adapter-ID
		D3DDEVTYPE			DeviceType;				// Gerätetyp
		BOOL				bWindowed;				// Fenstermodus?
		D3DDISPLAYMODE		VideoMode;				// Videomodus
		D3DFORMAT			BackBufferFormat;		// Bildpufferformat
		D3DFORMAT			ZStencilBufferFormat;	// Z-Stencil-Buffer-Format
		D3DMULTISAMPLE_TYPE	MultiSamplingType;		// Multi-Sampling-Typ
		DWORD				dwMultiSamplingQuality;	// Multi-Sampling-Qualität
		DWORD				dwFlags;				// Flags (Hardware oder Software?)
		BOOL				bVSync;					// V-Sync einschalten?

	} Direct3D;
};
