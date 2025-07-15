// Implementation of the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcScene::CMcScene()
{
	m_pDev	= NULL;
	
	m_pMesh	= NULL;
	m_nMtrl	= 0;
	m_pMtrl	= NULL;
}


CMcScene::~CMcScene()
{
	Destroy();
}

INT CMcScene::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr;
	m_pDev	= pDev;
	
	
	ID3DXBuffer*	mtlBuf = 0;
	DWORD			nMtrl = 0;
	
	if( FAILED( D3DXLoadMeshFromX( "xFile/bigship1.x"
								, D3DXMESH_SYSTEMMEM
								, m_pDev
								, NULL
								, &mtlBuf
								, NULL
								, &nMtrl
								, &m_pMesh ) ) )
		return -1;



	m_nMtrl = nMtrl;

	if( (NULL != mtlBuf) && 0 <m_nMtrl)
	{
		D3DXMATERIAL* pMtrl = (D3DXMATERIAL*)mtlBuf->GetBufferPointer();

		m_pMtrl	= new D3DMATERIAL9[m_nMtrl];

		
		for(INT i=0; i<m_nMtrl; ++i)
			memcpy(&m_pMtrl[i], &pMtrl[i].MatD3D, sizeof(D3DMATERIAL9) );
	}
	
	SAFE_RELEASE(mtlBuf);
	


	hr = m_pMesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE
		, NULL, 0, 0, 0);
	
	return 0;
}


void CMcScene::Destroy()
{
	SAFE_DELETE_ARRAY(	m_pMtrl		);
	SAFE_RELEASE(	m_pMesh		);
}

INT CMcScene::FrameMove()
{
	return 0;
}

void CMcScene::Render()
{
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	m_pDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	D3DXVECTOR3 dir(-1.0f, -1.0f, .0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);

	D3DLIGHT9 light;
	memset(&light, 0, sizeof(D3DLIGHT9));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Diffuse   = col;
	light.Ambient   = col * 0.3f;
	light.Specular  = col * 1.f;
	light.Direction = dir;
	
	
	m_pDev->SetLight(0, &light);
	m_pDev->LightEnable(0, TRUE);
	m_pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	
	m_pDev->SetTexture(0, NULL);

	for(int i=0; i<m_nMtrl; ++i)
	{
		m_pDev->SetMaterial(&m_pMtrl[i]);
		m_pMesh->DrawSubset(i);
	}


	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
}



