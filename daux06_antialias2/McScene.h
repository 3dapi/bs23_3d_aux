// Interface for the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _McScene_H_
#define _McScene_H_

class CMcScene
{
protected:
	LPDIRECT3DDEVICE9	m_pDev;

	ID3DXMesh*			m_pMesh;
	INT					m_nMtrl;
	D3DMATERIAL9*		m_pMtrl;

public:
	CMcScene();
	virtual ~CMcScene();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();
};

#endif