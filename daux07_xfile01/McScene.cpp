// Implementation of the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcScene::CMcScene()
{
	m_pDev		= NULL;
	m_pXFile	= NULL;
}


CMcScene::~CMcScene()
{
	Destroy();	
}



INT CMcScene::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr=0;
	m_pDev	= pDev;

	SAFE_NEWCREATE2(m_pXFile, CMcXFile, m_pDev, "xfile/dwarf/dwarf.x");
//	SAFE_NEWCREATE2(m_pXFile, CMcXFile, m_pDev, "xfile/wall_with_pillars/wall_with_pillars.x");

	return 0;
}


void CMcScene::Destroy()
{
	SAFE_DELETE(	m_pXFile		);
}


INT	CMcScene::FrameMove()
{
	SAFE_FRAMEMOVE(	m_pXFile	);

	return 0;
}

void CMcScene::Render()
{
	D3DXVECTOR3 dir1(-1.0f, -1.0f, 1.0f);
	D3DXVECTOR3 dir2( 1.0f, -1.0f, 1.0f);
	D3DXCOLOR col1(0.7f, 1.f, 1.f, 1.0f);
	D3DXCOLOR col2(0.6f, 0.2f, 0.6f, 1.0f);

	D3DLIGHT9 Lgt1;
	D3DLIGHT9 Lgt2;

	memset(&Lgt1, 0, sizeof(Lgt1));
	Lgt1.Type      = D3DLIGHT_DIRECTIONAL;
	Lgt1.Diffuse   = col1;
	Lgt1.Ambient   = col1 * .2f;
	Lgt1.Specular  = col1;
	Lgt1.Direction = dir1;


	memset(&Lgt2, 0, sizeof(Lgt2));
	Lgt2.Type      = D3DLIGHT_DIRECTIONAL;
	Lgt2.Diffuse   = col2;
	Lgt2.Ambient   = col2 * .2f;
	Lgt2.Specular  = col2;
	Lgt2.Direction = dir2;
	
	m_pDev->SetLight(0, &Lgt1);
	m_pDev->SetLight(1, &Lgt2);
	m_pDev->LightEnable(0, TRUE);
	m_pDev->LightEnable(1, TRUE);


	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);


	SAFE_RENDER(	m_pXFile	);


	m_pDev->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1,1,1,1));
	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

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