// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_d3dEnumeration.AppUsesMixedVP = TRUE;

	m_pD3DXFont		= NULL;
	m_pInput		= NULL;
	m_pCam			= NULL;
	
	m_pScene		= NULL;
}


HRESULT CMain::Init()
{
	HRESULT hr=-1;

	SAFE_NEWCREATE1(	m_pInput,	CMcInput, m_pd3dDevice	);
	SAFE_NEWCREATE1(	m_pCam,		CMcCamera, m_pd3dDevice	);
	SAFE_NEWCREATE1(	m_pScene,	CMcScene, m_pd3dDevice	);


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


	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pD3DXFont	);
	SAFE_DELETE(	m_pScene	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pCam		);
	
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

	SAFE_FRAMEMOVE(	m_pScene	);

	
	return S_OK;
}



HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );
	
	if(FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	SAFE_RENDER(	m_pScene	);


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
	
	rct.top	   = rct.top + 10;
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
		{
			break;
		}
		
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}


