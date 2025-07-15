// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
protected:

	HFONT				m_hFont;
	LPDIRECT3DTEXTURE9	m_pTex;
	LPDIRECT3DSURFACE9	m_pSfc;
	
public:
	CMain();
	
	virtual HRESULT Init();
	virtual HRESULT Destroy();

	virtual HRESULT Restore();
	virtual HRESULT Invalidate();
	
	virtual HRESULT FrameMove();
	virtual HRESULT Render();
	
	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
};

extern CMain*	g_pApp;
#define GMAIN	g_pApp

#endif
