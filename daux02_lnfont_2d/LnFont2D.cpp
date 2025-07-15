// Implementation of the CLnFont2D class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CLnFont2D::CLnFont2D()
{
	m_pDev	= NULL;
	m_pSpt	= NULL;
	m_pTxD	= NULL;

	m_hFnt	= NULL;
	m_hFntI	= NULL;

	memset(m_sStr, 0, sizeof(m_sStr));	
}

CLnFont2D::~CLnFont2D()
{
	if(m_hFntI)
	{
		DeleteObject(	m_hFntI	);
		m_hFntI	= NULL;
	}

	SAFE_RELEASE(	m_pTxD	);
	SAFE_RELEASE(	m_pDev	);
}


INT CLnFont2D::Create(LPD3DXSPRITE pSpt)
{
	m_pSpt	= pSpt;

	m_pSpt->GetDevice(&m_pDev);
	
	m_iH	= 60;
	LOGFONT logFont =
	{
		m_iH
		, 0
		, 0
		, 0
		, FW_BOLD
		, FALSE
		, FALSE
		, 0
		, 0
		, 0
		, 2
		, 1
		, VARIABLE_PITCH | FF_ROMAN
		, "Arial"
	};
	strcpy(logFont.lfFaceName, "Arial");

	m_hFntI = ::CreateFontIndirect(&logFont);

	m_hFnt	= m_hFntI;
	m_vcP.x = 0;
	m_vcP.y = 10;
	m_vcA.x = 0;
	m_vcA.y = 0;
	m_dwFrnt = RGB(  0, 255, 255);
	m_dwBack = RGB( 50,  50, 0);

	m_iThickX	= 2;
	m_iThickY	= 2;

	return 0;
}


void CLnFont2D::DrawTxt()
{
	if(NULL==m_pTxD)
		return;

	D3DXVECTOR2 pos;
	pos = m_vcP + m_vcA;
	D3DXVECTOR3	vcPos;
	vcPos.x = pos.x;
	vcPos.y = pos.y;
	vcPos.z = 0;

	m_pSpt->Begin(D3DXSPRITE_ALPHABLEND);
	m_pSpt->Draw(m_pTxD, &m_rt, NULL, &vcPos, D3DXCOLOR(1,1,1,1));
	m_pSpt->End();
}


void CLnFont2D::SetFont(HFONT hFontOut)
{
	m_hFnt	= hFontOut;

	if(NULL == m_hFnt)
		m_hFnt =m_hFntI;
}


void CLnFont2D::SetThickX(INT iThickX)
{
	m_iThickX	= iThickX;
}


void CLnFont2D::SetThickY(INT iThickY)
{
	m_iThickY	= iThickY;
}


INT	CLnFont2D::SetString(TCHAR* sStr)
{
	HDC			hDC;

	SIZE		sz;
	TEXTMETRIC	tm;

	m_iW		= 0;

	if(NULL ==sStr || 0==strlen(sStr))
	{
		SAFE_RELEASE(m_pTxD);

		m_iLen= 0;

		return 0;
	}

	if(0==strcmp(sStr, m_sStr))
	{
		return 0;
	}

	strcpy(m_sStr, sStr);


	m_iLen	= strlen(m_sStr);

	HWND hWnd = NULL;

	hDC = NULL;
	hDC = GetDC(hWnd);
	SelectObject(hDC, m_hFnt);													// Parent window의 DC를 이용해 스트링의 정확한 픽셀 사이즈를 얻어 온다.
	GetTextExtentPoint32(hDC, m_sStr, strlen(m_sStr), &sz);
	m_iW = sz.cx;
	
	GetTextMetrics(hDC, &tm);
	m_iW -= tm.tmOverhang;
	ReleaseDC(hWnd, hDC);

	return SetTexture();
}

void CLnFont2D::SetPos(D3DXVECTOR2	vcPos, DWORD dwAlign)
{
	m_vcP = vcPos;

	m_vcA.x = 0.f;
	m_vcA.y = 0.f;

	if( dwAlign & FNT_ALIGN_H_R)
		m_vcA.x = -m_iW*1.f;

	if( dwAlign & FNT_ALIGN_H_C)
		m_vcA.x = -m_iW/2.f;

	if( dwAlign & FNT_ALIGN_V_B)
		m_vcA.y = -m_iH*1.f;

	if( dwAlign & FNT_ALIGN_V_C)
		m_vcA.y = -m_iH/2.f;
}


