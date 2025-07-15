// Interface for the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _McScene_H_
#define _McScene_H_

class CMcScene
{
protected:
	LPDIRECT3DDEVICE9	m_pDev;

	// 3D 문자열 출력 객체
	LPD3DXMESH			m_pMshStr;

	
public:
	CMcScene();
	virtual ~CMcScene();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();

	INT		Restore();
	void	Invalidate();
};

#endif