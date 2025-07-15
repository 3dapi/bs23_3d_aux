// Interface for the CMcXFileIns class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _McXFileIns_H_
#define _McXFileIns_H_


class CMcXFileIns : public IMcXFile
{
protected:
	LPDIRECT3DDEVICE9		m_pDev;

	CMcXFile*				m_pOrg;		// Original X-file Instance

	ID3DXMesh*				m_pMsh;
	D3DMATERIAL9*			m_pMtl;
	LPDIRECT3DTEXTURE9*		m_pTex;
	INT						m_nMtl;

	D3DXMATRIX				m_mtWld;

public:
	CMcXFileIns();
	virtual ~CMcXFileIns();

	virtual INT		Create(LPDIRECT3DDEVICE9 pDev=NULL, char* sFileName=NULL, void* pOriginal=NULL);
	virtual	void	Destroy();

	virtual INT		FrameMove();
	virtual	void	Render();

	virtual IMcXFile* GetOrigin()					{	return m_pOrg;		}

	virtual void	SetWorldMatrix(const D3DXMATRIX* v){	m_mtWld = *v;	}
	virtual D3DXMATRIX* GetWorldMatrix()			{	return &m_mtWld;	}
};

#endif


