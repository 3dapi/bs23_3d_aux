// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
protected:
	ID3DXFont*		m_pD3DXFont;            // D3DX font
	VtxD			m_pXYZ[24];				// XYZ Axis

public:
	CMcInput*			m_pInput;
	CMcCamera*			m_pCam;

protected:	
	CMcScene*		m_pScene;
	
public:
	CMain();
	
	virtual HRESULT Init();
	virtual HRESULT Destroy();

	virtual HRESULT Restore();
	virtual HRESULT Invalidate();
	
	virtual HRESULT FrameMove();
	virtual HRESULT Render();
	
	HRESULT RenderText();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

extern CMain*	g_pApp;
#define GMAIN	g_pApp

#endif
