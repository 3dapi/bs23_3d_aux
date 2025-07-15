// Interface for the CLnFont class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCFONT2D_H_
#define _MCFONT2D_H_

#pragma warning(disable: 4996)
#include <vector>


typedef LPDIRECT3DTEXTURE9	PDTX;
typedef LPDIRECT3DSURFACE9	PDSF;
typedef	D3DXVECTOR2			VEC2;
typedef	D3DXVECTOR3			VEC3;


typedef enum tagEFntAlgn														// Font Align
{
	FNT_ALIGN_H_L	= 0x00000000,												// Horizontal Left align
	FNT_ALIGN_H_R	= 0x00000001,												// Horizontal Right align
	FNT_ALIGN_H_C	= 0x00000002,												// Horizontal center

	FNT_ALIGN_V_T	= 0x00000000,												// vertical Top
	FNT_ALIGN_V_B	= 0x00000004,												// vertical bottom
	FNT_ALIGN_V_C	= 0x00000008,												// vertical center
}EFntAlgn;


class CLnFont
{
public:
	struct VtxD
	{
		D3DXVECTOR3	p;
		DWORD	d;
		
		VtxD() : p(0,0,0), d(0xffffffff){}
		VtxD(D3DXVECTOR3 P, DWORD D=0xFFFFFFFF) : p(P), d(D){}
		VtxD(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF) : p(X,Y,Z), d(D){}

		enum { FVF =(D3DFVF_XYZ|D3DFVF_DIFFUSE)};
	};

	typedef std::vector<VtxD>		lsVtxD;
	typedef lsVtxD::iterator		itVtxD;

protected:
	LPDIRECT3DDEVICE9	m_pDev;
	LPD3DXSPRITE		m_pSpt;

	PDTX	m_pTxD;
	INT		m_iH;
	INT		m_iW;
	INT		m_iLen;
	TCHAR	m_sStr[512];
	RECT	m_rt;

	HFONT	m_hFnt;
	HFONT	m_hFntI;
	HFONT	m_hFntO;

	DWORD	m_dwFrnt;															// front color
	DWORD	m_dwBack;															// back color

	INT		m_iThickX;
	INT		m_iThickY;

	VEC2	m_vcP;
	VEC2	m_vcA;																// Align pos

public:
	CLnFont();
	virtual ~CLnFont();

	INT		Create(LPD3DXSPRITE pSpt);

	INT		SetString(	TCHAR* sStr,VEC2 vcPos=VEC2(0, 100), HFONT hFont=NULL, DWORD dwFrnt=0x00FFFF00,	DWORD dwBack=0x00323200, INT iThickX=0, INT iThickY=0);

	void	SetPos(VEC2	vcPos, DWORD dwAlign = (FNT_ALIGN_H_L| FNT_ALIGN_V_T));

	INT		SetColor(DWORD	dwFrnt = RGB(255, 255,  0),  DWORD	dwBack = RGB( 0,  0,    0));
	INT		SetColorFrnt(	DWORD	dwFrnt = RGB(255, 255,  0));
	INT		SetColorBack(	DWORD	dwBack = RGB( 0,  0,    0));
	
	void	DrawTxt();

	lsVtxD	lsVtx;

protected:
	INT		SetTexture();
};

typedef class CLnFont *LPLN2DFONT;

#endif
