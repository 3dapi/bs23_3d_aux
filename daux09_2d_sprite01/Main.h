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

	D3DXIMAGE_INFO		m_pInf0;
	LPDIRECT3DTEXTURE9	m_pTex0;

	D3DXIMAGE_INFO		m_pInf1;
	LPDIRECT3DTEXTURE9	m_pTex1;
	ILcSprite*			m_pLcSprite;

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

