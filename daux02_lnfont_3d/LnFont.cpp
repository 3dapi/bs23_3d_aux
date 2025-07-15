// Implementation of the CLnFont class.
//
//////////////////////////////////////////////////////////////////////

#include "_StdAfx.h"


CLnFont::CLnFont()
{
	m_pDev	= NULL;
	m_pSpt	= NULL;

	m_pTxD	= NULL;

	m_hFnt	= NULL;
	m_hFntI	= NULL;
	m_hFntO	= NULL;

	memset(m_sStr, 0, sizeof(m_sStr));

	m_iH	= 30;
	m_hFntI = CreateFont(m_iH, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, VARIABLE_PITCH | FF_ROMAN, "궁서");
	m_hFnt	= m_hFntI;
	m_vcP.x = 0;
	m_vcP.y = 50;
	m_dwFrnt = RGB(255, 255, 0);
	m_dwBack = RGB(50, 50, 50);
	
}

CLnFont::~CLnFont()
{
	DeleteObject(m_hFnt);
	SAFE_RELEASE(	m_pTxD	);
	SAFE_RELEASE(	m_pDev	);
}


INT CLnFont::Create(LPD3DXSPRITE pSpt)
{
	m_pSpt = pSpt;
	m_pSpt->GetDevice(&m_pDev);

	return 0;
}



void	CLnFont::DrawTxt()
{
	VEC2 pos;
	pos = m_vcP + m_vcA;

	VEC3	vcPos;
	vcPos.x = pos.x;
	vcPos.y = pos.y;
	vcPos.z = 0;

	m_pSpt->Begin(D3DXSPRITE_ALPHABLEND);
	m_pSpt->Draw(m_pTxD, &m_rt, NULL, &vcPos, D3DXCOLOR(1,1,1,1));
	m_pSpt->End();


	if(!lsVtx.empty())
	{
		INT iSize = lsVtx.size();

		m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDev->SetFVF(CLnFont::VtxD::FVF);
		m_pDev->SetTexture(0,0);

		VtxD*	pBegin = &lsVtx[0];
//		m_pDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, iSize/3, lsVtx.begin(), sizeof(VtxD));
		m_pDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, iSize/3, pBegin, sizeof(VtxD));
	}
}


INT	CLnFont::SetString(TCHAR* sStr, VEC2 vcPos, HFONT hFontOut, DWORD dwFrnt, DWORD dwBack, INT	iThickX, INT	iThickY)
{
	HDC			hDC;

	SIZE		sz;
	TEXTMETRIC	tm;

	m_iW		= 0;

	strcpy(m_sStr, sStr);
	m_vcP		= vcPos;
	m_hFntO		= hFontOut;
	m_dwFrnt	= dwFrnt;
	m_dwBack	= dwBack;

	m_iThickX	= iThickX;
	m_iThickY	= iThickY;

	if(m_hFntO)
		m_hFnt = m_hFntO;

	m_iLen	= strlen(m_sStr);

	hDC = NULL;
	hDC = GetDC(NULL);
	SelectObject(hDC, m_hFnt);													// Parent window의 DC를 이용해 스트링의 정확한 픽셀 사이즈를 얻어 온다.
	GetTextExtentPoint32(hDC, m_sStr, strlen(m_sStr), &sz);
	m_iW = sz.cx;
	
	GetTextMetrics(hDC, &tm);
	m_iW -= tm.tmOverhang;
	ReleaseDC(NULL, hDC);

	return SetTexture();
}

void CLnFont::SetPos(VEC2	vcPos, DWORD dwAlign)
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


INT	CLnFont::SetColor(DWORD dwFrnt,  DWORD dwBack)
{
	m_dwFrnt	= dwFrnt;
	m_dwBack	= dwBack;

	return SetTexture();
}


INT	CLnFont::SetColorFrnt(DWORD dwFrnt)
{
	m_dwFrnt	= dwFrnt;
	
	return SetTexture();
}

INT	CLnFont::SetColorBack(DWORD dwBack)
{
	m_dwBack	= dwBack;

	return SetTexture();
}


