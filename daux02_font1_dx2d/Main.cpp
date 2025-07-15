
#include "_StdAfx.h"

CMain::CMain()
{
	m_pScene		= NULL;
}



HRESULT CMain::Init()
{
	HRESULT hr=-1;
	SAFE_NEWCREATE1(	m_pScene,	CMcScene, m_pd3dDevice	);

	return S_OK;
}

HRESULT CMain::Destroy()
{
	SAFE_DELETE(	m_pScene	);

	return S_OK;
}

HRESULT CMain::Restore()
{
	m_pScene->Restore();
	return S_OK;
}

HRESULT CMain::Invalidate()
{
	m_pScene->Invalidate();

	return S_OK;
}


HRESULT CMain::FrameMove()
{

	SAFE_FRAMEMOVE(	m_pScene	);

	return S_OK;
}

HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );

	if(FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	SAFE_RENDER(	m_pScene	);

	m_pd3dDevice->EndScene();

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

