

#include "_StdAfx.h"


CMain::CMain()
{
	m_pLnFnt		= NULL;
}

HRESULT CMain::Init()
{
	HRESULT hr=-1;

	m_pLnFnt = new CLnFont2D;
	if(FAILED(m_pLnFnt->Create(m_pd3dSprite)))
		return -1;

	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_DELETE(	m_pLnFnt	);
	
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
	m_pLnFnt->SetPos(D3DXVECTOR2( 10, 200));
	m_pLnFnt->SetString("LnFont: ¾È³çÇÏ¼¼¿ä Hello world ");

	return S_OK;
}


HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );
	
	if(FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	m_pLnFnt->DrawTxt();

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



