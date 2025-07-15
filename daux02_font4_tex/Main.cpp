

#include "_StdAfx.h"


CMain::CMain()
{
	m_hFont	= NULL;
	m_pTex	= NULL;
	m_pSfc	= NULL;
}

HRESULT CMain::Init()
{
	HRESULT hr=-1;


	// 1. 폰트 객체 생성
	LOGFONT logf={0};
	logf.lfHeight      = 32;
	logf.lfWeight      = FW_NORMAL;
	logf.lfCharSet     = HANGUL_CHARSET;
	logf.lfOutPrecision= OUT_DEFAULT_PRECIS;
	logf.lfQuality		= ANTIALIASED_QUALITY;
	logf.lfPitchAndFamily= FF_DONTCARE;
	
	_tcscpy(logf.lfFaceName, _T("궁서"));

	m_hFont = CreateFontIndirect(&logf);


	// 2. 텍스처 생성
	hr = D3DXCreateTexture(m_pd3dDevice, 700, 400, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pTex);

	if(FAILED(hr))
		return -1;

	// 3. 서피스 가져옴
	m_pTex->GetSurfaceLevel(0, &m_pSfc);





	// 4. 텍스처의 색상을 바꾸어본다.
//	D3DLOCKED_RECT drc;
//	D3DSURFACE_DESC dsc;
//	m_pSfc->GetDesc(&dsc);
//	m_pSfc->LockRect(&drc, NULL, 0);
//
//	DWORD	i, j;
//	DWORD*	pPxl = (DWORD*)drc.pBits;
//
//	for(j=0; j<350; ++j)
//	{
//		for(i=0; i<dsc.Width; ++i)
//		{
//			pPxl[ dsc.Width*j + i] = D3DXCOLOR(0.7F, 0.7F, 0.7F, 1.F);
//		}
//	}
//
//	for(j=350; j<400; ++j)
//	{
//		for(i=0; i<dsc.Width; ++i)
//		{
//			pPxl[ dsc.Width*j + i] = D3DXCOLOR(0.4F, 0.4F, 0.4F, 1.F);
//		}
//	}
//	m_pSfc->UnlockRect();

	return S_OK;
}


HRESULT CMain::Destroy()
{
	if(m_hFont)
	{
		DeleteObject( m_hFont );
		m_hFont = NULL;
	}
	
	SAFE_RELEASE(	m_pSfc	);
	SAFE_RELEASE(	m_pTex	);
	
	return S_OK;
}


HRESULT CMain::Restore()
{
	return S_OK;
}


HRESULT CMain::Invalidate()
{
	return S_OK;
}


HRESULT CMain::FrameMove()
{
	// 텍스처 서피스의 DC에 문자열 출력
	if(m_pSfc)
	{
		HDC hDC = NULL;

		m_pSfc->GetDC(&hDC);

		if(hDC)
		{
			TCHAR sMsg[256] = "";
			sprintf( sMsg, "텍스처 DC에 문자열 출력입니다");


			HFONT hFontOld = (HFONT)SelectObject(hDC, m_hFont); 
			
			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(255, 0, 255));
			TextOut(hDC, 50, 50, sMsg, strlen(sMsg));

			SetTextColor(hDC, RGB( 0,255, 255));
			TextOut(hDC, 50, 100, sMsg, strlen(sMsg));

			SetTextColor(hDC, RGB(255,255, 0));
			TextOut(hDC, 50, 150, sMsg, strlen(sMsg));

			SelectObject(hDC, hFontOld);
			DeleteDC( hDC );

			m_pSfc->ReleaseDC(hDC);
		}
	}
	
	return S_OK;
}


HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );
	
	if(FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	RECT	rc={0,0,600, 400};
	D3DXVECTOR3 vcPos(100, 100, 0);

	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pd3dSprite->Draw(m_pTex, &rc, NULL, &vcPos, D3DXCOLOR(1,1,1,1));
	m_pd3dSprite->End();


		
	m_pd3dDevice->EndScene();
	
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



