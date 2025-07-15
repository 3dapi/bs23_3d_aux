// Implementation of the CMcField class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcField::CMcField()
{
	m_pDev	= NULL;
	m_pVtx	= NULL;
	m_pIdx	= NULL;
	m_pTx0	= NULL;

	m_iNvx	= 0;
	m_iNix	= 0;
}

CMcField::~CMcField()
{
	Destroy();
}


INT CMcField::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev = pDev;

	m_fAl = 45;
	m_iW = 32;
	m_vcLgt = D3DXVECTOR3( -cosf(D3DXToRadian(m_fAl)), -sinf(D3DXToRadian(m_fAl)), 0);
	
	//	Load texture
	D3DXCreateTextureFromFile(m_pDev, "Texture/detail.png", &m_pTx0);
	
	MapLoad();	

	ZeroMemory( &m_light, sizeof(m_light) );
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Diffuse = D3DXCOLOR( 1.F, 1.F, 1.F, 1.F);
	m_light.Specular= D3DXCOLOR( 0.F, 0.F, .5F, .5F);

	D3DUtil_InitMaterial( m_mtl, 1.0f, 1.0f, 1.0f );
	
	return 1;
}


void CMcField::Destroy()
{
	
	SAFE_DELETE_ARRAY(	m_pVtx	);
	SAFE_DELETE_ARRAY(	m_pIdx	);
	
	SAFE_RELEASE(	m_pTx0	);
}


INT	CMcField::FrameMove()
{
	CMcInput* pInput = GMAIN->GetInput();

	if( pInput->KeyState(DIK_LEFT))
	{
		++m_fAl;
		m_vcLgt = D3DXVECTOR3( -cosf(D3DXToRadian(m_fAl)), -sinf(D3DXToRadian(m_fAl)), 0);
	}
	
	if( pInput->KeyState(DIK_RIGHT))
	{
		--m_fAl;
		m_vcLgt = D3DXVECTOR3( -cosf(D3DXToRadian(m_fAl)), -sinf(D3DXToRadian(m_fAl)), 0);
	}
	
	return 1;
}

void CMcField::Render()
{
	
	m_light.Direction = m_vcLgt;

	m_pDev->SetRenderState( D3DRS_LIGHTING,  TRUE);
	m_pDev->SetLight( 0, &m_light );
	m_pDev->LightEnable( 0, TRUE );
	
	m_pDev->SetMaterial( &m_mtl );
	m_pDev->SetRenderState( D3DRS_AMBIENT,   0x000F0F0F );

	D3DXMATRIX	mtWorld;
	D3DXMatrixIdentity(&mtWorld);
	
	m_pDev->SetTransform(D3DTS_WORLD, &mtWorld);
	
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
	
	m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	m_pDev->SetTexture(0, m_pTx0);
	m_pDev->SetFVF(m_dFVF);
	m_pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNvx, m_iNix, m_pIdx, D3DFMT_INDEX16, m_pVtx, m_iVxS);
	
	m_pDev->SetTexture(0, 0);
	
	m_pDev->SetRenderState( D3DRS_LIGHTING,  FALSE);
	m_pDev->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW);
	D3DXMatrixIdentity(&mtWorld);
	m_pDev->SetTransform(D3DTS_WORLD, &mtWorld);
}




void CMcField::NormalSet()
{
	D3DXVECTOR3	n(0,0,0);
	D3DXVECTOR3	a;
	D3DXVECTOR3	b;
	D3DXVECTOR3	nT;
	
	for(INT z=0; z<m_iN; ++z)
	{
		for(INT x=0; x<m_iN; ++x)
		{
			m_pVtx[m_iN*z + x].n = -NormalVec(z, x);
		}
	}
}


