// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _MAIN_H_
#define _MAIN_H_


#pragma once


class CMain : public CD3DApplication
{
protected:

	ID3DXFont*		m_pD3DXFont;            // D3DX font

	TCHAR			m_strMeshFilename[MAX_PATH];

	CD3DArcBall		m_ArcBall;          // mouse rotation utility
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matWorld;


	ILcMdl*			m_pDXMeshOrg;
	ILcMdl*			m_pDxMeshIns1;
	ILcMdl*			m_pDxMeshIns2;
	ILcMdl*			m_pDxMeshIns3;

public:
	CMain();

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



