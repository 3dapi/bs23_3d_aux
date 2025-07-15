// Implementation of the CMcXmsh class.
//
//////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcXmsh::CMcXmsh()
{
	for(int i=0; i<6; ++i)
		m_pTex[i] = NULL;


	m_nVtx	=0;
	m_dVtx	=0;
	m_dFVF	=0;
	m_pVtx	= NULL;
}

CMcXmsh::~CMcXmsh()
{
	Destroy();
}


INT CMcXmsh::Create(LPDIRECT3DDEVICE9 pDev)
{
	INT		i;
	HRESULT hr = 0;

	m_pDev = pDev;


	ID3DXMesh* pMeshSrc = NULL;

	
	if(FAILED(CreateMeshData(m_pDev, &pMeshSrc)))
		return -1;


	// 1. 버텍스 인덱스를 얻어서 복사한다.
	INT	nVtxS= pMeshSrc->GetNumVertices();
	INT	nFceS= pMeshSrc->GetNumFaces();


	// Setup Vertex info
	m_nVtx	= nVtxS;
	m_dVtx	= (UINT)pMeshSrc->GetNumBytesPerVertex();
	m_dFVF	= pMeshSrc->GetFVF();
	m_pVtx	= malloc(m_nVtx * m_dVtx);



	// 버텍스 복사.
	VtxUV1* pVtxS=NULL;
	pMeshSrc->LockVertexBuffer(0, (void**)&pVtxS);
		memcpy(m_pVtx, pVtxS, sizeof(VtxUV1) * nVtxS);
	pMeshSrc->UnlockVertexBuffer();


	// 인덱스는 임시 버퍼에 복사.
	VtxIdx* pFce = new VtxIdx[nFceS];

	VtxIdx* pIdxS=NULL;
	pMeshSrc->LockIndexBuffer(0, (void**)&pIdxS);
		memcpy(pFce, pIdxS, sizeof(VtxIdx) * nFceS);
	pMeshSrc->UnlockIndexBuffer();


	

	// 2. 속성 테이블을 가져와서 Geometry 구조체에 복사한다.
	DWORD	nNumSubset=0;
	hr= pMeshSrc->GetAttributeTable(0, &nNumSubset);

	// Geometry 숫자를 설정한다.
	m_nGeo = nNumSubset;
	m_pGeo = new McGeo[m_nGeo];


	D3DXATTRIBUTERANGE* attTable = new D3DXATTRIBUTERANGE[nNumSubset];
	hr= pMeshSrc->GetAttributeTable(attTable, &nNumSubset);

	
	// Geometry 구성
	for(i=0; i<m_nGeo; ++i)
	{
		McGeo* pGeo = &m_pGeo[i];

		pGeo->nFce	= attTable[i].FaceCount;
		pGeo->nVtx	= attTable[i].VertexCount;

		INT nFaceBegin	= attTable[i].FaceStart;


		// Geometry의 인덱스 버퍼 생성
		pGeo->pIdx	= new VtxIdx[pGeo->nFce];

		// Index Buffer 복사
		VtxIdx* pIdxDst = pFce + nFaceBegin;
		memcpy(pGeo->pIdx, pIdxDst, sizeof(VtxIdx) * pGeo->nFce);
	}


	// Debug... 속성 가져오기
	//	INT	nAttS= 0;		// Attribute 숫자는 삼각형 숫자와 동일
	//	nAttS	= nFceS;
	//	DWORD*	pAttS = new DWORD[nAttS];
	//	DWORD*	pAttT = NULL;
	//	pMeshSrc->LockAttributeBuffer(0, &pAttT);
	//		memcpy(pAttS, pAttT, sizeof(DWORD) * nAttS);
	//	pMeshSrc->UnlockAttributeBuffer();
	//	delete [] pAttS;


	// 원본 ID3DXMesh, 임시 버퍼를 지운다.
	SAFE_DELETE_ARRAY( attTable	);
	SAFE_DELETE_ARRAY( pFce		);
	SAFE_RELEASE(	pMeshSrc	);

	



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


	for(i=0; i<6; ++i)
		D3DXCreateTextureFromFile( m_pDev, sTex[i], &m_pTex[i]);


	return 0;
}

void CMcXmsh::Destroy()
{
	for(int i = 0; i < 6; i++)
	{
		SAFE_RELEASE( m_pTex[i] );
	}
	
	SAFE_DELETE_ARRAY(	m_pGeo	);

	if(m_pVtx)
	{
		free(m_pVtx);
		m_pVtx = NULL;
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
	
	for(i = 0; i < m_nGeo; i++)
	{
		McGeo* pGeo = &m_pGeo[i];

		m_pDev->SetTexture( 0, m_pTex[i] );

		m_pDev->SetFVF(m_dFVF);
		m_pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST
										, 0
										, m_nVtx
										, pGeo->nFce
										, pGeo->pIdx
										, D3DFMT_INDEX16
										, m_pVtx
										, m_dVtx);
	}
	
}







INT CMcXmsh::CreateMeshData(LPDIRECT3DDEVICE9 pDev, ID3DXMesh** pOut)
{
	HRESULT hr=0;

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



	ID3DXMesh* pMeshSrc=NULL;


	hr = D3DXCreateMeshFVF(12, 24, D3DXMESH_SYSTEMMEM, VtxUV1::FVF, pDev, &pMeshSrc);
	
	if(FAILED(hr))
		return -1;
	

	VtxUV1* pVtx = NULL;
	pMeshSrc->LockVertexBuffer(0, (void**)&pVtx);
		memcpy(pVtx, pVtxS, sizeof(VtxUV1) * 24);	
	pMeshSrc->UnlockVertexBuffer();
	
	
	VtxIdx* pIdx = NULL;
	pMeshSrc->LockIndexBuffer(0, (void**)&pIdx);
		memcpy(pIdx, pIdxS, sizeof(VtxIdx) * 12);	
	pMeshSrc->UnlockIndexBuffer();
	

	DWORD* attBuf = NULL;
	pMeshSrc->LockAttributeBuffer(0, &attBuf);

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

	pMeshSrc->UnlockAttributeBuffer();


	DWORD* pAdjacencyBuf = new DWORD[12 * 3];

	pMeshSrc->GenerateAdjacency(0.f, pAdjacencyBuf);
	
	// 4. 최적화
	hr = pMeshSrc->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE
		, pAdjacencyBuf
		, 0, 0, 0);
	
	delete [] pAdjacencyBuf;


	*pOut = pMeshSrc;

	return 0;
}