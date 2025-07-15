

#include "_StdAfx.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>



const INT MaxCount = 4;



// 인텔 MMX이상 계열에서만 작동하는 고해상도 performance counter
// 시스템 대기면 clock이 0 
DOUBLE GetIntelPrecisionTime()
{
	#define cpuid __asm __emit 0fh __asm __emit 0a2h
	#define rdtsc __asm __emit 0fh __asm __emit 031h

	static LARGE_INTEGER	Freq ={0};
	static BOOL		bEnable = 1;

	LARGE_INTEGER	dCur ={0};
	DOUBLE			dTime =0;

	if(0 == bEnable)
		return -1;

	if(0 == Freq.QuadPart)
	{
		bEnable = QueryPerformanceFrequency(&Freq);

		if(0 == bEnable)
			return -1;
	}

	__asm
	{
		cpuid
		rdtsc
		mov dCur.LowPart, eax
		mov dCur.HighPart, edx
	}

	dTime = DOUBLE(dCur.QuadPart)/DOUBLE(Freq.QuadPart);
	dTime *=1000.;

	return  dTime;
}




FLOAT UseClock()																// c-style의 clock을 이용한 FPS구하기
{
	static DOUBLE	fFps = 0.;
	static INT		iCnt = 0;
	static clock_t	dBgn = clock();

	clock_t	dCur = clock();
	++iCnt;

	if(iCnt>=MaxCount)
	{
		fFps = DOUBLE(dCur - dBgn);
		fFps = iCnt*1000./fFps;

		iCnt = 0;
		dBgn = dCur;
	}

	return (FLOAT)fFps;
}


FLOAT UseTimeGetTime()															// WinApi의 timeGetTime을 이용한 FPS구하기
{
	static DOUBLE	fFps = 0.;
	static INT		iCnt = 0;
	static DWORD	dBgn = timeGetTime();

	DWORD	dCur = timeGetTime();												// GetTickCount()
	++iCnt;

	if(iCnt>=MaxCount)
	{
		fFps = DOUBLE(dCur-dBgn);
		fFps = iCnt*1000./fFps;

		iCnt = 0;
		dBgn = dCur;
	}

	return (FLOAT)fFps;
}


FLOAT UseTimeGetPrecisionTime()													//
{
	static DOUBLE	fFps = 0.;
	static INT		iCnt = 0;
	static DOUBLE	dBgn = GetIntelPrecisionTime();
	
	DOUBLE	dCur = GetIntelPrecisionTime();
	++iCnt;

	if(iCnt>=MaxCount)
	{
		fFps = dCur-dBgn;
		fFps = iCnt*1000./fFps;

		iCnt = 0;
		dBgn = dCur;
	}

	return (FLOAT)fFps;
}


FLOAT UsePerformanceCounter()													// 하드웨어에서 지원되는 UsePerformanceCounter를 이용한 FPS구하기
{
	static DOUBLE			fFps = 0.;
	static INT				iCnt = 0;
	static LARGE_INTEGER	Freq = {0};
	static LARGE_INTEGER	dBgn = {0};
	static BOOL		bEnable=1;

	LARGE_INTEGER	dCur = {0};

	if(0 == bEnable)
		return -1.f;															// 하드웨어 지원이 안됨

	if(0 == Freq.QuadPart)														// 최초 함수 호출시
	{
		bEnable = QueryPerformanceFrequency( &Freq);

		if(0 == bEnable)
			return -1.f;

		QueryPerformanceCounter(&dBgn);
	}


	QueryPerformanceCounter(&dCur);
	++iCnt;

	if(iCnt>=MaxCount)
	{
		DOUBLE fElapsedTime = DOUBLE(dCur.QuadPart - dBgn.QuadPart)/ DOUBLE(Freq.QuadPart);
		fFps = iCnt/fElapsedTime;

		iCnt = 0;
		dBgn = dCur;
	}

	return (FLOAT)fFps;
}



// 고해상도 타이머
// 시간을 1/1,000,000 초 단위로 읽을 수 있다.
// QueryPerformanceCounter()에서 읽은 값을 QueryPerformanceFrequency() 함수에서 읽은 값으로
// 나누면 시간(초)이 된다.
// 여기에 1000을 곱하면 timeGetTime()으로 읽은 값과 같은 ms 된다.
DOUBLE TimeGetPrecisionTime()
{
	static LARGE_INTEGER	Freq ={0};
	static BOOL				bEnable = 1;

	LARGE_INTEGER	dCur ={0};
	DOUBLE			dTime =0;

	if(0 == bEnable)
		return -1;

	if(0 == Freq.QuadPart)
	{
		bEnable = QueryPerformanceFrequency(&Freq);

		if(0 == bEnable)
			return -1;
	}

	QueryPerformanceCounter(&dCur);

	dTime = DOUBLE(dCur.QuadPart)/DOUBLE(Freq.QuadPart);
	dTime *=1000.;

	return  dTime;
}




CMain::CMain()
{    
}

HRESULT CMain::Init()
{
	TIMECAPS	caps={0};

//	timeBeginPeriod(1);
// 시스템의 timeGetTime()에 대한 성능 알아오기
	timeGetDevCaps(&caps, sizeof(TIMECAPS) );
  
	return S_OK;
}


HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0L );
	
	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;
	
	m_pd3dDevice->EndScene();
	
	
	
	HDC hDC= NULL;	
	m_pd3dBackBuffer->GetDC(&hDC);
	
	if(hDC)
	{
		TCHAR sMsg[256] = "";
		
		SetBkMode(hDC, TRANSPARENT);

		SetTextColor(hDC, RGB(128, 255, 255));
		sprintf( sMsg, "%s %s", m_strDeviceStats, m_strFrameStats );
		TextOut(hDC, 5, 10, sMsg, strlen(sMsg));


		SetTextColor(hDC, RGB(255, 128, 128));	sprintf( sMsg, "FPS: %5.f   UseClock", UseClock() );
		TextOut(hDC, 5,  75, sMsg, strlen(sMsg));
		
		SetTextColor(hDC, RGB(255, 255, 0));		sprintf( sMsg, "FPS: %5.f   UseTimeGetTime", UseTimeGetTime() );
		TextOut(hDC, 5, 100, sMsg, strlen(sMsg));


		SetTextColor(hDC, RGB(255, 200, 255));		sprintf( sMsg, "FPS: %5.f   UseTimeGetPrecisionTime", UseTimeGetPrecisionTime() );
		TextOut(hDC, 5, 125, sMsg, strlen(sMsg));

		
		SetTextColor(hDC, RGB(200, 200, 255));	sprintf( sMsg, "FPS: %5.f   UsePerformanceCounter", UsePerformanceCounter() );
		TextOut(hDC, 5, 150, sMsg, strlen(sMsg));


		SetTextColor(hDC, RGB(200, 255, 255));	sprintf( sMsg, "timeGetTime():                 %ld", timeGetTime());
		TextOut(hDC, 5, 250, sMsg, strlen(sMsg));

		SetTextColor(hDC, RGB(255, 200, 255));	sprintf( sMsg, "TimeGetPrecisionTime(): %.lf", TimeGetPrecisionTime() );
		TextOut(hDC, 5, 270, sMsg, strlen(sMsg));

		SetTextColor(hDC, RGB(255, 255, 200));	sprintf( sMsg, "GetIntelPrecisionTime(): %.lf", GetIntelPrecisionTime() );
		TextOut(hDC, 5, 290, sMsg, strlen(sMsg));
		
		m_pd3dBackBuffer->ReleaseDC(hDC);
	}
	
	return S_OK;
}


