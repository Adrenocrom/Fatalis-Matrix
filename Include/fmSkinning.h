/*
===============================
 Program: FATALISMATRIX
 Writer: Josef Schulz
 File: fmSkining.h
===============================
*/

#pragma once

class fmAllocateHierarchy : ID3DXAllocateHierarchy
{
public:
// The format of these interfaces is defined by D3DXLoadMeshHierarchyFromX

   // callback to create a D3DXFRAME derived object and initialize it
   STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );

   // callback to create a D3DXMESHCONTAINER derived object and initialise it
   STDMETHOD( CreateMeshContainer )( THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData,  CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer );

   // callback to release a D3DXFRAME derived object
   STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree );

   // callback to release a D3DXMESHCONTAINER derived object
   STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree );
};

struct D3DXFRAME_DERIVED: public D3DXFRAME
{
    D3DXMATRIXA16 exCombinedTransformationMatrix;
};

struct D3DXMESHCONTAINER_EXTENDED: public D3DXMESHCONTAINER
{
    IDirect3DTexture9**  exTextures;  // Array of texture pointers 
    D3DMATERIAL9*   exMaterials;      // Array of materials

    // Skinned mesh variables
    ID3DXMesh*     exSkinMesh;					 // The skin mesh
    D3DXMATRIX*    exBoneOffsets;				 // The bone matrix Offsets
    D3DXMATRIX**   exFrameCombinedMatrixPointer; // Array of frame matrix
};

