// Interface for the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _McScene_H_
#define _McScene_H_

class CMcScene
{
protected:
	LPDIRECT3DDEVICE9		m_pDev;

	ID3DXFont*				m_pDXFont1;
	ID3DXFont*				m_pDXFont2;

	std::string				cs_name1;
	std::string				cs_name2;

	
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