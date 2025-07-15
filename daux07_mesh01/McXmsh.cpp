// Implementation of the CMcXmsh class.
//
//////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcXmsh::CMcXmsh()
{
	m_pMsh	= NULL;
	
	m_nMtl	= 6;
	
	for(int i=0; i<6; ++i)
		m_pTex[i] = NULL;
}

CMcXmsh::~CMcXmsh()
{
	Destroy();
}


INT CMcXmsh::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr = 0;

	m_pDev = pDev;



	VtxUV1 pVtxS[24];

	// front
	pVtxS[ 0] = VtxUV1(-6.F, -6.F, -6.F,	0.f, 0.f);
	pVtxS[ 1] = VtxUV1(-6.F,  6.F, -6.F,	0.f, 1.f);
	pVtxS[ 2] = VtxUV1( 6.F,  6.F, -6.F,	1.f, 1.f);
	pVtxS[ 3] = VtxUV1( 6.F, -6.F, -6.F,	1.f, 0.f);
	
	// back
	pVtxS[ 4] = VtxUV1(-6.F, -6.F, 6.F,		0.f, 0.f);
	pVtxS[ 5] = VtxUV1( 6.F, -6.F, 6.F,		0.f, 1.f);
	pVtxS[ 6] = VtxUV1( 6.F,  6.F, 6.F,		1.f, 1.f);
	pVtxS[ 7] = VtxUV1(-6.F,  6.F, 6.F,		1.f, 0.f);
	
	// top
	pVtxS[ 8] = VtxUV1(-6.F, 6.F, -6.F,		0.f, 0.f);
	pVtxS[ 9] = VtxUV1(-6.F, 6.F,  6.F,		0.f, 1.f);
	pVtxS[10] = VtxUV1( 6.F, 6.F,  6.F,		1.f, 1.f);
	pVtxS[11] = VtxUV1( 6.F, 6.F, -6.F,		1.f, 0.f);
	
	// bottom
	pVtxS[12] = VtxUV1(-6.F, -6.F, -6.F,	0.f, 0.f);
	pVtxS[13] = VtxUV1( 6.F, -6.F, -6.F,	0.f, 1.f);
	pVtxS[14] = VtxUV1( 6.F, -6.F,  6.F,	1.f, 1.f);
	pVtxS[15] = VtxUV1(-6.F, -6.F,  6.F,	1.f, 0.f);
	
	// left
	pVtxS[16] = VtxUV1(-6.F, -6.F,  6.F,	0.f, 0.f);
	pVtxS[17] = VtxUV1(-6.F,  6.F,  6.F,	0.f, 1.f);
	pVtxS[18] = VtxUV1(-6.F,  6.F, -6.F,	1.f, 1.f);
	pVtxS[19] = VtxUV1(-6.F, -6.F, -6.F,	1.f, 0.f);
	
	// right
	pVtxS[20] = VtxUV1( 6.F, -6.F, -6.F,	0.f, 0.f);
	pVtxS[21] = VtxUV1( 6.F,  6.F, -6.F,	0.f, 1.f);
	pVtxS[22] = VtxUV1( 6.F,  6.F,  6.F,	1.f, 1.f);
	pVtxS[23] = VtxUV1( 6.F, -6.F,  6.F,	1.f, 0.f);


	VtxIdx pIdxS[12];

	pIdxS[ 0] = VtxIdx(  0,  1,  2);
	pIdxS[ 1] = VtxIdx(  0,  2,  3);
	
	pIdxS[ 2] = VtxIdx(  4,  5,  6);
	pIdxS[ 3] = VtxIdx(  4,  6,  7);
	
	pIdxS[ 4] = VtxIdx(  8,  9, 10);
	pIdxS[ 5] = VtxIdx(  8, 10, 11);
	
	pIdxS[ 6] = VtxIdx( 12, 13, 14);
	pIdxS[ 7] = VtxIdx( 12, 14, 15);
	
	pIdxS[ 8] = VtxIdx( 16, 17, 18);
	pIdxS[ 9] = VtxIdx( 16, 18, 19);
	
	pIdxS[10] = VtxIdx( 20, 21, 22);
	pIdxS[11] = VtxIdx( 20, 22, 23);


	






	// 1. 메쉬 생성
	hr = D3DXCreateMeshFVF(12, 24, D3DXMESH_SYSTEMMEM, VtxUV1::FVF, m_pDev, &m_pMsh);
	
	if(FAILED(hr))
		return -1;
	

	// 2. 정점, 인덱스 복사
	VtxUV1* pVtx = NULL;
	m_pMsh->LockVertexBuffer(0, (void**)&pVtx);
		memcpy(pVtx, pVtxS, sizeof(VtxUV1) * 24);	
	m_pMsh->UnlockVertexBuffer();
	
	
	VtxIdx* pIdx = NULL;
	m_pMsh->LockIndexBuffer(0, (void**)&pIdx);
		memcpy(pIdx, pIdxS, sizeof(VtxIdx) * 12);	
	m_pMsh->UnlockIndexBuffer();
	

	// 3. 속성과 인덱스 연결
	DWORD* attBuf = NULL;
	m_pMsh->LockAttributeBuffer(0, &attBuf);

	
