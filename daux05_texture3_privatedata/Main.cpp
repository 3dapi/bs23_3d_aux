// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"



extern GUID IID_ILcTexInfo;	// {0x85c31227, 0x3de5, 0x4f00, 0x9b, 0x3a, 0xf1, 0x1a, 0xc3, 0x8c, 0x18, 0xb5}


CMain::CMain()
{
	m_pD3DXFont		= NULL;
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


	hr = LcUtil_TextureLoad(m_pd3dDevice, &m_pTex1, "texture/image1.png", 0x00ffffff);
	hr = LcUtil_TextureLoad(m_pd3dDevice, &m_pTex2, "texture/image2.png", 0x00ffffff);

	return S_OK;
}



HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pD3DXFont	);



	HRESULT			hr = -1;
	DWORD			dSize= 0;
	DWORD			dData= 0;
	D3DXIMAGE_INFO* pData;

	hr = m_pTex1->GetPrivateData( IID_ILcTexInfo, NULL, &dSize);
	hr = m_pTex1->GetPrivateData( IID_ILcTexInfo, &dData, &dSize);
	hr = m_pTex1->FreePrivateData(IID_ILcTexInfo);

	pData = (D3DXIMAGE_INFO*)dData;
	SAFE_DELETE(	pData	);


	hr = m_pTex2->GetPrivateData( IID_ILcTexInfo, NULL, &dSize);
	hr = m_pTex2->GetPrivateData( IID_ILcTexInfo, &dData, &dSize);
	hr = m_pTex2->FreePrivateData(IID_ILcTexInfo);

	pData = (D3DXIMAGE_INFO*)dData;
	SAFE_DELETE(	pData	);


	SAFE_RELEASE(	m_pTex1		);
	SAFE_RELEASE(	m_pTex2		);

	return S_OK;
}



HRESULT CMain::Restore()
{
	m_pD3DXFont->OnResetDevice();

	// Antialias Enable
	m_pd3dDevice->SetRenderState (D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_pd3dDevice->SetRenderState (D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE);
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,  FALSE);
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

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



	HRESULT			hr = -1;
	DWORD			dSize= 0;
	DWORD			dData= 0;
	D3DXIMAGE_INFO* pData;

	hr = m_pTex1->GetPrivateData(IID_ILcTexInfo, NULL, &dSize);
	hr = m_pTex2->GetPrivateData(IID_ILcTexInfo, NULL, &dSize);


	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);	

	
	hr = m_pTex1->GetPrivateData(IID_ILcTexInfo, &dData, &dSize);
	pData = (D3DXIMAGE_INFO*)dData;
	RECT	rc1={0,0, pData->Width, pData->Height};

	m_pd3dSprite->Draw(m_pTex1, &rc1, NULL, &D3DXVECTOR3(25, 50, 0), 0Xffffffff);

	
	hr = m_pTex2->GetPrivateData(IID_ILcTexInfo, &dData, &dSize);
	pData = (D3DXIMAGE_INFO*)dData;
	RECT	rc2={0,0, pData->Width, pData->Height};
	m_pd3dSprite->Draw(m_pTex2, &rc2, NULL, &D3DXVECTOR3(325, 50, 0), 0Xffffffff);


	m_pd3dSprite->End();
	
	
//	RenderText();

	m_pd3dDevice->EndScene();

	return S_OK;
}







HRESULT CMain::RenderText()
{
	D3DCOLOR fontColor		= D3DCOLOR_ARGB(255,255,255,0);
	TCHAR szMsg[MAX_PATH]	= {0};


	TCHAR szMsg2[512] = TEXT("");

	sprintf( szMsg, "%s %s", m_strDeviceStats, m_strFrameStats );


	RECT rct={ 10, 10, m_d3dsdBackBuffer.Width - 20, 10+30};
	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );
	
	return S_OK;
}



LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_PAINT:
		{
			if( m_bLoadingApp )
			{
				HDC hDC = GetDC( hWnd );
				ReleaseDC( hWnd, hDC );
			}
			break;
		}

	}

	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}


