// Interface for the CMcXFile class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _McXFile_H_
#define _McXFile_H_


class CMcXFile : public IMcXFile
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

	virtual INT		Create(LPDIRECT3DDEVICE9 pDev=NULL, char* sFileName=NULL, void* pOriginal=NULL);
	virtual void	Destroy();

	virtual INT		FrameMove();
	virtual void	Render();

	virtual IMcXFile* GetOrigin()	{	return this;		}

	virtual void	SetWorldMatrix(const D3DXMATRIX*){					}
	virtual D3DXMATRIX* GetWorldMatrix()			{	return NULL;	}



public:
	ID3DXMesh*			GetMesh()		{	return m_pMsh;	}
	D3DMATERIAL9*		GetMaterial()	{	return m_pMtl;	}
	LPDIRECT3DTEXTURE9*	GetTexture()	{	return m_pTex;	}
	INT					GetNumMaterial(){	return m_nMtl;	}

};

#endif


