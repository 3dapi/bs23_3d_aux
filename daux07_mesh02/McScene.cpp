// Implementation of the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcScene::CMcScene()
{
	m_pDev		= NULL;
	m_pXmsh		= NULL;
}


CMcScene::~CMcScene()
{
	Destroy();	
}



INT CMcScene::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr=0;
	m_pDev	= pDev;

	SAFE_NEWCREATE1(m_pXmsh, CMcXmsh, m_pDev);

	return 0;
}


void CMcScene::Destroy()
{
	SAFE_DELETE(	m_pXmsh		);
}


INT	CMcScene::FrameMove()
{
	SAFE_FRAMEMOVE(	m_pXmsh	);

	return 0;
}

void CMcScene::Render()
{

	SAFE_RENDER(	m_pXmsh		);

//	RenderXYZ();
}


void CMcScene::RenderXYZ()
{
	VtxD pVtx[12];
	FLOAT fMax = 10000;

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