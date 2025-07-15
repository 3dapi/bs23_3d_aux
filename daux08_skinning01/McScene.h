// Interface for the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _MCSCENE_H_
#define _MCSCENE_H_


struct VtxBlend
{
	D3DXVECTOR3	p;

	FLOAT		g[3];		// BLEND WEIGHT
	BYTE		m[4];		// MATRIX Index

	FLOAT		u,v;		// UV

	VtxBlend() : p(0,0,0), u(0), v(0)
	{
		g[0]=g[1]=g[2]=0;
		m[0]=m[1]=m[2]=m[3]=0;
	}

	enum {	FVF = (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_TEX1),	};
};


class CMcScene
{
public:
	struct VtxD
	{
		D3DXVECTOR3	p;
		DWORD		d;

		VtxD() : d(0xFFFFFFFF){}
		VtxD(FLOAT X, FLOAT Y, FLOAT Z, DWORD D) : p(X,Y,Z), d(D){}
		enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE)};
	};


protected:
	LPDIRECT3DDEVICE9	m_pDev;


	VtxBlend			m_pVtx[42];
	D3DXMATRIX			m_mtWld[256];

	LPDIRECT3DTEXTURE9	m_pTex;
	INT					m_nMaxMatrixSize;

public:
	CMcScene();
	virtual ~CMcScene();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();
	
	void	RenderXYZ();
};

#endif
