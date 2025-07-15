

#include "_StdAfx.h"


CMain::CMain()
{
	m_hFont	= NULL;
}

HRESULT CMain::Init()
{
	HRESULT hr=0;


	// ���� ��ü: http://design.seoul.go.kr/policy/data_view.php?id=63
	//��ü ���
	hr = AddFontResourceEx("font/08SeoulNamsanB.ttf", FR_NOT_ENUM, NULL);

	LOGFONT logf={0};
	logf.lfHeight      = 60;
	logf.lfWeight      = FW_BOLD;
	logf.lfCharSet     = HANGUL_CHARSET;
	logf.lfOutPrecision= OUT_DEFAULT_PRECIS;
	logf.lfQuality		= ANTIALIASED_QUALITY;
	logf.lfPitchAndFamily= FF_DONTCARE;
	
	_tcscpy(logf.lfFaceName, _T("08���ﳲ��ü B"));

	m_hFont = CreateFontIndirect(&logf);

	return S_OK;
}


HRESULT CMain::Destroy()
{
	DeleteObject(m_hFont);

	return S_OK;
}



HRESULT CMain::Restore()
{
	return S_OK;
}


HRESULT CMain::Invalidate()
{
	return S_OK;
}




HRESULT CMain::FrameMove()
{
	return S_OK;
}


HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );
	
		if(FAILED( m_pd3dDevice->BeginScene() ) )
			return -1;

		m_pd3dDevice->EndScene();


		// �ĸ� ������ ���� ���۸� �����´�.
		LPDIRECT3DSURFACE9	pBackBuffer = NULL;
		m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

		if(pBackBuffer)
		{
			// ���� ���ۿ��� DC�� �����´�.
			HDC hDC = NULL;
			pBackBuffer->GetDC(&hDC);

			if(hDC)
			{
				// ���ڿ��� ����Ѵ�.
				TCHAR sMsg[256] = "";
				sprintf( sMsg, "DC�� �̿��� ���ڿ� ����Դϴ�.   %s %s", m_strDeviceStats, m_strFrameStats );

				HFONT	hFontOld = NULL;
				hFontOld = (HFONT)SelectObject(hDC, m_hFont); 
				
				SetBkMode(hDC, TRANSPARENT);
				SetTextColor(hDC, RGB(255, 255, 128));
				TextOut(hDC, 10, 200, sMsg, strlen(sMsg));

				SelectObject(hDC, hFontOld);
				DeleteDC( hDC );

				pBackBuffer->ReleaseDC(hDC);
			}

			pBackBuffer->Release();
		}

	
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



