// Implementation of the CLcSprite class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#include "ILcSprite.h"


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }




class CLcSprite : public ILcSprite
{
protected:
	LPDIRECT3DDEVICE9		m_pDev;		// Device
	LPDIRECT3DSTATEBLOCK9	m_pStCur;	// Current State Block
	LPDIRECT3DSTATEBLOCK9	m_pStSpt;	// Sprite State Block
	
public:
	CLcSprite();
	virtual ~CLcSprite();

	INT		Create(LPDIRECT3DDEVICE9 pDev);

	virtual INT	Draw(void* pTx0, const RECT* pRcDrw=NULL
					, const FLOAT* pvcPos=NULL						// Position
					, DWORD Color=0xFFFFFFFF						// Duffuse color
					, const FLOAT* pvcRot=NULL, FLOAT fAngle=0.f	// Rotation and Rotation Angle(Degree)
					);
};



CLcSprite::CLcSprite()
{
	m_pDev		= NULL;

	m_pStCur	= NULL;
	m_pStSpt	= NULL;
}



CLcSprite::~CLcSprite()
{
	SAFE_RELEASE(	m_pStCur	);
	SAFE_RELEASE(	m_pStSpt	);
}



INT CLcSprite::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr=-1;

	m_pDev = (LPDIRECT3DDEVICE9)pDev;


	DWORD	vCur[128]={0};

	// Save Current State
	m_pDev->GetRenderState( D3DRS_FOGENABLE,			&vCur[ 0]);
	m_pDev->GetRenderState( D3DRS_LIGHTING,				&vCur[ 1]);
	m_pDev->GetRenderState( D3DRS_ZENABLE,				&vCur[ 2]);

	m_pDev->GetRenderState( D3DRS_ALPHATESTENABLE,		&vCur[ 3]);
	m_pDev->GetRenderState( D3DRS_ALPHABLENDENABLE,		&vCur[ 4]);
	m_pDev->GetRenderState( D3DRS_SRCBLEND,				&vCur[ 5]);
	m_pDev->GetRenderState( D3DRS_DESTBLEND,			&vCur[ 6]);

	m_pDev->GetSamplerState( 0, D3DSAMP_MINFILTER,		&vCur[ 7]);
	m_pDev->GetSamplerState( 0, D3DSAMP_MAGFILTER,		&vCur[ 8]);
	m_pDev->GetSamplerState( 0, D3DSAMP_MIPFILTER,		&vCur[ 9]);

	m_pDev->GetSamplerState( 0, D3DSAMP_ADDRESSU,		&vCur[10]);
	m_pDev->GetSamplerState( 0, D3DSAMP_ADDRESSV,		&vCur[11]);
	m_pDev->GetSamplerState( 0, D3DSAMP_ADDRESSW,		&vCur[12]);

	m_pDev->GetSamplerState( 1, D3DSAMP_MINFILTER,		&vCur[13]);
	m_pDev->GetSamplerState( 1, D3DSAMP_MAGFILTER,		&vCur[14]);
	m_pDev->GetSamplerState( 1, D3DSAMP_MIPFILTER,		&vCur[15]);

	m_pDev->GetSamplerState( 1, D3DSAMP_ADDRESSU,		&vCur[16]);
	m_pDev->GetSamplerState( 1, D3DSAMP_ADDRESSV,		&vCur[17]);
	m_pDev->GetSamplerState( 1, D3DSAMP_ADDRESSW,		&vCur[18]);

	m_pDev->GetTextureStageState( 0, D3DTSS_COLORARG1,	&vCur[19]);
	m_pDev->GetTextureStageState( 0, D3DTSS_COLORARG2,	&vCur[20]);
	m_pDev->GetTextureStageState( 0, D3DTSS_COLOROP,	&vCur[21]);

	m_pDev->GetTextureStageState( 0, D3DTSS_ALPHAARG1,	&vCur[22]);
	m_pDev->GetTextureStageState( 0, D3DTSS_ALPHAARG2,	&vCur[23]);
	m_pDev->GetTextureStageState( 0, D3DTSS_ALPHAOP,	&vCur[24]);

	m_pDev->GetTextureStageState( 1, D3DTSS_COLORARG1,	&vCur[25]);
	m_pDev->GetTextureStageState( 1, D3DTSS_COLORARG2,	&vCur[26]);
	m_pDev->GetTextureStageState( 1, D3DTSS_COLOROP,	&vCur[27]);

	m_pDev->GetTextureStageState( 1, D3DTSS_ALPHAARG1,	&vCur[28]);
	m_pDev->GetTextureStageState( 1, D3DTSS_ALPHAARG2,	&vCur[29]);
	m_pDev->GetTextureStageState( 1, D3DTSS_ALPHAOP,	&vCur[30]);



	// Record State Block
	m_pDev->BeginStateBlock();

	m_pDev->SetRenderState( D3DRS_FOGENABLE,			FALSE );
	m_pDev->SetRenderState( D3DRS_LIGHTING,				FALSE );
	m_pDev->SetRenderState( D3DRS_ZENABLE,				FALSE);

	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE,		TRUE);
	m_pDev->SetRenderState( D3DRS_SRCBLEND,				D3DBLEND_SRCALPHA );
	m_pDev->SetRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA );

	m_pDev->SetSamplerState( 0, D3DSAMP_MINFILTER,		D3DTEXF_NONE);
	m_pDev->SetSamplerState( 0, D3DSAMP_MAGFILTER,		D3DTEXF_NONE);
	m_pDev->SetSamplerState( 0, D3DSAMP_MIPFILTER,		D3DTEXF_NONE);

	m_pDev->SetSamplerState( 0, D3DSAMP_ADDRESSU,		D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState( 0, D3DSAMP_ADDRESSV,		D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState( 0, D3DSAMP_ADDRESSW,		D3DTADDRESS_CLAMP);


	m_pDev->SetSamplerState( 1, D3DSAMP_MINFILTER,		D3DTEXF_NONE);
	m_pDev->SetSamplerState( 1, D3DSAMP_MAGFILTER,		D3DTEXF_NONE);
	m_pDev->SetSamplerState( 1, D3DSAMP_MIPFILTER,		D3DTEXF_NONE);

	m_pDev->SetSamplerState( 1, D3DSAMP_ADDRESSU,		D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState( 1, D3DSAMP_ADDRESSV,		D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState( 1, D3DSAMP_ADDRESSW,		D3DTADDRESS_CLAMP);


	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE);
	m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE);

	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE);
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);

	// End Record and Create State Block
	m_pDev->EndStateBlock(&m_pStSpt);



	m_pDev->BeginStateBlock();
	m_pDev->SetRenderState( D3DRS_FOGENABLE,			vCur[ 0]);
	m_pDev->SetRenderState( D3DRS_LIGHTING,				vCur[ 1]);
	m_pDev->SetRenderState( D3DRS_ZENABLE,				vCur[ 2]);

	m_pDev->SetRenderState( D3DRS_ALPHATESTENABLE,		vCur[ 3]);
	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE,		vCur[ 4]);
	m_pDev->SetRenderState( D3DRS_SRCBLEND,				vCur[ 5]);
	m_pDev->SetRenderState( D3DRS_DESTBLEND,			vCur[ 6]);

	m_pDev->SetSamplerState( 0, D3DSAMP_MINFILTER,		vCur[ 7]);
	m_pDev->SetSamplerState( 0, D3DSAMP_MAGFILTER,		vCur[ 8]);
	m_pDev->SetSamplerState( 1, D3DSAMP_MINFILTER,		vCur[ 9]);
	m_pDev->SetSamplerState( 1, D3DSAMP_MAGFILTER,		vCur[10]);

	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1,	vCur[11]);
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2,	vCur[12]);
	m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,	vCur[13]);

	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	vCur[14]);
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2,	vCur[15]);
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,	vCur[16]);

	m_pDev->SetTextureStageState( 1, D3DTSS_COLORARG1,	vCur[17]);
	m_pDev->SetTextureStageState( 1, D3DTSS_COLORARG2,	vCur[18]);
	m_pDev->SetTextureStageState( 1, D3DTSS_COLOROP,	vCur[19]);

	m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAARG1,	vCur[20]);
	m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAARG2,	vCur[21]);
	m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,	vCur[22]);
	m_pDev->EndStateBlock(&m_pStCur);

	return 0;
}


