// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _Main_H_
#define _Main_H_


class CMain : public CD3DApplication
{
public:
	ID3DXFont*			m_pD3DXFont	;											// D3DX font
	CMcInput*			m_pInput	;
	CMcCamera*			m_pCam		;

	CMcField*			m_pField	;

	INT					m_nFogVtx	;
	INT					m_nFogMode	;
	BOOL				m_bRangeFog	;

	
public:
	CMain();

	virtual HRESULT Init();
	virtual HRESULT Destroy();

	virtual HRESULT Restore();
	virtual HRESULT Invalidate();

	virtual HRESULT Render();
	virtual HRESULT FrameMove();
	virtual	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

protected:
	HRESULT RenderText();

public:
	CMcInput*	GetInput(){	return m_pInput;	}
};


extern	CMain*		g_pApp;
#define	GMAIN		g_pApp

#endif

