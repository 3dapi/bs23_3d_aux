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

	ILcTexture*		m_pTex0;
	ILcTexture*		m_pTex1;
	ILcSprite*		m_pLcSprite;

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
	void	RenderXYZ();
};

extern CMain*	g_pApp;
#define GMAIN	g_pApp

#endif

