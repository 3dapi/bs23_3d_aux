//
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "LcUtil.h"



GUID IID_ILcTexInfo ={0x85c31227, 0x3de5, 0x4f00, 0x9b, 0x3a, 0xf1, 0x1a, 0xc3, 0x8c, 0x18, 0xb5};

INT	LcUtil_TextureLoad(LPDIRECT3DDEVICE9 pDev, LPDIRECT3DTEXTURE9* pTx, TCHAR* sFile, DWORD dc)
{
	HRESULT hr=-1;
	D3DXIMAGE_INFO* pImg = new D3DXIMAGE_INFO;

	if(FAILED(D3DXCreateTextureFromFileEx(
		pDev
		, sFile
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, D3DX_FILTER_NONE
		, D3DX_FILTER_NONE
		, dc
		, pImg
		, NULL
		, pTx
		)) )
	{
		pTx = NULL;
		return -1;
	}

//	hr = (*pTx)->SetPriority(0);
	hr = (*pTx)->SetPrivateData( IID_ILcTexInfo, &pImg, sizeof(pImg), 0);

	return 0;
}