INT CLnFont::SetTexture()
{
	PDTX			pTxS	= NULL;
	PDSF			pSfTxtS	= NULL;
	PDSF			pSfTxtD	= NULL;
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
	{
		DWORD*	pData = (DWORD*) rt.pBits;
		memset(pData, 0x00, sizeof(DWORD) * dsc.Width * dsc.Height);			// 배경을 검정색으로 칠한다.
	}
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
	

	pSfTxtS->LockRect(&rt, 0, 0);
	{
		DWORD*	pData = (DWORD*) rt.pBits;
		
		for(int j=0; j<(int)dsc.Height; ++j)
		{
			for(int i=0; i<(int)dsc.Width; ++i)
			{
				if(0x0 !=pData[dsc.Width * j + i])
				{
					VtxD	pVtx[8];
					pVtx[0] =VtxD(   i,  j-1, -10.0f, 0xff222200);
					pVtx[1] =VtxD(   i,    j, -10.0f, 0xff222200);
					pVtx[2] =VtxD( i+1,  j-1, -10.0f, 0xff222200);
					pVtx[3] =VtxD( i+1,    j, -10.0f, 0xff222200);

					pVtx[4] =VtxD(   i,  j-1, +10.0f, 0xffffff00);
					pVtx[5] =VtxD(   i,    j, +10.0f, 0xffffff00);
					pVtx[6] =VtxD( i+1,  j-1, +10.0f, 0xffffff00);
					pVtx[7] =VtxD( i+1,    j, +10.0f, 0xffffff00);

					for(int k=0; k<8; ++k)
						pVtx[k].p -=VEC3(150, 0, 0);

					if(	pData[i+0 + (j-1) * dsc.Width] &&
						pData[i-1 + (j+0) * dsc.Width] &&
						pData[i+1 + (j+0) * dsc.Width] &&
						pData[i+0 + (j+1) * dsc.Width]
						)
					{
						lsVtx.push_back(pVtx[0]);	lsVtx.push_back(pVtx[1]);	lsVtx.push_back(pVtx[2]);
						lsVtx.push_back(pVtx[3]);	lsVtx.push_back(pVtx[2]);	lsVtx.push_back(pVtx[1]);
						lsVtx.push_back(pVtx[4]);	lsVtx.push_back(pVtx[6]);	lsVtx.push_back(pVtx[5]);
						lsVtx.push_back(pVtx[7]);	lsVtx.push_back(pVtx[5]);	lsVtx.push_back(pVtx[6]);
					}
					else
					{
						lsVtx.push_back(pVtx[0]);	lsVtx.push_back(pVtx[1]);	lsVtx.push_back(pVtx[2]);
						lsVtx.push_back(pVtx[3]);	lsVtx.push_back(pVtx[2]);	lsVtx.push_back(pVtx[1]);
							
						lsVtx.push_back(pVtx[4]);	lsVtx.push_back(pVtx[6]);	lsVtx.push_back(pVtx[5]);
						lsVtx.push_back(pVtx[7]);	lsVtx.push_back(pVtx[5]);	lsVtx.push_back(pVtx[6]);

						lsVtx.push_back(pVtx[2]);	lsVtx.push_back(pVtx[3]);	lsVtx.push_back(pVtx[6]);
						lsVtx.push_back(pVtx[7]);	lsVtx.push_back(pVtx[6]);	lsVtx.push_back(pVtx[3]);

						lsVtx.push_back(pVtx[0]);	lsVtx.push_back(pVtx[4]);	lsVtx.push_back(pVtx[1]);
						lsVtx.push_back(pVtx[5]);	lsVtx.push_back(pVtx[1]);	lsVtx.push_back(pVtx[4]);

						lsVtx.push_back(pVtx[1]);	lsVtx.push_back(pVtx[5]);	lsVtx.push_back(pVtx[3]);
						lsVtx.push_back(pVtx[7]);	lsVtx.push_back(pVtx[3]);	lsVtx.push_back(pVtx[5]);

						lsVtx.push_back(pVtx[0]);	lsVtx.push_back(pVtx[2]);	lsVtx.push_back(pVtx[4]);
						lsVtx.push_back(pVtx[6]);	lsVtx.push_back(pVtx[4]);	lsVtx.push_back(pVtx[2]);
					}
				}
			}
		}
	}
	pSfTxtS->UnlockRect();
	


	pTxS->UnlockRect(0);














	
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