

#include "_StdAfx.h"


CMain::CMain()
{
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

	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_DELETE(	m_pScene	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pCam		);
	
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
	SAFE_FRAMEMOVE(	m_pInput	);


	D3DXVECTOR3 vcDelta = m_pInput->GetMouseDelta();

	if(vcDelta.z !=0.f)
		m_pCam->MoveForward(-vcDelta.z* 1.f, 1.f);

	if(m_pInput->KeyState(DIK_W))					// W
		m_pCam->MoveForward( 1.f, 1.f);

	if(m_pInput->KeyState(DIK_S))					// S
		m_pCam->MoveForward(-1.f, 1.f);


	if(m_pInput->KeyState(DIK_A))					// A
		m_pCam->MoveSideward(-1.f);
		

	if(m_pInput->KeyState(DIK_D))					// D
		m_pCam->MoveSideward(1.f);

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



