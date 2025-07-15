// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_pD3DXFont		= NULL;
	m_pInput		= NULL;
	m_pCam			= NULL;

	m_pField		= NULL;

	m_nFogVtx		= 1;
	m_nFogMode		= D3DFOG_LINEAR;
	m_bRangeFog		= TRUE;
}

HRESULT CMain::Init()
{
	SAFE_NEWCREATE1(	m_pInput,	CMcInput, m_pd3dDevice	);
	SAFE_NEWCREATE1(	m_pCam,		CMcCamera, m_pd3dDevice	);
	SAFE_NEWCREATE1(	m_pField,	CMcField, m_pd3dDevice	);
	
	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pCam		);
	SAFE_DELETE(	m_pField	);
		
	return S_OK;
}


HRESULT CMain::Restore()
{
	D3DXFONT_DESC hFont =
	{
		16, 0
		, FW_NORMAL, 1, 0
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS
		, ANTIALIASED_QUALITY
		, FF_DONTCARE
		, "Arial"
	};

	if( FAILED( D3DXCreateFontIndirect( m_pd3dDevice, &hFont, &m_pD3DXFont ) ) )
		return -1;



	return S_OK;
}



HRESULT CMain::Invalidate()
{
	SAFE_RELEASE( m_pD3DXFont );
	
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



	SAFE_FRAMEMOVE(	m_pField	);

	if(m_pInput->GetKey(DIK_F))
		m_bRangeFog	^= 1;

	if(m_pInput->GetKey(DIK_1))
		m_nFogMode	= D3DFOG_LINEAR;

	if(m_pInput->GetKey(DIK_2))
		m_nFogMode	= D3DFOG_EXP;

	if(m_pInput->GetKey(DIK_3))
		m_nFogMode	= D3DFOG_EXP2;


	if(m_pInput->GetKey(DIK_4))
		m_nFogVtx ^= 1;

	return S_OK;
}


inline DWORD FtoDW(float& p)	{	return *((DWORD*)&p);	}


HRESULT CMain::Render()
{
	HRESULT hr=0;
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF006699, 1.0f, 0L);

	
//	m_pd3dDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
//	m_pd3dDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);




	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	DWORD	dFog	= D3DXCOLOR(.2f, 1.f, .7f, 1.0f);
	float	fStart	= 300.0f;
	float	fEnd	= 700.0f;
	float	fDens	= 0.0015f;

	m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);


	if(m_nFogVtx)
	{
		hr=m_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		hr=m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, m_nFogMode);
	}
	else
	{
		hr=m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		hr=m_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, m_nFogMode);
	}


	m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, dFog);
	m_pd3dDevice->SetRenderState(D3DRS_RANGEFOGENABLE, m_bRangeFog);
	m_pd3dDevice->SetRenderState(D3DRS_FOGSTART,	*((DWORD*)(&fStart)));
	m_pd3dDevice->SetRenderState(D3DRS_FOGEND,		FtoDW(fEnd));
	m_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY,	FtoDW(fDens));


	SAFE_RENDER(	m_pField	);


	
	RenderText();

	m_pd3dDevice->EndScene();
	
	return S_OK;
}





// Name: RenderText()
// Desc: Renders stats and help text to the scene.

HRESULT CMain::RenderText()
{
	m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pd3dDevice->SetTexture( 0, 0);

	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
	TCHAR szMsg[MAX_PATH] = TEXT("");
	RECT rct;
	ZeroMemory( &rct, sizeof(rct) );       
	

	rct.left   = 2;
	rct.right  = m_d3dsdBackBuffer.Width - 20;
	
	// Output display stats
	INT nNextLine = 580; 
	
	sprintf( szMsg, "%s %s", m_strDeviceStats, m_strFrameStats );
	nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );
	
	sprintf( szMsg, "Range: \"%s\". %s,  %s Fog, Press F, 1,2,3,4 key to Fog Change."
		,	m_bRangeFog? "TRUE": "FALSE"
		,	m_nFogVtx? "Vertex": "Pixel"
		,	m_nFogMode==D3DFOG_LINEAR? "LINEAR": (m_nFogMode==D3DFOG_EXP? "EXP" :"EXP2")
		);


	nNextLine -= 20; rct.top = nNextLine; rct.bottom = rct.top + 20;    
	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );


	
	return S_OK;
}



LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	switch( msg )
	{
	case WM_PAINT:
			break;
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}


