// Interface for the CMcField class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCFIELD_H_
#define _MCFIELD_H_

class CMcField
{
protected:
	INT				m_iN;														// Number of tile for Width
	INT				m_iW;														// Width of tile for x;

	INT				m_iNvx;														// Vertex Number
	INT				m_iNix;														// Index Number
	INT				m_iVxS;														// Vertex Size
	DWORD			m_dFVF;


public:
	LPDIRECT3DDEVICE9	m_pDev;
	VtxNDUV1*			m_pVtx;
	VtxIdx*				m_pIdx;
	LPDIRECT3DTEXTURE9	m_pTx0;

	FLOAT				m_fAl;
	D3DXVECTOR3			m_vcLgt;
	D3DLIGHT9			m_light;
	D3DMATERIAL9		m_mtl;
	
public:
	CMcField();
	~CMcField();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();
	
protected:
	void	NormalSet();
	D3DXVECTOR3	NormalVec(int z, int x);

	void	MapLoad();
};

#endif