INT CLcSprite::Draw(void* pTx0, const RECT* pRcDrw, const FLOAT* pvcPos
					, DWORD Color
					, const FLOAT* pvcRot, FLOAT fAngle)
{
	LPDIRECT3DTEXTURE9	pTex0= (LPDIRECT3DTEXTURE9)	pTx0;

	if(NULL == pTex0)
		return -1;


	D3DSURFACE_DESC dsc;
	pTex0->GetLevelDesc(0, &dsc);
	FLOAT fImgW = FLOAT(dsc.Width);		// Width of Surface
	FLOAT fImgH = FLOAT(dsc.Height);	// Height of Surface

	FLOAT fTheta =D3DXToRadian(fAngle);

	// Draw Region Left, Top, Width, Height
	FLOAT	fDrwL=  0;
	FLOAT	fDrwT=  0;
	FLOAT	fDrwW=fImgW;
	FLOAT	fDrwH=fImgH;


	D3DXVECTOR2	pos[4];

	D3DXVECTOR2 rot(0,0);
	D3DXCOLOR	dColor = Color;
	D3DXVECTOR2	uv0(0,0);
	D3DXVECTOR2	uv1(1,1);


	// Draw Region Width, Height
	if(pRcDrw)
	{
		fDrwL = FLOAT(pRcDrw->left);
		fDrwT = FLOAT(pRcDrw->top);
		fDrwW = FLOAT(pRcDrw->right - pRcDrw->left);
		fDrwH = FLOAT(pRcDrw->bottom- pRcDrw->top );
	}


	// Setup UV
	uv0.x = (fDrwL +     0)/fImgW;
	uv1.x = (fDrwL + fDrwW)/fImgW;

	uv0.y = (fDrwT +     0)/fImgH;
	uv1.y = (fDrwT + fDrwH)/fImgH;


	// Setup Draw Position
	if(pvcPos)
	{
		pos[0].x = pvcPos[0];
		pos[0].y = pvcPos[1];
	}
	else
	{
		pos[0].x = 0;
		pos[0].y = 0;
	}


	// Setup Postion
	pos[1] = pos[0] + D3DXVECTOR2(fDrwW,     0);
	pos[2] = pos[0] + D3DXVECTOR2(fDrwW, fDrwH);
	pos[3] = pos[0] + D3DXVECTOR2(    0, fDrwH);


	// 회전 적용
	if(pvcRot)
	{
		D3DXVECTOR2 vcR = D3DXVECTOR2(pvcRot[0], pvcRot[1]);
		D3DXVECTOR2 d(0,0);

		FLOAT	fCos = cosf(fTheta);
		FLOAT	fSin = sinf(fTheta);

		for(INT i=0; i<4; ++i)
		{
			D3DXVECTOR2 vcT = pos[i] - vcR;

			d.x = vcT.x * fCos + vcT.y * fSin;
			d.y =-vcT.x * fSin + vcT.y * fCos;

			pos[i] = vcR + d;
		}
	}



	
	// Setup Vertices
	struct TVtxRHWUV1
	{
		FLOAT		p[4];
		DWORD		d;
		FLOAT		u, v;
	} pVtx[4] =
	{
		{pos[0].x, pos[0].y,  0,  1,  dColor,   uv0.x,  uv0.y},
		{pos[1].x, pos[1].y,  0,  1,  dColor,   uv1.x,  uv0.y},
		{pos[2].x, pos[2].y,  0,  1,  dColor,   uv1.x,  uv1.y},
		{pos[3].x, pos[3].y,  0,  1,  dColor,   uv0.x,  uv1.y},
	};


	// Rendering 2D

	// Save Current State
	m_pStCur->Capture();

	// Apply 2D Sprite State
	m_pStSpt->Apply();
	

	m_pDev->SetTexture(0, pTex0);
	m_pDev->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	m_pDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, pVtx, sizeof(TVtxRHWUV1) );
	m_pDev->SetTexture(0, NULL);

	m_pStCur->Apply();

	return 0;
}





INT LcxCreateSprite(ILcSprite** pOut, void* pd3dDevice)
{
	CLcSprite*	pObj=NULL;

	pObj = new CLcSprite;

	if(FAILED(pObj->Create((LPDIRECT3DDEVICE9)pd3dDevice)))
	{
		delete pObj;
		return -1;
	}

	*pOut = pObj;
	return 0;
}

