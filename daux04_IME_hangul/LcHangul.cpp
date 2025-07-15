// Implementation of the CLcHangul class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER >=1200
  #pragma warning(disable: 4996)
#endif


#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "LcHangul.h"


CLcHangul::CLcHangul()
{
	m_bUse	= FALSE;
	
	memset(m_sWrd, 0, sizeof m_sWrd);
	memset(m_sStr, 0, sizeof m_sStr);
	memset(m_sOut, 0, sizeof m_sOut);

	m_dStB	= 0;
}


CLcHangul::~CLcHangul()
{
}



INT CLcHangul::Create(DWORD dBackSpace)
{
	m_dStB = dBackSpace;
	return 0;
}



void CLcHangul::Destroy()
{
	Reset();
}


void CLcHangul::Set()
{
	m_bUse	= TRUE;
	
	memset(m_sWrd, 0, sizeof(m_sWrd));
	memset(m_sStr, 0, sizeof(m_sStr));
	memset(m_sOut, 0, sizeof(m_sOut));
}



void CLcHangul::Reset()
{
	m_bUse	= FALSE;
	
	memset(m_sWrd, 0, sizeof m_sWrd);
	memset(m_sStr, 0, sizeof m_sStr);
	memset(m_sOut, 0, sizeof m_sOut);
}


BOOL CLcHangul::GetState()			{ return m_bUse;	}
void CLcHangul::SetState(BOOL use)	{ m_bUse = use;		}



void CLcHangul::EraseWord()
{
	if( _tcslen(m_sWrd) )
		memset(m_sWrd, 0, sizeof(m_sWrd));
	
	else
	{
		int iLength = _tcslen(m_sStr);
		
		if( iLength > 0)
		{
			if(m_sStr[iLength - 1] < 0)											// 한글은 2바이트 코드...
			{
				m_sStr[iLength - 1] = 0;
				m_sStr[iLength - 2] = 0;
			}
			
			else																// 1바이트 코드들...
				m_sStr[iLength - 1] = 0;
		}
	}
}



LRESULT CLcHangul::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(!m_bUse)
		return FALSE;

	INT		len = 0;
	HIMC	hIMC= 0;
	WPARAM	wLo = LOWORD(wParam);


	if(m_dStB && WM_COMMAND == uMsg && m_dStB == wLo)							// String Table를 사용하고 Back space로 정의된 이벤트가 발생한 경우
	{
		EraseWord();
		return FALSE;
	}
	
	
	if(WM_IME_STARTCOMPOSITION == uMsg)
		return TRUE;
		
	else if(WM_IME_COMPOSITION == uMsg)											// 글씨 조합 중
	{
		hIMC = ImmGetContext(hWnd);												// Get IME Handle
		
		if (lParam & GCS_RESULTSTR)
		{
			len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);		// 조합이 완료된 길이

			if( 0 < len)														// 글씨가 조합이 되었으면
			{
				ImmGetCompositionString(hIMC, GCS_RESULTSTR, m_sWrd, len);		// 현재 IME의 스트링 길이를 얻는다

				if( _tcslen(m_sStr) <= MAX_CHAT_LEN)
					_tcscat(m_sStr, m_sWrd);

				memset(m_sWrd,0, sizeof(m_sWrd));
			}
		}
			
		else if (lParam & GCS_COMPSTR)											// 조합중이면;
		{
			len = ImmGetCompositionString(hIMC, GCS_COMPSTR, NULL, 0);			// 조합중인 길이를 얻는다.
			
			if(0==len)															// 조합중에 Back space가 온 경우
				EraseWord();

			else
				ImmGetCompositionString(hIMC, GCS_COMPSTR, m_sWrd, len);		// str에  조합중인 문자를 얻는다.
		}
		
		ImmReleaseContext(hWnd, hIMC);											// IME 핸들 반환!!
		
		return TRUE;
	}

	else if(WM_CHAR == uMsg)													// 한글이 조합중이 아닐 때(영문자, 기타 키 이벤트)
	{
		if( _tcslen(m_sStr) <= MAX_CHAT_LEN)
		{
			if(wParam>=32 && wParam<127)
				m_sStr[_tcslen(m_sStr)] = wParam;

			else if(!m_dStB && VK_BACK ==wParam)								// 스트링 테이블를 사용하지 않고 백스페이스인 경우
			{
				INT c;
				EraseWord();
				c=0;
			}
		}	
	}
	
	return FALSE;
}




int CLcHangul::OutStringBeam(TCHAR* sOut)
{
	_tcscpy(m_sOut, m_sStr);
	_tcscat(m_sOut, m_sWrd);
	_tcscpy(sOut, m_sOut);	

	static DWORD start = timeGetTime();
	static DWORD end;
	
	end = timeGetTime();
	
	if(end-start>1200)
		start = end;
	
	if(end-start>600)
		_tcscat(sOut, _T(" ") );
	
	else
		_tcscat(sOut, _T("|") );

	return _tcslen(m_sOut);
}



void CLcHangul::OutStringStar(TCHAR* sOut)
{
	TCHAR sOut1[512];
	memset(sOut1, 0, sizeof(sOut1));
	
	_tcscpy(sOut1, m_sStr);
	_tcscat(sOut1, m_sWrd);
	
	memset(sOut, '*', _tcslen(sOut1));
	
	static DWORD start = timeGetTime();
	static DWORD end;
	
	end = timeGetTime();
	
	if(end-start>1000)
		start = end;
	
	if(end-start>500)
		_tcscat(sOut, _T("|"));

	else
		_tcscat(sOut, _T(" ") );
}


int CLcHangul::OutString(TCHAR* sOut)
{
	_tcscpy(m_sOut, m_sStr);
	_tcscat(m_sOut, m_sWrd);
	_tcscpy(sOut, m_sOut);

	return _tcslen(m_sOut);
}


