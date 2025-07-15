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
	m_pMshStr	= NULL;
}


CMcScene::~CMcScene()
{
	Destroy();
}

void CMcScene::Destroy()
{
	SAFE_RELEASE(	m_pMshStr	);

	RemoveFontResourceEx("font/08SeoulNamsanB.ttf", FR_NOT_ENUM, NULL);
}


INT CMcScene::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr=0;
	m_pDev	= pDev;


	// 서울 서체: http://design.seoul.go.kr/policy/data_view.php?id=63
	//서체 등록
	hr = AddFontResourceEx("font/08SeoulNamsanB.ttf", FR_NOT_ENUM, NULL);
	
	
	// DC 생성
	HDC hdc = CreateCompatibleDC( NULL );
	HFONT hFontNew = NULL;
	HFONT hFontOld = NULL;


	LOGFONT hfont={0};

	hfont.lfHeight      = 32;
	hfont.lfWeight      = FW_BOLD;
	hfont.lfCharSet     = HANGUL_CHARSET;
	hfont.lfOutPrecision= OUT_DEFAULT_PRECIS;
	hfont.lfQuality		= ANTIALIASED_QUALITY;
	hfont.lfPitchAndFamily= FF_DONTCARE;

	_tcscpy(hfont.lfFaceName, _T("08서울남산체 B"));

	hFontNew = CreateFontIndirect(&hfont);
	hFontOld = (HFONT)SelectObject(hdc, hFontNew); 


	char	sSrc[256] = "안녕하세요  Hello world !  Α.Ω. \0";
	WCHAR sDst[256]={0};

	// 멀티 바이트를 유니코드로 변경
	LcStr_AnsiToUnicode(sDst, sSrc, strlen(sSrc)+1);

	// 3D 문자열을 생성
	D3DXCreateTextW(m_pDev, hdc, sDst, 200.f, 3.f, &m_pMshStr, 0, 0);

	SelectObject(hdc, hFontOld);
	DeleteObject( hFontNew);

	// DC 해제
	DeleteDC( hdc );

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
	
	D3DXCOLOR		dColor(1,1,0,1);
	D3DLIGHT9		light;
	D3DMATERIAL9	mtrl  = { {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {0,0,0,1}, 3.F};



	D3DXMATRIX mtWld;
	D3DXMATRIX mtS, mtR, mtT1, mtT2;
	float fRot = D3DXToRadian(GetTickCount() * 0.03f);

	D3DXMatrixIdentity(&mtS);
	D3DXMatrixIdentity(&mtR);
	D3DXMatrixIdentity(&mtT1);
	D3DXMatrixIdentity(&mtT2);

	
	D3DXMatrixScaling(&mtS, 5, 5, 5);
	D3DXMatrixRotationX(&mtR, fRot);
	D3DXMatrixTranslation(&mtT1, -30, 0, 0);


	mtWld = mtS* mtT1 * mtR * mtT2;



	memset(&light, 0, sizeof(D3DLIGHT9));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = dColor * 0.2f;
	light.Diffuse   = dColor;
	light.Specular  = dColor * 0.7f;
	light.Direction = D3DXVECTOR3(0.0f, -0.5f, 1.0f);
	
	m_pDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDev->SetLight(0, &light);
	m_pDev->LightEnable(0, TRUE);
	
	m_pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	m_pDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	

	m_pDev->SetTransform(D3DTS_WORLD, &mtWld);
	m_pDev->SetMaterial(&mtrl);
	m_pMshStr->DrawSubset(0);
	
	
	D3DXMATRIX mtI;
	D3DXMatrixIdentity(&mtI);
	m_pDev->SetTransform(D3DTS_WORLD, &mtI);
}


