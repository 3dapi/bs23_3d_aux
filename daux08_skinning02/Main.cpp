// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	strcpy(m_strClassName, TEXT( "LcXSkinSrc" ));
	m_d3dEnumeration.AppUsesDepthBuffer = TRUE;
	m_d3dEnumeration.AppUsesMixedVP		= TRUE;
	
	m_dwCreationWidth			= 800;
	m_dwCreationHeight			= 600;
	
	m_bStartFullscreen			= false;
	m_bShowCursorWhenFullscreen	= true;


	memset(m_strMeshFilename, 0, sizeof(m_strMeshFilename));

	strcpy(m_strMeshFilename, "xfile/ggrg/ggrg_man_01.X");
//	strcpy(m_strMeshFilename, "xfile/tiny/tiny_4anim.x");
	
	m_pDXMeshOrg	= NULL;
	m_pDxMeshIns1	= NULL;
	m_pDxMeshIns2	= NULL;
	m_pDxMeshIns3	= NULL;
}



HRESULT CMain::Init()
{
	// TODO: create device objects
	HRESULT hr=-1;
	
	
	D3DXFONT_DESC hFont =
	{
		16, 0
			, FW_NORMAL
			, 1					// Mip Level
			, FALSE				// Italic
			, HANGUL_CHARSET	// Charset
			, OUT_DEFAULT_PRECIS// Output Precision
			, ANTIALIASED_QUALITY// Qulity
			, FF_DONTCARE		// Pitch And Family
			, "Arial Black"			// FaceName
	};
	
	if( FAILED( hr = D3DXCreateFontIndirect( m_pd3dDevice, &hFont, &m_pD3DXFont ) ) )
		return -1;
	

	if(FAILED(LcMdl_Create(NULL, &m_pDXMeshOrg, m_pd3dDevice, m_strMeshFilename)))
		return -1;


	
	if(m_pDxMeshIns1)
		delete m_pDxMeshIns1;

	if(m_pDxMeshIns2)
		delete m_pDxMeshIns2;

	if(m_pDxMeshIns3)
		delete m_pDxMeshIns3;



	if(FAILED(LcMdl_Create(NULL, &m_pDxMeshIns1, m_pd3dDevice, NULL, m_pDXMeshOrg)))
		return -1;


	if(FAILED(LcMdl_Create(NULL, &m_pDxMeshIns2, m_pd3dDevice, NULL, m_pDXMeshOrg)))
		return -1;


	if(FAILED(LcMdl_Create(NULL, &m_pDxMeshIns3, m_pd3dDevice, NULL, m_pDXMeshOrg)))
		return -1;

	
	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pD3DXFont		);
	
	SAFE_DELETE(	m_pDXMeshOrg	);
	SAFE_DELETE(	m_pDxMeshIns1	);
	SAFE_DELETE(	m_pDxMeshIns2	);
	SAFE_DELETE(	m_pDxMeshIns3	);
	
	return S_OK;
}



HRESULT CMain::Restore()
{
	HRESULT hr = -1;
	
	// Antialias Enable
	m_pd3dDevice->SetRenderState (D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_pd3dDevice->SetRenderState (D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	
	m_pD3DXFont->OnResetDevice();

	


	
	// Setup render state
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,         TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,     TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,         D3DCULL_CCW );
	m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,          0x33333333 );
	m_pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	
	
	FLOAT fObjectRadius = 0;
	
	m_pDXMeshOrg->GetAttrib("Radius", &fObjectRadius);


	// Setup the arcball parameters
	m_ArcBall.SetWindow( m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height, 0.85f );
	m_ArcBall.SetTranslationRadius( fObjectRadius );

	return S_OK;
}



HRESULT CMain::Invalidate()
{
	m_pD3DXFont->OnLostDevice();

	return S_OK;
}



HRESULT CMain::FrameMove()
{
	// Setup world matrix
	D3DXVECTOR3		vObjectCenter(0,0,0);
	FLOAT			fObjectRadius = 0;

	m_pDXMeshOrg->GetAttrib("Center", &vObjectCenter);
	m_pDXMeshOrg->GetAttrib("Radius", &fObjectRadius);




	D3DXMatrixTranslation( &m_matWorld, -vObjectCenter.x, -vObjectCenter.y, -vObjectCenter.z );
	
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_ArcBall.GetRotationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_ArcBall.GetTranslationMatrix() );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
	
	D3DXVECTOR3 vEye( 0, 0, -2*fObjectRadius );
	D3DXVECTOR3 vAt( 0, 0, 0 );
	D3DXVECTOR3 vUp( 0, 1, 0 );
	D3DXMatrixLookAtLH( &m_matView, &vEye, &vAt, &vUp);
	
	m_pd3dDevice->SetTransform( D3DTS_VIEW,  &m_matView );


	// Setup the projection matrix
	D3DXMATRIX	matProj;
	FLOAT		fAspect = (FLOAT)m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;
	
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, fObjectRadius/64.0f, fObjectRadius*200.0f );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	return S_OK;
}


