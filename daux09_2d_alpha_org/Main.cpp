// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_dwCreationWidth           = 800;
	m_dwCreationHeight          = 600;
	strcpy(m_strClassName, TEXT( "RhwSprite" ));
	
	m_pD3DXFont                 = NULL;
	
	m_pTx0	= NULL;
	m_pTx1	= NULL;
}


HRESULT CMain::OneTimeSceneInit()
{
	SendMessage( m_hWnd, WM_PAINT, 0, 0 );
	return 0;
}



HRESULT CMain::FinalCleanup()
{
	return 0;
}



HRESULT CMain::Init()
{
	HRESULT hr=-1;

	m_pSprite = new CMcSprite;
	m_pSprite->Create(m_pd3dDevice);

	
	hr = D3DXCreateTextureFromFileEx(	m_pd3dDevice
									, "Texture/tile.png"
									, D3DX_DEFAULT
									, D3DX_DEFAULT
									, 1
									, 0
									, D3DFMT_UNKNOWN
									, D3DPOOL_MANAGED
									, D3DX_DEFAULT
									, D3DX_DEFAULT
									, 0x00FFFFFF
									, NULL
									, NULL
									, &m_pTx1);

	hr = D3DXCreateTextureFromFileEx(	m_pd3dDevice
									, "Texture/tank_gril.jpg"
									, D3DX_DEFAULT
									, D3DX_DEFAULT
									, 1
									, 0
									, D3DFMT_UNKNOWN
									, D3DPOOL_MANAGED
									, D3DX_FILTER_NONE
									, D3DX_FILTER_NONE
									, 0x00000000
									, NULL
									, NULL
									, &m_pTx0);

	return 0;
}



HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pTx0		);
	SAFE_RELEASE(	m_pTx1		);

	SAFE_DELETE(	m_pSprite	);

	return 0;
}



HRESULT CMain::Restore()
{
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
	
//	// Create a D3D font using D3DX
//	HRESULT hr;
//	HFONT hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
//		HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
//		ANTIALIASED_QUALITY, FF_DONTCARE, "Arial" );      
//	
//	if( FAILED( hr = D3DXCreateFont( m_pd3dDevice, hFont, &m_pD3DXFont ) ) )
//		return -1;
//	
//	DeleteObject(hFont);
	
	return 0;
}




HRESULT CMain::Invalidate()
{
	SAFE_RELEASE( m_pD3DXFont );
	return 0;
}


			
				
HRESULT CMain::FrameMove()
{
	return 0;
}



HRESULT CMain::Render()
{
	// Clear the viewport
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );
	
	// Begin the scene
	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;
	
	// Set up the textures
	m_pSprite->SetTexture(0, m_pTx0);
	m_pSprite->SetTexture(1, m_pTx1);

	m_pSprite->Render();


	RenderText();

	m_pd3dDevice->EndScene();
	
	return 0;
}



HRESULT CMain::RenderText()
{
	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
	TCHAR szMsg[MAX_PATH] = TEXT("");
	RECT rct;
	ZeroMemory( &rct, sizeof(rct) );       
	
//	m_pD3DXFont->Begin();
//	rct.left   = 2;
//	rct.right  = m_d3dsdBackBuffer.Width - 20;
//	
//	// Output display stats
//	INT nNextLine = 20; 
//	
//	sprintf( szMsg, "%s %s", m_strDeviceStats, m_strFrameStats );
//	nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
//	m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );
//	
//	
//	m_pD3DXFont->End();
	
	return 0;
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















