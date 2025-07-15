// Implementation of the CMcXFileIns class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "IMcXFile.h"
#include "McXFile.h"
#include "McXFileIns.h"


CMcXFileIns::CMcXFileIns()
{
	m_pDev		= NULL;
	m_pOrg		= NULL;
	m_pMsh		= NULL;
	m_pMtl		= NULL;
	m_pTex		= NULL;
	m_nMtl		= 0;

	D3DXMatrixIdentity(&m_mtWld);
}

CMcXFileIns::~CMcXFileIns()
{
	Destroy();
}


void CMcXFileIns::Destroy()
{
}


INT CMcXFileIns::Create(LPDIRECT3DDEVICE9 pDev, char* sFileName, void* pOriginal)
{
	HRESULT hr = 0;

	m_pDev = pDev;


	m_pOrg = (CMcXFile*)pOriginal;

	m_pMsh	= m_pOrg->GetMesh();
	m_pMtl	= m_pOrg->GetMaterial();
	m_pTex	= m_pOrg->GetTexture();
	m_nMtl	= m_pOrg->GetNumMaterial();

	return 0;
}

INT CMcXFileIns::FrameMove()
{
	return 0;
}

void CMcXFileIns::Render()
{
	int		i=0;
	
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pDev->SetTransform(D3DTS_WORLD, &m_mtWld);
	
	for(i=0; i<m_nMtl; ++i)
	{
		m_pDev->SetMaterial( &m_pMtl[i] );
		m_pDev->SetTexture( 0, m_pTex[i] );
		m_pMsh->DrawSubset( i );
	}
	
	static D3DXMATRIX mtI(1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1);
	m_pDev->SetTransform(D3DTS_WORLD, &mtI);
}

