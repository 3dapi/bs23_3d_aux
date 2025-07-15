


#include <vector>

#include "_StdAfx.h"


#define MRG_FILE_MAX_COUNTER	1000
//#define MRG_FILE_START_POINTER	(300 * MRG_FILE_MAX_COUNTER)
#define MRG_FILE_START_POINTER	(300000)


struct MrgTx
{
	char	sFile[256];		// File Name
	int		iSize;			// File Size
	int		iBgn;			// Starting point
	int		iEnd;			// End point


	MrgTx()
	{
		memset(sFile, 0, sizeof(sFile));

		iSize	= 0;
		iBgn	= MRG_FILE_START_POINTER;
		iEnd	= 0;
	}


	MrgTx(char* _sFile, MrgTx* pPrv=NULL)
	{
		strcpy(sFile, _sFile);

		FILE* fp = fopen(sFile, "rb");

		iBgn	= ftell(fp);
		
		fseek(fp, 0L, SEEK_END);
		iEnd	= ftell(fp);
		fclose(fp);

		iSize	= iEnd - iBgn;
		iBgn	= MRG_FILE_START_POINTER;
		iEnd	= iBgn+iSize;

		if(!pPrv)
			return;


		iBgn = pPrv->iEnd;
		iEnd = iBgn+iSize;
	}
};


typedef std::vector<MrgTx*	>	lsTx;
typedef lsTx::iterator			itTx;




void CMain::MergeFile()
{
	lsTx	vTx;

	// 1. Merge File Information
	vTx.push_back(new MrgTx("Texture/seatosky1.jpg"));
	vTx.push_back(new MrgTx("Texture/seatosky2.jpg",	vTx[vTx.size()-1]));
	vTx.push_back(new MrgTx("Texture/seatosky3.jpg",	vTx[vTx.size()-1]));
	vTx.push_back(new MrgTx("Texture/seatosky4.jpg",	vTx[vTx.size()-1]));
	vTx.push_back(new MrgTx("Texture/seatosky5.jpg",	vTx[vTx.size()-1]));

	vTx.push_back(new MrgTx("Texture/env3.bmp",			vTx[vTx.size()-1]));
	vTx.push_back(new MrgTx("Texture/hike.jpg",			vTx[vTx.size()-1]));
	vTx.push_back(new MrgTx("Texture/lake.jpg",			vTx[vTx.size()-1]));
	vTx.push_back(new MrgTx("Texture/lake2.jpg",		vTx[vTx.size()-1]));
	vTx.push_back(new MrgTx("Texture/lake3.jpg",		vTx[vTx.size()-1]));

	int		i;
	int		iN=vTx.size();
	BYTE*	pSrc;

	FILE* fpDst = fopen("Texture/Merge.mrg", "wb");
	FILE* fpSrc = NULL;


	// 2. Write File Counter
	fwrite(&iN, 1, sizeof(iN), fpDst);


	// 3. Write File Information
	for(i=0; i<iN; ++i)
	{
		MrgTx* pTxF = vTx[i];
		fwrite(pTxF->sFile,	1, sizeof(pTxF->sFile), fpDst);
		fwrite(&pTxF->iSize,1, sizeof(pTxF->iSize), fpDst);
		fwrite(&pTxF->iBgn,	1, sizeof(pTxF->iBgn), fpDst);
		fwrite(&pTxF->iEnd,	1, sizeof(pTxF->iEnd), fpDst);
	}

	// 4. Move File Point
	fseek(fpDst, MRG_FILE_START_POINTER, SEEK_SET);
	

	// 5. Write File Data
	for(i=0; i<iN; ++i)
	{
		MrgTx* pTxF = vTx[i];

		fpSrc = fopen(pTxF->sFile, "rb");
		pSrc = new BYTE[pTxF->iSize];
		fread(pSrc, 1, pTxF->iSize, fpSrc);
		fclose(fpSrc);
		
		fwrite(pSrc, 1, pTxF->iSize, fpDst);

		SAFE_DELETE_ARRAY(	pSrc	);
	}

	fclose(fpDst);


	for(i=0; i<iN; ++i)
	{
		MrgTx* pTxF = vTx[i];
		delete pTxF;
	}

	vTx.clear();
}


