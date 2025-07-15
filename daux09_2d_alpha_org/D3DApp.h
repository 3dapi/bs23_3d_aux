//
// Desc: Application class for the Direct3D samples framework library.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _D3DAPP_H_
#define _D3DAPP_H_




// A base class for creating sample D3D9 applications. To create a simple
// Direct3D application, simply derive this class into a class (such as
// class CMyD3DApplication) and override the following functions, as needed:
//
//  OneTimeSceneInit()	- To initialize app data (alloc mem, etc.)
//  FinalCleanup()		- To cleanup app data (for exitting the app)
//  Init()				- To initialize the 3D scene objects
//  Destroy()			- To cleanup the 3D scene objects
//  FrameMove()			- To animate the scene
//  Render()			- To render the scene
//  MsgProc()			- To handle Windows messages


class CD3DApplication
{
protected:
	CD3DEnumeration			m_d3dEnumeration			;
	CD3DSettings			m_d3dSettings				;

	// Internal variables for the state of the app
	BOOL					m_bWindowed					;
	BOOL					m_bActive					;
	BOOL					m_bDeviceLost				;
	BOOL					m_bMinimized				;
	BOOL					m_bMaximized				;
	BOOL					m_bIgnoreSizeChange			;
	BOOL					m_bDeviceObjectsInited		;
	BOOL					m_bDeviceObjectsRestored	;
	BOOL					m_bLoadingApp				;						// TRUE, if the app is loading

	// Internal variables used for timing
	BOOL					m_bFrameMoving				;
	BOOL					m_bSingleStep				;

	// Internal error handling function
	HRESULT			DisplayErrorMsg( HRESULT hr, DWORD dwType );

	// Internal functions to manage and render the 3D scene
	static BOOL		ConfirmDeviceHelper( D3DCAPS9* pCaps, INT, void* );
	HRESULT			ConfirmDevice(D3DCAPS9*,DWORD,D3DFORMAT,D3DFORMAT);

	void			BuildPresentParamsFromSettings();
	BOOL			FindBestWindowedMode( BOOL bRequireHAL, BOOL bRequireREF );
	BOOL			FindBestFullscreenMode( BOOL bRequireHAL, BOOL bRequireREF );
	HRESULT			ChooseInitialD3DSettings();
	HRESULT			Initialize3DEnvironment();
	HRESULT			HandlePossibleSizeChange();
	HRESULT			Reset3DEnvironment();
	HRESULT			ToggleFullscreen();
	HRESULT			ForceWindowed();
	HRESULT			UserSelectNewDevice();
	void			Cleanup3DEnvironment();
	HRESULT			Render3DEnvironment();
	virtual HRESULT	AdjustWindowForChange();
	virtual void	UpdateStats();

protected:
	// Main objects used for creating and rendering the 3D scene
	HINSTANCE				m_hInst						;
	HWND					m_hWnd						;						// The main app window
	HWND					m_hWndFocus					;						// The D3D focus window (usually same as m_hWnd)
	HMENU					m_hMenu						;						// App menu bar (stored here when fullscreen)

	D3DPRESENT_PARAMETERS	m_d3dpp						;						// Parameters for CreateDevice/Reset
	LPDIRECT3D9				m_pD3D						;						// The main D3D object
	LPDIRECT3DDEVICE9		m_pd3dDevice				;						// The D3D rendering device
	LPD3DXSPRITE			m_pd3dSprite				;						// 2D Sprite
	LPDIRECT3DSURFACE9		m_pd3dBackBuffer			;

	D3DCAPS9				m_d3dCaps					;						// Caps for the device
	D3DSURFACE_DESC			m_d3dsdBackBuffer			;						// Surface desc of the backbuffer
	DWORD					m_dwCreateFlags				;						// Indicate sw or hw vertex processing
	DWORD					m_dwWindowStyle				;						// Saved window style for mode switches
	RECT					m_rcWindowBounds			;						// Saved window bounds for mode switches
	RECT					m_rcWindowClient			;						// Saved client area size for mode switches

	// Variables for timing
	FLOAT					m_fTime						;						// Current time in seconds
	FLOAT					m_fElapsedTime				;						// Time elapsed since last frame
	FLOAT					m_fFPS						;						// Instanteous frame rate
	TCHAR					m_strDeviceStats[90]		;						// String to hold D3D device stats
	TCHAR					m_strFrameStats[90]			;						// String to hold frame stats

	// Overridable variables for the app
	TCHAR					m_strClassName[256]			;						// Class Name for the app's window
	DWORD					m_dwCreationWidth			;						// Width used to create window
	DWORD					m_dwCreationHeight			;						// Height used to create window
	BOOL					m_bShowCursorWhenFullscreen	;						// Whether to show cursor when fullscreen
	BOOL					m_bClipCursorWhenFullscreen	;						// Whether to limit cursor pos when fullscreen
	BOOL					m_bStartFullscreen			;						// Whether to start up the app in fullscreen mode

	// Overridable functions for the 3D scene created by the app
	virtual HRESULT OneTimeSceneInit()		{ return 0; }
	virtual HRESULT FinalCleanup()			{ return 0; }

	virtual HRESULT Init()					{ return 0; }
	virtual HRESULT Destroy()				{ return 0; }

	virtual HRESULT Restore()				{ return 0; }
	virtual HRESULT Invalidate()			{ return 0; }

	virtual HRESULT FrameMove()				{ return 0; }
	virtual HRESULT Render()				{ return 0; }


public:
	// Functions to create, run, pause, and clean up the application
	virtual HRESULT			Create( HINSTANCE);
	virtual INT				Run();

	virtual LRESULT			MsgProc(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT WINAPI	WndProc(HWND, UINT, WPARAM, LPARAM);

	virtual void		Pause(BOOL);

	// Internal constructor
	CD3DApplication();
	virtual ~CD3DApplication(){}


};




#endif