HRESULT CMain::Render()
{
	// Clear the viewport
	m_pd3dDevice->Clear( 0L
		, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, 0x00006699
		, 1.0f
		, 0L );
	
	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;
	
	

	// Setup Lighting
	D3DLIGHT9 Lgt0;
	D3DLIGHT9 Lgt1;
	D3DLIGHT9 Lgt2;

	D3DXVECTOR3 vcLgt1( 0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 vcLgt2(-1.0f, 0.0f, -1.0f);
	D3DXVECTOR3 vcLgt3(-1.0f, 0.0f, -1.0f);

	D3DXVec3Normalize( &vcLgt1, &vcLgt1 );
	D3DXVec3Normalize( &vcLgt2, &vcLgt2 );

	memset(&Lgt0, 0, sizeof(D3DLIGHT9) );
	memset(&Lgt1, 0, sizeof(D3DLIGHT9) );
	memset(&Lgt2, 0, sizeof(D3DLIGHT9) );

	Lgt0.Type		= D3DLIGHT_DIRECTIONAL;
	Lgt0.Ambient	= D3DXCOLOR(1,1,1,1)*0.3f;
	Lgt0.Diffuse	= D3DXCOLOR(1,1,1,1);
	Lgt0.Specular	= D3DXCOLOR(1,1,1,1);
	Lgt0.Direction	= vcLgt1;

	Lgt1.Type		= D3DLIGHT_DIRECTIONAL;
	Lgt1.Ambient	= D3DXCOLOR(1,1,1,1)*0.2f;
	Lgt1.Diffuse	= D3DXCOLOR(1,1,1,1)*0.8f;
	Lgt1.Specular	= D3DXCOLOR(1,1,1,1)*0.8f;
	Lgt1.Direction	= vcLgt2;


	Lgt2.Type		= D3DLIGHT_DIRECTIONAL;
	Lgt2.Ambient	= D3DXCOLOR(1,1,1,1)*0.2f;
	Lgt2.Diffuse	= D3DXCOLOR(1,1,1,1)*.8f;
	Lgt2.Specular	= D3DXCOLOR(1,1,1,1)*.8f;
	Lgt2.Direction	= vcLgt3;


	m_pd3dDevice->SetLight(0, &Lgt0 );
	m_pd3dDevice->SetLight(1, &Lgt1 );
	m_pd3dDevice->SetLight(2, &Lgt2 );
	m_pd3dDevice->LightEnable(0, TRUE );
	m_pd3dDevice->LightEnable(1, TRUE );
	m_pd3dDevice->LightEnable(2, TRUE );
	


	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 0X80);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);



	FLOAT	fObjectRadius = 0;
	m_pDXMeshOrg->GetAttrib("Radius", &fObjectRadius);


	// Setup Animation
	if(::GetAsyncKeyState(VK_F2)&0X8000)
	{
		if(GetAsyncKeyState('1')&0X8000)	{	INT nAni = 1;	m_pDxMeshIns1->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('2')&0X8000)	{	INT nAni = 2;	m_pDxMeshIns1->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('3')&0X8000)	{	INT nAni = 3;	m_pDxMeshIns1->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('0')&0X8000)	{	INT nAni = 0;	m_pDxMeshIns1->SetAttrib("Animation", &nAni);	}
	}

	if(::GetAsyncKeyState(VK_F3)&0X8000)
	{
		if(GetAsyncKeyState('1')&0X8000)	{	INT nAni = 1;	m_pDxMeshIns2->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('2')&0X8000)	{	INT nAni = 2;	m_pDxMeshIns2->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('3')&0X8000)	{	INT nAni = 3;	m_pDxMeshIns2->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('0')&0X8000)	{	INT nAni = 0;	m_pDxMeshIns2->SetAttrib("Animation", &nAni);	}
	}

	if(::GetAsyncKeyState(VK_F4)&0X8000)
	{
		if(GetAsyncKeyState('1')&0X8000)	{	INT nAni = 1;	m_pDxMeshIns3->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('2')&0X8000)	{	INT nAni = 2;	m_pDxMeshIns3->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('3')&0X8000)	{	INT nAni = 3;	m_pDxMeshIns3->SetAttrib("Animation", &nAni);	}
		if(GetAsyncKeyState('0')&0X8000)	{	INT nAni = 0;	m_pDxMeshIns3->SetAttrib("Animation", &nAni);	}
	}



	m_matWorld._41 = -fObjectRadius*1.4f;
	m_pDxMeshIns1->SetAttrib("World Matrix", &m_matWorld);
	m_pDxMeshIns1->SetAttrib("Elapsed Time", &m_fElapsedTime);
	m_pDxMeshIns1->Render();


	m_matWorld._41 = 0;
	m_pDxMeshIns2->SetAttrib("World Matrix", &m_matWorld);
	m_pDxMeshIns2->SetAttrib("Elapsed Time", &m_fElapsedTime);
	m_pDxMeshIns2->Render();


	m_matWorld._41 = fObjectRadius*1.4f;
	m_pDxMeshIns3->SetAttrib("World Matrix", &m_matWorld);
	m_pDxMeshIns3->SetAttrib("Elapsed Time", &m_fElapsedTime);
	m_pDxMeshIns3->Render();


	m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	
	
	static D3DXMATRIX mtIdentity(	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &mtIdentity);
	
	