BYTE* CMain::GetFileData(int idx, int* iSize)
{
	int		i;
	int		iN	= 0;
	BYTE*	pBuf = NULL;

	lsTx	vTx;

	FILE* fp = fopen("Texture/Merge.mrg", "rb");

	
	// 2. Read File Counter
	fread(&iN, 1, sizeof(iN), fp);

	if(idx<0 || idx>=iN)
	{
		fclose(fp);
		return NULL;
	}


	// 3. Read File Information
	for(i=0; i<iN; ++i)
	{
		MrgTx* pTxF = new MrgTx;
		vTx.push_back(pTxF);
		pTxF	= vTx[i];

		fread(pTxF->sFile,	1, sizeof(pTxF->sFile), fp);
		fread(&pTxF->iSize,	1, sizeof(pTxF->iSize), fp);
		fread(&pTxF->iBgn,	1, sizeof(pTxF->iBgn), fp);
		fread(&pTxF->iEnd,	1, sizeof(pTxF->iEnd), fp);		
	}

	MrgTx* pTxF = vTx[idx];

	// 4. Move File Point
	fseek(fp, pTxF->iBgn, SEEK_SET);

	// 5. Set iSize Value
	*iSize = pTxF->iSize;
	

	// 6. Read File Data
	pBuf = new BYTE[pTxF->iSize];
	fread(pBuf, 1, pTxF->iSize, fp);
	fclose(fp);


	for(i=0; i<iN; ++i)
		SAFE_DELETE(vTx[i]);

	vTx.clear();

	return pBuf;
}


CMain::CMain()
{
	m_pD3DXFont		= NULL;

	m_nTx	= 1;
	m_pTx	= NULL;
}


HRESULT CMain::Init()
{
	HRESULT hr=-1;

	D3DXFONT_DESC hFont =
	{
		16, 0, FW_NORMAL
			, 1, 0
			, HANGUL_CHARSET
			, OUT_DEFAULT_PRECIS
			, ANTIALIASED_QUALITY
			, FF_DONTCARE
			, "Arial"
	};

	hr = D3DXCreateFontIndirect(m_pd3dDevice, &hFont, &m_pD3DXFont);

	if( FAILED(hr) )
		return -1;
	


	MergeFile();


	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pD3DXFont	);


	SAFE_RELEASE(	m_pTx	);
	
	return S_OK;
}



HRESULT CMain::Restore()
{
	if(m_pD3DXFont)
		m_pD3DXFont->OnResetDevice();	
	
	return S_OK;
}


HRESULT CMain::Invalidate()
{
	if(m_pD3DXFont)
		m_pD3DXFont->OnLostDevice();
	
	return S_OK;
}




HRESULT CMain::FrameMove()
{
	static DWORD dBgn = timeGetTime();

	DWORD dEnd = timeGetTime();

	if(dEnd-dBgn>1000)
	{
		dBgn = dEnd;
		int iIdx = rand()%10;

		INT		iSize=0;
		BYTE*	pSrc = GetFileData(iIdx, &iSize);

		SAFE_RELEASE(	m_pTx	);
		
		D3DXCreateTextureFromFileInMemoryEx(
			m_pd3dDevice
			, pSrc
			, iSize
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, 0
			, D3DFMT_UNKNOWN
			, D3DPOOL_MANAGED
			, D3DX_FILTER_NONE
			, D3DX_FILTER_NONE
			, 0x00FFFFFF
			, &m_Img
			, NULL
			, &m_pTx);

		SAFE_DELETE_ARRAY(pSrc);
	}

	return S_OK;
}


HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );
	
	if(FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);


	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	RECT rc={0,0, m_Img.Width, m_Img.Height};

	D3DXVECTOR3	vcPos(50,10, 0);

	for(int i=0; i<m_nTx; ++i)
	{
		m_pd3dSprite->Draw(m_pTx, &rc, NULL, &vcPos, D3DXCOLOR(1,1,1,1));
	}

	m_pd3dSprite->End();



	

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pd3dDevice->SetTexture( 0, NULL);

	RenderText();
		
	m_pd3dDevice->EndScene();
	
	return S_OK;
}



HRESULT CMain::RenderText()
{
	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
	TCHAR szMsg[MAX_PATH] = TEXT("");
	RECT rct;
	ZeroMemory( &rct, sizeof(rct) );       
	
	rct.top	   = rct.top + 20;
	rct.left   = 2;
	rct.right  = m_d3dsdBackBuffer.Width - 20;
	rct.bottom = rct.top + 20;

	sprintf( szMsg, "%s %s", m_strDeviceStats, m_strFrameStats);
	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rct, 0, fontColor );

	return S_OK;
}




LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_PAINT:
		{
			break;
		}
		
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}



