// Implementation of the CMcScene class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"



int LcStr_AnsiToUnicode(WCHAR* sDst, const char* sSrc, INT iLen )
{
	if( sDst==NULL || sSrc==NULL || iLen < 1 )
		return -1;
	
	INT nResult = MultiByteToWideChar( CP_ACP, 0, sSrc, -1, sDst, iLen );

	sDst[iLen-1] = 0;
	
	if( 0 == nResult)
		return -1;

	return 0;
}


CMcScene::CMcScene()
{
	m_pDev		= NULL;
	m_pLnFnt	= NULL;
}


CMcScene::~CMcScene()
{
	Destroy();
}

void CMcScene::Destroy()
{
	SAFE_DELETE(	m_pLnFnt	);
}


INT CMcScene::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr=0;
	m_pDev	= pDev;


	LPD3DXSPRITE pSpt = GMAIN->GetSprite();

	m_pLnFnt = new CLnFont;
	m_pLnFnt->Create(pSpt);
	m_pLnFnt->SetString("한글 2D, 3D Font 연습");
	m_pLnFnt->SetPos( D3DXVECTOR2(40, 140));

	return 0;
}


INT CMcScene::Restore()
{
	return 0;
}

void CMcScene::Invalidate()
{
}


INT CMcScene::FrameMove()
{
	return 0;
}

void CMcScene::Render()
{
	D3DXMATRIX mtWld;
	D3DXMATRIX mtS, mtR, mtT1, mtT2;
	float fRot = D3DXToRadian(GetTickCount() * 0.03f);

	D3DXMatrixIdentity(&mtS);
	D3DXMatrixIdentity(&mtR);
	D3DXMatrixIdentity(&mtT1);
	D3DXMatrixIdentity(&mtT2);

	
	D3DXMatrixScaling(&mtS, 0.4f, 0.4f, 0.4f);
	D3DXMatrixRotationX(&mtR, fRot);
	D3DXMatrixTranslation(&mtT1, 40, 0, 0);
	D3DXMatrixTranslation(&mtT2,  0, 5, 0);


	mtWld = mtS* mtT1 * mtR * mtT2;


	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDev->SetTransform(D3DTS_WORLD, &mtWld);
	m_pLnFnt->DrawTxt();
	
	
	D3DXMATRIX mtI;
	D3DXMatrixIdentity(&mtI);
	m_pDev->SetTransform(D3DTS_WORLD, &mtI);
}


