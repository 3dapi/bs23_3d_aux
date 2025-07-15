// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
protected:
	ID3DXFont*		m_pD3DXFont;            // D3DX font    

	CMcInput*		m_pInput;
	CMcCamera*		m_pCam;
	CMcScene*		m_pScene;
	
public:
	CMain();
	

protected:
	virtual HRESULT Init();
	virtual HRESULT Destroy();

	virtual HRESULT FrameMove();
	virtual HRESULT Render();

	virtual HRESULT Restore();
	virtual HRESULT Invalidate();

	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

protected:
	HRESULT RenderText();
};

extern CMain*	g_pApp;
#define GMAIN	g_pApp

#endif

