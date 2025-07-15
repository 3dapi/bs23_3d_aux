// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


MEMORYSTATUSEX	g_MmSt;		// 전역 변수


DWORDLONG LnUtil_GetUsedMemory()
{
	//MEMORYSTATUS	st;
	//GlobalMemoryStatus(&st);	// it cannot work on 64bit
	//return ((g_MmSt.dwAvailPhys - st.dwAvailPhys)/1024);						// KByte

	MEMORYSTATUSEX st;
	st.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&st);

	return ((g_MmSt.ullAvailPhys - st.ullAvailPhys)/1024);						// KByte
}


CMain::CMain()
{
}


HRESULT CMain::Init()
{
	HRESULT hr=-1;

	D3DXFONT_DESC hFont =
	{
		16, 0
		, FW_BOLD, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS
		, ANTIALIASED_QUALITY, FF_DONTCARE
		, "Arial"
	};

	if( FAILED( hr = D3DXCreateFontIndirect( m_pd3dDevice, &hFont, &m_pD3DXFont ) ) )
		return -1;


	g_MmSt.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&g_MmSt);





	CHAR				sFile[128]	;
	LPDIRECT3DTEXTURE9	pTexture[100];
	DWORD				dColor		;
	
	D3DXIMAGE_INFO		pSrcInf		;
	DWORD				Filter		;
	DWORD				MipFilter	;
	D3DFORMAT			d3Fmt		;

	FLOAT	Used1	= 0;
	FLOAT	Used2	= 0;
	FLOAT	Before	= 0;
	FLOAT	After	= 0;
	INT		nCnt	= 10;

	int		i=0;

	strcpy(sFile, "Texture/Lenna.jpg");
	dColor=0x00FFFFFF;

	Filter		= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR);
	MipFilter	= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR);
	d3Fmt		= D3DFMT_UNKNOWN;

	Before = (FLOAT)LnUtil_GetUsedMemory();

	for(i=0; i<nCnt; ++i)
	{
//		hr=D3DXCreateTextureFromFileEx(
//				m_pd3dDevice
//				, sFile
//				, D3DX_DEFAULT
//				, D3DX_DEFAULT
//				, D3DX_DEFAULT
//				, 0
//				, d3Fmt
//				, D3DPOOL_MANAGED
//				, Filter
//				, MipFilter
//				, dColor
//				, &pSrcInf
//				, NULL
//				, &pTexture[i]	);


		hr=D3DXCreateTextureFromFile(m_pd3dDevice, sFile, &pTexture[i]	);

		if(FAILED(hr))
			return -1;
	}

	After = (FLOAT)LnUtil_GetUsedMemory();
	Used1 = (After - Before);
	Used1 /= nCnt;

	for(i=0; i<nCnt; ++i)
		SAFE_RELEASE(	pTexture[i]	);
	
	Before = (FLOAT)LnUtil_GetUsedMemory();

	for(i=0; i<nCnt; ++i)
	{
		hr=D3DXCreateTextureFromFileEx(
				m_pd3dDevice
				, sFile
				, D3DX_DEFAULT
				, D3DX_DEFAULT
				, 1
				, 0
				, d3Fmt
				, D3DPOOL_MANAGED
				, Filter
				, MipFilter
				, dColor
				, &pSrcInf
				, NULL
				, &pTexture[i]	);

		if(FAILED(hr))
			return -1;		
	}

	After = (FLOAT)LnUtil_GetUsedMemory();
	Used2 = (After - Before);
	Used2 /= nCnt;



	FLOAT fRatio = (Used1-Used2)*100.f/Used1;



	memset(m_szMsg, 0, sizeof(m_szMsg));
	sprintf(m_szMsg, "%s, MipMap %.f KB, Non %.f KB Ratio = %.f %% "
				, sFile
				, Used1
				, Used2
				, fRatio );


	for(i=0; i<nCnt; ++i)
		SAFE_RELEASE(	pTexture[i]	);

	return S_OK;
}



HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pD3DXFont	);

	return S_OK;
}



HRESULT CMain::Restore()
{
	m_pD3DXFont->OnResetDevice();

	return S_OK;
}



HRESULT CMain::Invalidate()
{
	m_pD3DXFont->OnLostDevice();

	return S_OK;
}



HRESULT CMain::FrameMove()
{
	return S_OK;
}


HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L
						, NULL
						, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
						, 0x00006699
						, 1.0f
						, 0L );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	RenderText();

	m_pd3dDevice->EndScene();

	return S_OK;
}







HRESULT CMain::RenderText()
{
	D3DCOLOR fontColor		= D3DCOLOR_ARGB(255,255,255,0);

	RECT rct={ 10, 10, m_d3dsdBackBuffer.Width - 20, 10+30};
	m_pD3DXFont->DrawText(NULL, m_szMsg, -1, &rct, 0, fontColor );
	
	return S_OK;
}



LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_PAINT:
		{
			break;
		}

	}

	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}


