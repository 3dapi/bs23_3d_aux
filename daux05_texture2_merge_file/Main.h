// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
protected:
	ID3DXFont*		m_pD3DXFont;            // D3DX font

	INT					m_nTx;
	LPDIRECT3DTEXTURE9	m_pTx;
	D3DXIMAGE_INFO		m_Img;
	
public:
	CMain();
	
	virtual HRESULT Init();
	virtual HRESULT Destroy();

	virtual HRESULT Restore();
	virtual HRESULT Invalidate();
	
	virtual HRESULT FrameMove();
	virtual HRESULT Render();
	
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

protected:
	HRESULT RenderText();


	void	MergeFile();
	BYTE*	GetFileData(int idx, int* iSize);
};



extern CMain*	g_pApp;


#endif
