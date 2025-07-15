// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
	ID3DXFont*              m_pD3DXFont;            // D3DX font

	CMcSprite*				m_pSprite;
	LPDIRECT3DTEXTURE9		m_pTx0;
	LPDIRECT3DTEXTURE9		m_pTx1;


public:
	CMain();

	virtual HRESULT OneTimeSceneInit();
	virtual HRESULT FinalCleanup();

	virtual HRESULT Init();
	virtual HRESULT Destroy();

	virtual HRESULT Restore();
	virtual HRESULT Invalidate();

	virtual HRESULT FrameMove();
	virtual HRESULT Render();

	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

protected:
	HRESULT RenderText();
};


extern CMain* g_pApp;

#endif