#if 1
	attBuf[ 0] = 0;
	attBuf[ 1] = 1;
	attBuf[ 2] = 2;
	attBuf[ 3] = 3;
	
	attBuf[ 4] = 4;
	attBuf[ 5] = 5;
	attBuf[ 6] = 0;
	attBuf[ 7] = 1;

	attBuf[ 8] = 2;
	attBuf[ 9] = 3;
	attBuf[10] = 4;
	attBuf[11] = 5;
#else
	attBuf[ 0] = 0;
	attBuf[ 1] = 0;
	attBuf[ 2] = 1;
	attBuf[ 3] = 1;
	
	attBuf[ 4] = 2;
	attBuf[ 5] = 2;
	attBuf[ 6] = 3;
	attBuf[ 7] = 3;

	attBuf[ 8] = 4;
	attBuf[ 9] = 4;
	attBuf[10] = 5;
	attBuf[11] = 5;
#endif

	m_pMsh->UnlockAttributeBuffer();

	

	DWORD* pAdjacencyBuf = new DWORD[12 * 3];

	m_pMsh->GenerateAdjacency(0.f, pAdjacencyBuf);
	
	// 4. 최적화
	hr = m_pMsh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE
		, pAdjacencyBuf
		, 0, 0, 0);
	
	delete [] pAdjacencyBuf;



	// Debugging
	// Attribute Buffer를 가져오면 다음의 GetAttributeTable()에서 nNumSubset을 가져
	// 오지 못한다.
	//	if(false)
	//	{
	//		attBuf = NULL;
	//		m_pMsh->LockAttributeBuffer(0, &attBuf);
	//		m_pMsh->UnlockAttributeBuffer();
	//	}

	// 속성 테이블의 내용을 가져와 본다.
	DWORD	nNumSubset=0;
	hr= m_pMsh->GetAttributeTable(0, &nNumSubset);

//	D3DXATTRIBUTERANGE	* attTable= new D3DXATTRIBUTERANGE[nNumSubset];
//	hr= m_pMsh->GetAttributeTable(attTable, &nNumSubset);
//
//	delete [] attTable;

	// 테이블 수로 DrawSubset() 함수 호출 횟수를 지정한다.
	m_nMtl = nNumSubset;


	

	// 출력에 필요한 텍스처 생성
	char* sTex[] =
	{
		"Texture/cellwall.png",
		"Texture/concrete.png",
		"Texture/greyBricks.png",
		"Texture/stones.png",
		"Texture/wood.png",
		"Texture/rocks.png",
	};


	for(int i=0; i<6; ++i)
		D3DXCreateTextureFromFile( m_pDev, sTex[i], &m_pTex[i]);


	return 0;
}

void CMcXmsh::Destroy()
{
	SAFE_RELEASE(	m_pMsh	);
	

	for(int i = 0; i < 6; i++)
	{
		SAFE_RELEASE( m_pTex[i] );
	}
	
	
}

INT CMcXmsh::FrameMove()
{
	return 0;
}

void CMcXmsh::Render()
{
	int		i = 0;
	
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	

	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	for(i = 0; i < m_nMtl; i++)
	{
		m_pDev->SetTexture( 0, m_pTex[i] );
		m_pMsh->DrawSubset( i );
	}
	
}