D3DXVECTOR3 CMcField::NormalVec(int z, int x)
{
	D3DXVECTOR3	n(0,0,0);
	D3DXVECTOR3	a;
	D3DXVECTOR3	b;
	D3DXVECTOR3	nT;
	INT		i;

	INT		index = m_iN*z + x;
	INT		iVtx[10];

	iVtx[9] = index;
	iVtx[0] = iVtx[9];
	iVtx[1] = iVtx[9];
	iVtx[2] = iVtx[9];
	iVtx[3] = iVtx[9];
	iVtx[4] = iVtx[9];
	iVtx[5] = iVtx[9];
	iVtx[6] = iVtx[9];
	iVtx[7] = iVtx[9];
	iVtx[8] = iVtx[9];
	
	if(0==z && 0==x)
	{
		iVtx[0] = iVtx[9] + 1;
		iVtx[1] = iVtx[0] + m_iN;
		iVtx[2] = iVtx[9] + m_iN;
	}

	else if(0==z && (m_iN-1) == x)
	{
		iVtx[0] = iVtx[9] + m_iN;
		iVtx[1] = iVtx[0] - 1;
		iVtx[2] = iVtx[9] - 1;
	}

	else if(0==z)
	{
		if(index%2)
		{
			iVtx[0] = iVtx[9] + 1;
			iVtx[1] = iVtx[0] + m_iN;
			iVtx[2] = iVtx[9] - 1;			
		}
		else
		{
			iVtx[0] = iVtx[9] + 1;
			iVtx[1] = iVtx[0] + m_iN;
			iVtx[2] = iVtx[9] + m_iN;
			iVtx[3] = iVtx[2] - 1;
			iVtx[4] = iVtx[9] - 1;
		}
	}
	
	else if( (m_iN-1) == z && 0==x)
	{
		iVtx[0] = iVtx[9] - m_iN;
		iVtx[1] = iVtx[0] + 1;
		iVtx[2] = iVtx[9] + 1;
	}

	else if( (m_iN-1) == z && (m_iN-1) == x)
	{
		iVtx[0] = iVtx[9] - 1;
		iVtx[1] = iVtx[0] - m_iN;
		iVtx[2] = iVtx[9] - m_iN;

	}

	else if((m_iN-1) == z)
	{
		if(index%2)
		{
			iVtx[0] = iVtx[9] - 1;
			iVtx[1] = iVtx[9] - m_iN;
			iVtx[2] = iVtx[9] + 1;
		}
		else
		{
			iVtx[0] = iVtx[9] - 1;
			iVtx[1] = iVtx[0] - m_iN;
			iVtx[2] = iVtx[9] - m_iN;
			iVtx[3] = iVtx[2] + 1;
			iVtx[4] = iVtx[9] + 1;
		}
	}

	else if(0 == x)
	{
		if(index%2)
		{
			iVtx[0] = iVtx[9] - m_iN;
			iVtx[1] = iVtx[9] + 1;
			iVtx[2] = iVtx[9] + m_iN;
		}
		else
		{
			iVtx[0] = iVtx[9] - m_iN;
			iVtx[1] = iVtx[0] + 1;
			iVtx[2] = iVtx[9] + 1;
			iVtx[3] = iVtx[2] + m_iN;
			iVtx[4] = iVtx[9] + m_iN;
		}
	}

	else if((m_iN-1) == x)
	{
		if(index%2)
		{
			iVtx[0] = iVtx[9] + m_iN;
			iVtx[1] = iVtx[9] - 1;
			iVtx[2] = iVtx[9] - m_iN;
		}
		else
		{
			iVtx[0] = iVtx[9] + m_iN;
			iVtx[1] = iVtx[0] - 1;
			iVtx[2] = iVtx[9] - 1;
			iVtx[3] = iVtx[2] - m_iN;
			iVtx[4] = iVtx[9] - m_iN;
		}
	}
	

	else
	{
		if(index%2)																// 홀 수
		{
			iVtx[0] = iVtx[9] - 1;
			iVtx[1] = iVtx[9] - m_iN;
			iVtx[2] = iVtx[9] + 1;
			iVtx[3] = iVtx[9] + m_iN;
			iVtx[4] = iVtx[0];
		}
		else																	// 짝 수
		{
			iVtx[6] = index +m_iN	-1;		iVtx[5] = iVtx[6] + 1;	iVtx[4] = iVtx[5] + 1;
			iVtx[7] = index			-1;		iVtx[9] = iVtx[7] + 1;	iVtx[3] = iVtx[9] + 1;
			iVtx[0] = index -m_iN	-1;		iVtx[1] = iVtx[0] + 1;	iVtx[2] = iVtx[1] + 1;
			iVtx[8] = iVtx[0];
		}
	}

	for(i=0; i<8; ++i)
	{
		a = m_pVtx[iVtx[i+0] ].p - m_pVtx[iVtx[9] ].p;
		b = m_pVtx[iVtx[i+1] ].p - m_pVtx[iVtx[9] ].p;
		D3DXVec3Cross(&nT, &a, &b);
		D3DXVec3Normalize(&nT, &nT);
		n +=nT;
	}

	
	D3DXVec3Normalize(&n, &n);
	
	return n;
}



void CMcField::MapLoad()
{
	INT x, z;
	

	m_dFVF = VtxNDUV1::FVF;
	m_iVxS = sizeof(VtxNDUV1);
	
	m_iN = 129;
	m_iNvx = m_iN * m_iN;

	m_pVtx = new VtxNDUV1[m_iNvx];
	
	for(z=0; z<m_iN; ++z)
	{
		for(x=0; x<m_iN; ++x)
		{
			m_pVtx[z * m_iN +x ].p = D3DXVECTOR3(FLOAT(x * m_iW), 0.f, FLOAT(z * m_iW));
		}
	}
	
	
	for(z=0; z<m_iN; ++z)														// 정점과 UV를 채운다.
	{
		for(x=0; x<m_iN; ++x)
		{
			m_pVtx[z * m_iN +x ].u = x /2.f;
			m_pVtx[z * m_iN +x ].v = z /2.f;
		}
	}
	

	NormalSet();																// 법선 벡터를 채운다.


	
	INT iN = m_iN-1;
	
	m_iNix = 8 * (m_iN-1)/2 * (m_iN-1)/2;
	
	m_pIdx = new VtxIdx[m_iNix];
	
	INT i=0;
	
	WORD index;
	WORD f[9];
	
	for(z=0; z< iN/2;++z)														// Index를 채운다.
	{
		for(x=0;x<iN/2;++x)
		{
			index = 2*m_iN*z + m_iN+1 + 2*x;
			
			f[6] = index +m_iN-1;	f[5] = index + m_iN;	f[4] = index +m_iN+1;
			f[7] = index      -1;	f[8] = index	   ;	f[3] = index      +1;
			f[0] = index -m_iN-1;	f[1] = index - m_iN;	f[2] = index -m_iN+1;
			
			
			i = z * iN/2 + x;
			i *=8;
			
			for(int m=0; m<8; ++m)
				m_pIdx[i+m] = VtxIdx( f[8], f[(m+1)%8], f[(m+0)%8]);
		}
	}

}