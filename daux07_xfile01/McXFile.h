// Interface for the CMcXFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _McXFile_H_
#define _McXFile_H_


class CMcXFile
{
protected:
	LPDIRECT3DDEVICE9		m_pDev;

	char					m_sFile[MAX_PATH];

	ID3DXMesh*				m_pMsh;
	D3DMATERIAL9*			m_pMtl;
	LPDIRECT3DTEXTURE9*		m_pTex;
	INT						m_nMtl;

public:
	CMcXFile();
	virtual ~CMcXFile();

	INT		Create(LPDIRECT3DDEVICE9 pDev, char* sFileName);
	void	Destroy();

	INT		FrameMove();
	void	Render();
};

#endif


