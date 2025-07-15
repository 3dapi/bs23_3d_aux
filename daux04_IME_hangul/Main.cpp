// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	_tcscpy(m_strClassName, _T("Mck"));

	m_dwCreationWidth			= 800;
	m_dwCreationHeight			= 600;
	
	m_bStartFullscreen			= false;
	m_bShowCursorWhenFullscreen	= true;

	m_pD3DXFont		= NULL;

	m_pIme			= NULL;
}



HRESULT CMain::Init()
{
	HRESULT hr=-1;

	// Create a D3D font using D3DX
	D3DXFONT_DESC hFont =
	{
		16, 0
		, FW_BOLD, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS
		, ANTIALIASED_QUALITY, FF_DONTCARE
		, _T("Arial")
	};

	if( FAILED( hr = D3DXCreateFontIndirect( m_pd3dDevice, &hFont, &m_pD3DXFont ) ) )
		return -1;



	SetWindowText(m_hWnd, _T("한글을 입력해 보세요") );
	// Ime 생성
	m_pIme = new CLcHangul;
	m_pIme->Create();
	m_pIme->Set();

	
	return S_OK;
}



HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pD3DXFont	);

	SAFE_DELETE(	m_pIme		);

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
	static D3DXMATRIX mtI(1,0,0,0,    0,1,0,0,    0,0,1,0,    0,0,0,1);
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
	D3DCOLOR fontColor		= D3DCOLOR_ARGB(255,0,255,255);
	TCHAR szMsg[MAX_PATH]	= {0};


	TCHAR szMsg2[512] = TEXT("");

	_stprintf( szMsg, TEXT("%s %s"), m_strDeviceStats, m_strFrameStats );

	

	RECT rct={ 10, 10, m_d3dsdBackBuffer.Width - 20, 10+30};
//	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );

	
	rct.top +=60;
	rct.bottom+=60;

	if(m_pIme->GetState())
	{
		m_pIme->OutStringBeam(szMsg2);
		m_pD3DXFont->DrawText(NULL, szMsg2, -1, &rct, 0, 0xFFFFFF00);
	}
	else
	{
		m_pIme->OutString(szMsg2);
		m_pD3DXFont->DrawText(NULL, szMsg2, -1, &rct, 0, 0xFFFFFF00 );
	}
	
	return S_OK;
}



LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if(m_pIme && m_pIme->GetState())
	{
		if(m_pIme->MsgProc( hWnd, msg, wParam, lParam ))
			return 0;
	}


	if(WM_COMMAND == msg)
	{
		if( IDM_RETURN == LOWORD(wParam) )
		{
			if(m_pIme->GetState())
			{
				TCHAR szMsg[512] = TEXT("");
				m_pIme->OutString(szMsg);
				SetWindowText(hWnd, szMsg);
				m_pIme->Reset();
			}
			else
				m_pIme->Set();

			return FALSE;
		}
	}



	
	switch( msg )
	{
		case WM_PAINT:
		{
			break;
		}

	}

	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}


