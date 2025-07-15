// Interface for the CMcScene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MCSCENE_H_
#define _MCSCENE_H_

class CMcScene
{
public:
	struct VtxD
	{
		D3DXVECTOR3	p;
		DWORD	d;

		VtxD() : d(0xFFFFFFFF){}
		VtxD(FLOAT X, FLOAT Y, FLOAT Z, DWORD D) : p(X,Y,Z), d(D){}
		enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE)};
	};


protected:
	LPDIRECT3DDEVICE9	m_pDev;
	
	CMcXmsh*	m_pXmsh;

public:
	CMcScene();
	virtual ~CMcScene();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();
	
	void	Restore();
	INT		Invalidate();

	void	RenderXYZ();
};

#endif
