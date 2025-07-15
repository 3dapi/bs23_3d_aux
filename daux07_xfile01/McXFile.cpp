// Implementation of the CMcXFile class.
//
//////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcXFile::CMcXFile()
{
	memset(m_sFile, 0, sizeof(m_sFile));

	m_pDev		= NULL;
	m_pMsh		= NULL;
	m_pMtl		= NULL;
	m_pTex		= NULL;
	m_nMtl		= 0;
}

CMcXFile::~CMcXFile()
{
	Destroy();
}


INT CMcXFile::Create(LPDIRECT3DDEVICE9 pDev, char* sFile)
{
	HRESULT hr = 0;

	m_pDev = pDev;

	INT			i=0;
	ID3DXBuffer* AdjBuf = NULL;
	ID3DXBuffer* MtlBuf = NULL;
	DWORD		 nMtl;

	// �޽� �ε�
	hr = D3DXLoadMeshFromX(sFile, D3DXMESH_SYSTEMMEM, m_pDev
						, &AdjBuf
						, &MtlBuf
						, 0
						, &nMtl
						, &m_pMsh);
	
	if(FAILED(hr))
		return -1;


	// ������ �� ����
	m_nMtl = nMtl;

	// ���� �̸� ����
	strcpy(m_sFile, sFile);

	// �޽� ����ȭ
	DWORD dOpt  = D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT  | D3DXMESHOPT_VERTEXCACHE;
	DWORD* pAdj = (DWORD*)AdjBuf->GetBufferPointer();

	hr = m_pMsh->OptimizeInplace(dOpt, pAdj, 0, 0, 0);

	if(FAILED(hr))
	{
		SAFE_RELEASE(	AdjBuf	);
		SAFE_RELEASE(	MtlBuf	);
		SAFE_RELEASE(	m_pMsh	);
		return -1;
	}


	// ������ ��, �ؽ�ó�� ������ ���� ����
	if(0 >= m_nMtl || NULL == MtlBuf)
	{
		SAFE_RELEASE(	AdjBuf	);
		SAFE_RELEASE(	MtlBuf	);
		return 0;
	}


	// ������ �ؽ�ó ����
	m_pMtl = new D3DMATERIAL9[m_nMtl];
	m_pTex = new LPDIRECT3DTEXTURE9[m_nMtl];

	// �ؽ�ó ������ �ʱ�ȭ
	memset(m_pTex, 0, sizeof(LPDIRECT3DTEXTURE9) *m_nMtl);


	// ���� ������ ������
	D3DXMATERIAL* pMtl = (D3DXMATERIAL*)MtlBuf->GetBufferPointer();


	// ���� ����.
	for(i=0; i<m_nMtl; ++i)
		memcpy(&(m_pMtl[i]), &(pMtl[i].MatD3D), sizeof(D3DMATERIAL9));		// ��Ƽ���� ����



	char	sParentPath[MAX_PATH]={0};
	char	dir[_MAX_DIR]={0};

	_splitpath( sFile, NULL, dir, NULL, NULL);
	_makepath(sParentPath, NULL, dir, NULL, NULL);


	// �ؽ�ó ����		
	for(i=0; i<m_nMtl; ++i)
	{
		char sTexFile[MAX_PATH]={0};
		char fname[_MAX_FNAME]={0};
		char ext[_MAX_EXT]={0};


		char* sTexSrc = pMtl[i].pTextureFilename;

		if(NULL == sTexSrc || strlen(sTexSrc)<3)
			continue;


		_splitpath( sTexSrc, NULL, NULL, fname, ext);
		_makepath(sTexFile, NULL, sParentPath, fname, ext);


		IDirect3DTexture9* pTx = NULL;
		D3DXCreateTextureFromFile(m_pDev, sTexFile, &pTx);
		m_pTex[i] = pTx;

	}


	SAFE_RELEASE(	AdjBuf	);
	SAFE_RELEASE(	MtlBuf	);

	return 0;
}

void CMcXFile::Destroy()
{
	for(int i=0; i<m_nMtl; ++i)
	{
		SAFE_RELEASE( m_pTex[i] );
	}


	SAFE_DELETE_ARRAY(	m_pTex	);
	SAFE_DELETE_ARRAY(	m_pMtl	);

	SAFE_RELEASE(	m_pMsh	);
}

INT CMcXFile::FrameMove()
{
	return 0;
}

void CMcXFile::Render()
{
	int		i=0;
	
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	
	for(i=0; i<m_nMtl; ++i)
	{
		m_pDev->SetMaterial( &m_pMtl[i] );
		m_pDev->SetTexture( 0, m_pTex[i] );
		m_pMsh->DrawSubset( i );
	}
	
}

