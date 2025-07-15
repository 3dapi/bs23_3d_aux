// Implementation of the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcScene::CMcScene()
{
	m_pDev		= NULL;

	m_pTex		= NULL;
	m_nMaxMatrixSize = 0;
}


CMcScene::~CMcScene()
{
	Destroy();	
}



INT CMcScene::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr=0;
	m_pDev	= pDev;

	int i=0;
	for(i=0; i<21; ++i)
	{
		m_pVtx[2*i + 0].p = D3DXVECTOR3( i-10.F, -7, 0)* 5;
		m_pVtx[2*i + 0].u = i/20.f;
		m_pVtx[2*i + 0].v = 1;

		m_pVtx[2*i + 1].p = D3DXVECTOR3( i-10.F,  7, 0)* 5;
		m_pVtx[2*i + 1].u = i/20.f;
		m_pVtx[2*i + 1].v = 0;

		m_pVtx[2*i + 0].g[0] = 1.0F;		// 비중
		m_pVtx[2*i + 0].m[0] = i;			// 행렬 인덱스

		m_pVtx[2*i + 1].g[0] = 1.0F;		// 비중
		m_pVtx[2*i + 1].m[0] = i;			// 행렬 인데스
	}


	D3DXCreateTextureFromFile(m_pDev, "texture/taegeukgi.png", &m_pTex);




	D3DCAPS9 d3dCaps;
	m_pDev->GetDeviceCaps( &d3dCaps );
	m_nMaxMatrixSize = d3dCaps.MaxVertexBlendMatrixIndex;

//	if(m_nMaxMatrixSize<21)
//		return -1;

	for(i=0; i<256; ++i)
		D3DXMatrixIdentity(&m_mtWld[i]);


	return 0;
}



void CMcScene::Destroy()
{
	SAFE_RELEASE(	m_pTex	);
}


INT	CMcScene::FrameMove()
{
	INT		i=0;

	FLOAT	fAngle = GetTickCount() * 0.4f;

	for(i=0; i<256; ++i)
	{
		float fTheta = fAngle + i * (360.f/15);
		m_mtWld[i]._41 = cosf( D3DXToRadian(fTheta) ) * 1.5f;
		m_mtWld[i]._42 = sinf( D3DXToRadian(fTheta) ) * 4.0f;
	}
	

	return 0;
}

void CMcScene::Render()
{	int i;

	if(m_nMaxMatrixSize<128)
		m_pDev->SetSoftwareVertexProcessing(TRUE);

	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	m_pDev->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );	// enables indexed vertex blending
	m_pDev->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS );	// transformation states are 0, 1, 2, and 3


	for(i=0; i<256; ++i)
		m_pDev->SetTransform( D3DTS_WORLDMATRIX(i), &m_mtWld[i] );


	m_pDev->SetTexture(0, m_pTex);
	m_pDev->SetFVF(VtxBlend::FVF);
	m_pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 40, m_pVtx, sizeof(VtxBlend));


	m_pDev->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	m_pDev->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE  );	// the transformation state is 0

	m_pDev->SetSoftwareVertexProcessing(FALSE);
	
	



	
	// 디바이스의 행렬 초기화
	static D3DXMATRIX mtIdentity(	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1);

	for(i=0; i<256; ++i)
		m_pDev->SetTransform( D3DTS_WORLDMATRIX(i), &mtIdentity);



//	RenderXYZ();
}




void CMcScene::RenderXYZ()
{
	VtxD pVtx[12];
	FLOAT fMax = 5000;

	pVtx[ 0] = VtxD(-fMax,     0,     0, 0xFFFF0000);
	pVtx[ 1] = VtxD(    0,     0,     0, 0xFFFF0000);
	pVtx[ 2] = VtxD(    0,     0,     0, 0xFFFF0000);
	pVtx[ 3] = VtxD( fMax,     0,     0, 0xFFFF0000);

	pVtx[ 4] = VtxD(    0, -fMax,     0, 0xFF00FF00);
	pVtx[ 5] = VtxD(    0,     0,     0, 0xFF00FF00);
	pVtx[ 6] = VtxD(    0,     0,     0, 0xFF00FF00);
	pVtx[ 7] = VtxD(    0,  fMax,     0, 0xFF00FF00);

	pVtx[ 8] = VtxD(    0,     0, -fMax, 0xFF0000FF);
	pVtx[ 9] = VtxD(    0,     0,     0, 0xFF0000FF);
	pVtx[10] = VtxD(    0,     0,     0, 0xFF0000FF);
	pVtx[11] = VtxD(    0,     0,  fMax, 0xFF0000FF);

	// Render Lines
	m_pDev->SetRenderState( D3DRS_LIGHTING,  FALSE);
	
	m_pDev->SetTexture(0, NULL);
	m_pDev->SetFVF(VtxD::FVF);
	m_pDev->DrawPrimitiveUP(D3DPT_LINELIST, 6, pVtx, sizeof(VtxD));
}