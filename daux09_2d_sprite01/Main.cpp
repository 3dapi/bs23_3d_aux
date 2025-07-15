// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_pD3DXFont		= NULL;
	m_pInput		= NULL;
	m_pCam			= NULL;


	m_pTex0		= NULL;
	m_pTex1		= NULL;
	m_pLcSprite	= NULL;
}


HRESULT CMain::Init()
{
	HRESULT hr=-1;

	D3DXFONT_DESC hFont =
	{
		16, 0, FW_NORMAL
			, 1, 0
			, HANGUL_CHARSET
			, OUT_DEFAULT_PRECIS
			, ANTIALIASED_QUALITY
			, FF_DONTCARE
			, "Arial"
	};

	hr = D3DXCreateFontIndirect(m_pd3dDevice, &hFont, &m_pD3DXFont);

	if( FAILED(hr) )
		return -1;


	SAFE_NEWCREATE1(	m_pInput,	CMcInput, m_pd3dDevice	);
	SAFE_NEWCREATE1(	m_pCam,		CMcCamera, m_pd3dDevice	);


	LcxCreateSprite(&m_pLcSprite, m_pd3dDevice);
	
	
	
	if(FAILED(D3DXGetImageInfoFromFile("texture/animate.bmp", &m_pInf0)))
		return -1;

	D3DXCreateTextureFromFile(m_pd3dDevice, "texture/animate.bmp", &m_pTex0);

	if(FAILED(D3DXGetImageInfoFromFile("texture/Sprites.dds", &m_pInf1)))
		return -1;

	D3DXCreateTextureFromFile(m_pd3dDevice, "texture/Sprites.dds", &m_pTex1);

	


//	D3DXCreateTextureFromFileExW(pDevice, pSrcFile, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppTexture).


	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pD3DXFont	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pCam		);


	SAFE_RELEASE(	m_pTex0		);
	SAFE_RELEASE(	m_pTex1		);

	SAFE_DELETE(	m_pLcSprite	);

	return S_OK;
}


