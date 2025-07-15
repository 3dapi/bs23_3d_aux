//
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "resource.h"
#include "Converter.h"


extern INT		Init();
extern void		Destroy();
extern INT		Render();
extern LRESULT	MsgProc(HWND, UINT, WPARAM, LPARAM);


#define SAFE_RELEASE(p)	{ if(p){	(p)->Release(); p = NULL; } }


HINSTANCE			m_hInst		= NULL;
HWND				m_hWnd		= NULL;
LPDIRECT3D9			m_pD3D		= NULL;		// D3D
LPDIRECT3DDEVICE9	m_pd3dDevice= NULL;		// Device
LPD3DXSPRITE		m_pd3dSprite= NULL;		// Sprite


INT		Create(HINSTANCE hInst);
INT		Run();
void	Cleanup();
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );




INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	if(FAILED(Create(hInst)))
		return -1;
	
	return Run();
}




INT Create( HINSTANCE hInst)
{
	m_hInst = hInst;

	WNDCLASS wc =
	{
		CS_CLASSDC
			, WndProc
			, 0L
			, 0L
			, m_hInst
			, NULL
			, LoadCursor(NULL,IDC_ARROW)
			, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
			, NULL
			, "Convert To Targa"
	};

	// Register the window class
	if( ! RegisterClass( &wc ))
		return -1;

	DWORD	dStyle = WS_OVERLAPPEDWINDOW| WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
	RECT rc ={0, 0, 800, 600};
	AdjustWindowRect( &rc, dStyle, FALSE );

	m_hWnd = CreateWindow( "Convert To Targa"
						, "Convert To Targa"
						, dStyle
						, 30
						, 30
						, (rc.right - rc.left)
						, (rc.bottom - rc.top)
						, GetDesktopWindow()
						, LoadMenu(m_hInst, MAKEINTRESOURCE(IDR_MENU1))
						, m_hInst
						, NULL );

	if(!m_hWnd)
		return -1;


	// D3D생성
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return -1;

	// 디바이스를 생성하기 위해서는 프리젠트 파라메터 구조체가 필요
	// 전부 0으로 셋팅한다음 일부만 변수세팅
	D3DPRESENT_PARAMETERS d3dpp={0};

	d3dpp.Windowed				= TRUE;
	d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil= TRUE;
	d3dpp.AutoDepthStencilFormat= D3DFMT_D16;
	d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;


	// D3DADAPTER_DEFAULT: 대부분의 그래픽카드는 모노 듀얼일 경우 이부분을 수정
	// D3DDEVTYPE_HAL : 하드웨어 가속(가장 큰 속도)을 받을 것인가.. 하드웨어 지
	// 원이 없을 경우 D3D는 소프트웨어로 이를 대체 할 수 있다.
	if( FAILED( m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT
									,	D3DDEVTYPE_HAL
									,	m_hWnd
									,	D3DCREATE_SOFTWARE_VERTEXPROCESSING
									,	&d3dpp
									,	&m_pd3dDevice ) ) )
	{
		m_pD3D->Release();
		return -1;
	}

	D3DXCreateSprite(m_pd3dDevice, &m_pd3dSprite);

	::ShowWindow( m_hWnd, SW_SHOWDEFAULT );
	::UpdateWindow( m_hWnd );


	if(FAILED(Init()))
		return -1;


	return 0;
}


void Cleanup()
{
	Destroy();

	SAFE_RELEASE(	m_pd3dSprite	);
	SAFE_RELEASE(	m_pd3dDevice	);
	SAFE_RELEASE(	m_pD3D			);
}


LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( MsgProc(hWnd, msg, wParam, lParam))
		return 0;

	switch( msg )
	{
		case WM_PAINT:
			Render();
			break;

		case WM_CLOSE:
		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}



INT Run()
{
	// Enter the message loop
	MSG msg={0};
	while( msg.message!=WM_QUIT )
	{
		if( GetMessage( &msg, NULL, 0U, 0U) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
//		else
//		{
//			if(FAILED(Render()))
//				SendMessage(m_hWnd, WM_QUIT,0,0);
//		}
	}


	return 0;
}


