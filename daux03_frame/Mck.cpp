
#include "_StdAfx.h"

CMain*		g_pApp  = NULL;


LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return g_pApp->MsgProc( hWnd, uMsg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CMain d3dApp;
	
	g_pApp= &d3dApp;

	InitCommonControls();

	if( FAILED( g_pApp->Create( hInst ) ) )
	{
		return 0;
	}
	
	g_pApp->Run();

	return 0;
}