HRESULT CMain::Restore()
{
	if(m_pD3DXFont)
		m_pD3DXFont->OnResetDevice();	


	m_pd3dDevice->SetRenderState (D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_pd3dDevice->SetRenderState (D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	return S_OK;
}


HRESULT CMain::Invalidate()
{
	if(m_pD3DXFont)
		m_pD3DXFont->OnLostDevice();
	
	return S_OK;
}




HRESULT CMain::FrameMove()
{
	SAFE_FRAMEMOVE(	m_pInput	);


	D3DXVECTOR3 vcDelta = m_pInput->GetMouseDelta();

	FLOAT	fSpeed = m_fElapsedTime*10;


	if(vcDelta.z !=0.f)
		m_pCam->MoveForward(-vcDelta.z* fSpeed, 1.f);

	if(m_pInput->KeyState(DIK_W))					// W
		m_pCam->MoveForward( fSpeed, 1.f);

	if(m_pInput->KeyState(DIK_S))					// S
		m_pCam->MoveForward(-fSpeed, 1.f);


	if(m_pInput->KeyState(DIK_A))					// A
		m_pCam->MoveSideward(-fSpeed);
		

	if(m_pInput->KeyState(DIK_D))					// D
		m_pCam->MoveSideward(fSpeed);

	if(m_pInput->GetMouseSt(1))
		m_pCam->Rotate(vcDelta.x, vcDelta.y);


	SAFE_FRAMEMOVE(	m_pCam		);

	
	return S_OK;
}



HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );
	
	if(FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


//	RenderXYZ();



	{
		static RECT rcDrw= {0,0,0, 32};
		static DWORD dTimeBgn	= timeGetTime();
		DWORD dTimeEnd	= timeGetTime();


		if( (dTimeEnd -dTimeBgn)>12)
		{
			rcDrw.left +=32;

			if(rcDrw.left +32 >=960)
				rcDrw.left = 0;

			rcDrw.right =rcDrw.left +32;
			dTimeBgn = dTimeEnd;
		}

		D3DXVECTOR3 vcPos = D3DXVECTOR3( 2* cosf(D3DX_PI* dTimeBgn*0.0001f)
									, sinf(D3DX_PI* dTimeBgn*0.0003f) , 0);

		vcPos *= 100.f;
		vcPos += D3DXVECTOR3(300 , 300, 0.F);


		
		
		DWORD color = D3DXCOLOR(0, 1, 1, 0.8F);
		m_pLcSprite->Draw(m_pTex0, &rcDrw, (FLOAT*)&vcPos, color);



		D3DCOLOR fontColor        = D3DXCOLOR(1,1,1,1);
		TCHAR szMsg[MAX_PATH] = TEXT("xx");
		RECT rct;
		ZeroMemory( &rct, sizeof(rct) );       
	
		rct.top	   = (LONG)vcPos.y;
		rct.left   = (LONG)vcPos.x;
		rct.right  = rct.left+ 100;
		rct.bottom = rct.top + 20;
		sprintf( szMsg, "Lc");
		m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );
		

		
		D3DXVECTOR3 t = vcPos + D3DXVECTOR3(40, 0,0);

		m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pd3dSprite->Draw(m_pTex0, &rcDrw, NULL, &t, color);
		m_pd3dSprite->End();

		rct.top	   = (LONG)t.y;
		rct.left   = (LONG)t.x;
		rct.right  = rct.left+ 100;
		rct.bottom = rct.top + 20;
		sprintf( szMsg, "DX");
		m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );
	}

	{
		RECT rcImg={  0,  0, m_pInf1.Width, m_pInf1.Height};
		RECT rcDrw={128,  0, 256, 128};

		D3DXVECTOR3 vcPos(150, 300, 0);

		m_pLcSprite->Draw(m_pTex1, &rcDrw, (FLOAT*)&vcPos, D3DXCOLOR(1, 0, 1, 1.F));




		D3DCOLOR fontColor        = D3DXCOLOR(1,1,1,1);
		TCHAR szMsg[MAX_PATH] = TEXT("xx");
		RECT rct;
		ZeroMemory( &rct, sizeof(rct) );       
	
		rct.top	   = (LONG)vcPos.y;
		rct.left   = (LONG)vcPos.x;
		rct.right  = rct.left+ 100;
		rct.bottom = rct.top + 20;
		sprintf( szMsg, "Lc");
		m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );







		D3DXVECTOR3 t = vcPos + D3DXVECTOR3(160, 0,0);

		m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pd3dSprite->Draw(m_pTex1, &rcDrw, NULL, &t, D3DXCOLOR(1,1,1,1));
		m_pd3dSprite->End();

		rct.top	   = (LONG)t.y;
		rct.left   = (LONG)t.x;
		rct.right  = rct.left+ 100;
		rct.bottom = rct.top + 20;
		sprintf( szMsg, "DX");
		m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );
	}


	
//	RenderText();



	m_pd3dDevice->EndScene();

	return S_OK;
}


HRESULT CMain::RenderText()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pd3dDevice->SetTexture( 0, NULL);
	
	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
	TCHAR szMsg[MAX_PATH] = TEXT("");
	RECT rct;
	ZeroMemory( &rct, sizeof(rct) );       
	
	rct.top	   = rct.top + 5;
	rct.left   = 2;
	rct.right  = m_d3dsdBackBuffer.Width - 20;
	rct.bottom = rct.top + 20;

	sprintf( szMsg, "%s %s", m_strDeviceStats, m_strFrameStats);
	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );
	
	return S_OK;
}



LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_PAINT:
			break;
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}




void CMain::RenderXYZ()
{
	struct VtxD
	{
		FLOAT	p[3];
		DWORD	d;
	};


	FLOAT fMax = 10000;

	VtxD pVtx[12] =
	{
		{-fMax,     0,     0, 0xFFFF0000},
		{    0,     0,     0, 0xFFFF0000},
		{    0,     0,     0, 0xFFFF0000},
		{ fMax,     0,     0, 0xFFFF0000},

		{    0, -fMax,     0, 0xFF00FF00},
		{    0,     0,     0, 0xFF00FF00},
		{    0,     0,     0, 0xFF00FF00},
		{    0,  fMax,     0, 0xFF00FF00},

		{    0,     0, -fMax, 0xFF0000FF},
		{    0,     0,     0, 0xFF0000FF},
		{    0,     0,     0, 0xFF0000FF},
		{    0,     0,  fMax, 0xFF0000FF},
	};

	// Render Lines
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE);
	
	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 6, pVtx, sizeof(VtxD));
}