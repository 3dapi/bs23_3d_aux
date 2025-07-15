// Interface for the CLnFont2D class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LNFONT2D_H_
#define _LNFONT2D_H_


typedef enum tagEFntAlgn														// Font Align
{
	FNT_ALIGN_H_L	= 0x00000000,												// Horizontal Left align
	FNT_ALIGN_H_R	= 0x00000001,												// Horizontal Right align
	FNT_ALIGN_H_C	= 0x00000002,												// Horizontal center

	FNT_ALIGN_V_T	= 0x00000000,												// vertical Top
	FNT_ALIGN_V_B	= 0x00000004,												// vertical bottom
	FNT_ALIGN_V_C	= 0x00000008,												// vertical center
}EFntAlgn;

class CLnFont2D
{
protected:
	LPDIRECT3DDEVICE9	m_pDev;													// Device
	LPD3DXSPRITE		m_pSpt;													// Sprite

	LPDIRECT3DTEXTURE9	m_pTxD;													// Output Texture

	INT			m_iH;
	INT			m_iW;
	INT			m_iLen;
	TCHAR		m_sStr[512];
	RECT		m_rt;

	HFONT		m_hFnt;
	HFONT		m_hFntI;

	DWORD		m_dwFrnt;														// front color
	DWORD		m_dwBack;														// back color

	INT			m_iThickX;
	INT			m_iThickY;

	D3DXVECTOR2	m_vcP;
	D3DXVECTOR2	m_vcA;															// Align pos

public:
	CLnFont2D();
	~CLnFont2D();

	INT		Create(LPD3DXSPRITE pSpt);
	INT		SetString(	TCHAR* sStr);
	void	SetFont(HFONT hFontOut=NULL);
	void	SetThickX(INT iThickX=1);
	void	SetThickY(INT iThickY=1);

	void	SetPos(D3DXVECTOR2 vcPos=D3DXVECTOR2(0, 100), DWORD dwAlign = (FNT_ALIGN_H_L| FNT_ALIGN_V_T));

	INT		SetColor(DWORD	dwFrnt = 0x00FFFF00,  DWORD	dwBack = 0x00323200);
	INT		SetColorFrnt(DWORD dwFrnt = RGB(255, 255,  0));
	INT		SetColorBack(DWORD dwBack = RGB(  0,   0,  0));
	
	void	DrawTxt();

protected:
	INT		SetTexture();
};

#endif
