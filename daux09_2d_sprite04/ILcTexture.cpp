// Implementation of the CLcTexture class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#include "ILcTexture.h"


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }




class CLcTexture : public ILcTexture
{
protected:
	LPDIRECT3DTEXTURE9		m_pTex;
	D3DXIMAGE_INFO			m_pInf;

public:
	CLcTexture();
	virtual ~CLcTexture();

	virtual	void*	GetTexture(){	return m_pTex;			}
	virtual	INT		GetImgW()	{	return m_pInf.Width;	}
	virtual	INT		GetImgH()	{	return m_pInf.Height;	}
	virtual	INT		GetImgD()	{	return m_pInf.Depth;	}

	INT		Create(LPDIRECT3DDEVICE9 pDev, char* sFile, DWORD dFilter=-1, DWORD dColorKey=0x00FFFFFF);
	void	Destory();
};



CLcTexture::CLcTexture()
{
	m_pTex	= NULL;

	memset(&m_pInf, 0, sizeof(D3DXIMAGE_INFO));
}


CLcTexture::~CLcTexture()
{
	Destory();
}


void CLcTexture::Destory()
{
	SAFE_RELEASE(	m_pTex	);
}


INT CLcTexture::Create(LPDIRECT3DDEVICE9 pDev, char* sFile, DWORD dFilter, DWORD dColorKey)
{
	HRESULT hr=-1;

	UINT MipLevels = (dFilter==-1? -1 : 1);

//	hr = D3DXCreateTextureFromFileEx(pDev, sFile
//				, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0
//				, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &m_pInf, NULL, &m_pTex);

	hr = D3DXCreateTextureFromFileEx(pDev, sFile
				, D3DX_DEFAULT, D3DX_DEFAULT, MipLevels, 0
				, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, dFilter, dFilter, dColorKey, &m_pInf, NULL, &m_pTex);

	if(FAILED(hr))
	{
		m_pTex = NULL;
		return -1;
	}

	return 0;
}






INT LcxCreateTextureFromFile(ILcTexture** pOut, void* pDev, char* sFile, DWORD dFilter, DWORD dColor)
{
	CLcTexture*	pObj=NULL;

	pObj = new CLcTexture;

	if(FAILED(pObj->Create((LPDIRECT3DDEVICE9)pDev, sFile, dFilter, dColor)))
	{
		delete pObj;
		return -1;
	}

	*pOut = pObj;
	return 0;
}