//	RenderText();
	m_pd3dDevice->EndScene();
	
	return S_OK;
}



HRESULT CMain::RenderText()
{
	D3DCOLOR fontColor		= D3DCOLOR_ARGB(255,255,255,0);
	TCHAR szMsg[MAX_PATH]	= {0};
	
	sprintf( szMsg, "%s %s", m_strDeviceStats, m_strFrameStats );
	
	
	RECT rct={ 5, 5, m_d3dsdBackBuffer.Width - 20, 10+30};
	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );
	
	
	
	INT nMethod = 0;
	m_pDXMeshOrg->GetAttrib("Method", &nMethod);


	
	TCHAR* pstrType;
	if( 0 == nMethod)
		pstrType = TEXT("No skinning");
	else
		pstrType = TEXT("Using non-indexed skinning");
	
	
	RECT rc2={ 5, m_d3dsdBackBuffer.Height - 40, m_d3dsdBackBuffer.Width - 20, m_d3dsdBackBuffer.Height};
	m_pD3DXFont->DrawText(NULL, pstrType, -1, &rc2, 0, D3DXCOLOR(1,0,1,1));
	
	
	
	return S_OK;
}



LRESULT CMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_PAINT:
		{
			break;
		}
	}
	
	
	
	
	// Pass mouse messages to the ArcBall so it can build internal matrices
	m_ArcBall.HandleMessages( hWnd, uMsg, wParam, lParam );
	
	// Trap context menu
	if( WM_CONTEXTMENU == uMsg )
		return 0;
	
	// Handle key presses
	if( WM_KEYDOWN == uMsg )
	{
		if( 'W' == (int)wParam )
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		
		if( 'S' == (int)wParam )
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}
	else if( uMsg == WM_COMMAND )
	{
		// Handle the open file command
		if( LOWORD(wParam) == IDM_OPENFILE )
		{
			char sCurrentDir[MAX_PATH];
			::GetCurrentDirectory( MAX_PATH, sCurrentDir);
			TCHAR g_strFilename[MAX_PATH]   = _T("");

			TCHAR strMesh[MAX_PATH]   = _T("");
			
			// Display the OpenFileName dialog. Then, try to load the specified file
			OPENFILENAME ofn =
			{
				sizeof(OPENFILENAME)
					, NULL
					, NULL
					, _T(".X Files (.x)\0*.x\0\0")
					, NULL
					, 0
					, 1
					, strMesh
					, MAX_PATH
					, g_strFilename
					, MAX_PATH
					, NULL
					, _T("Open Mesh File")
					, OFN_FILEMUSTEXIST
					, 0
					, 1
					, NULL
					, 0
					, NULL
					, NULL
			};
				
			if( TRUE == GetOpenFileName( &ofn ) )
			{
				strcpy(m_strMeshFilename, strMesh);
				SetCurrentDirectory( sCurrentDir );
				
				// Destroy and recreate everything
				Invalidate();
				Destroy();
				Init();
				Restore();
			}
		}
	}
	
	return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}