INT	CLnFont2D::SetColor(DWORD dwFrnt,  DWORD dwBack)
{
	m_dwFrnt	= dwFrnt;
	m_dwBack	= dwBack;

	return SetTexture();
}


INT	CLnFont2D::SetColorFrnt(DWORD dwFrnt)
{
	m_dwFrnt	= dwFrnt;
	
	return SetTexture();
}

INT	CLnFont2D::SetColorBack(DWORD dwBack)
{
	m_dwBack	= dwBack;

	return SetTexture();
}


INT CLnFont2D::SetTexture()
{
	LPDIRECT3DTEXTURE9	pTxS	= NULL;		// 불투명 텍스처
	LPDIRECT3DSURFACE9	pSfTxtS	= NULL;		// 불투명 텍스처의 서피스
	LPDIRECT3DSURFACE9	pSfTxtD	= NULL;		// 출력용 텍스처 m_pTxD의 서피스
	D3DSURFACE_DESC	dsc;
	D3DLOCKED_RECT	rt;
	HRESULT			hr;
	INT				i, j;
	HDC				hDC;

	SAFE_RELEASE(	m_pTxD	);

	hr = D3DXCreateTexture(m_pDev, m_iW, INT(m_iH + m_iThickY* 4), 0, 0
					, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTxS);					// 불투명 Texture를 만든다.

	if(FAILED(hr))
		return hr;
	
	hr  = pTxS->GetSurfaceLevel(0, &pSfTxtS);
	hr  = pSfTxtS->GetDesc(&dsc);

	pSfTxtS->LockRect(&rt, 0, 0);
		DWORD*	pData = (DWORD*) rt.pBits;
		memset(pData, 0x00, sizeof(DWORD) * dsc.Width * dsc.Height);			// 배경을 검정색으로 칠한다.
	pSfTxtS->UnlockRect();


	hDC = NULL;																	// 불투명 텍스처에 DC를 얻어와 쓴다.
	pSfTxtS->GetDC(&hDC);

	if(!hDC)
	{
		pSfTxtS->ReleaseDC(hDC);
		SAFE_RELEASE(	pTxS		);
		SAFE_RELEASE(	pSfTxtS		);

		return -1;
	}

	SelectObject(hDC, m_hFnt);
	SetBkMode(hDC, TRANSPARENT);


	for(i=-m_iThickX; i<=m_iThickX; ++i)
	{
		for(j=-m_iThickY; j<=m_iThickY ; ++j)
		{
			if(i==0 && i==j)
				continue;
			
			SetTextColor(hDC, m_dwBack );
			TextOut(hDC, m_iThickX+ i, m_iThickY+j, m_sStr, m_iLen);
		}
	}

	SetTextColor(hDC, m_dwFrnt);
	TextOut(hDC, m_iThickX, m_iThickY, m_sStr, m_iLen);
	hr = pSfTxtS->ReleaseDC(hDC);

	if(FAILED(hr))
		return -1;
	


	
	
	D3DXCreateTexture(m_pDev, m_iW, INT(m_iH + m_iThickY* 4), 0, 0
					, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTxD);				// 반투명 텍스처를 만든다.

	m_pTxD->GetSurfaceLevel(0, &pSfTxtD);
	pSfTxtD->GetDesc(&dsc);

	m_rt.left = 0;																// 반투명 텍스처의 영역으로 드러우의 영역을 설정한다.
	m_rt.top = 0;
	m_rt.right = dsc.Width;
	m_rt.bottom = dsc.Height;

//	hr = D3DXLoadSurfaceFromSurface( pSfTxtD, NULL, &m_rt,
//									pSfTxtS,  NULL, &m_rt,
//									D3DX_DEFAULT, 0xFF000000);

	hr = D3DXLoadSurfaceFromSurface( pSfTxtD, NULL, NULL,						// 불투명 텍스처를 반투명으로 옮기되 불투명의 배경으로 사용한 검정색을 컬러키로 설정한다.
									pSfTxtS,  NULL, NULL,
									D3DX_DEFAULT, 0xFF000000);

	SAFE_RELEASE(	pSfTxtS		);
	SAFE_RELEASE(	pSfTxtD		);
	SAFE_RELEASE(	pTxS		);

	return 0;
}