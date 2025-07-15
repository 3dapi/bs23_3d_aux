

#include "_StdAfx.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>



const INT MaxCount = 4;



// ���� MMX�̻� �迭������ �۵��ϴ� ���ػ� performance counter
// �ý��� ���� clock�� 0 
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




FLOAT UseClock()																// c-style�� clock�� �̿��� FPS���ϱ�
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


FLOAT UseTimeGetTime()															// WinApi�� timeGetTime�� �̿��� FPS���ϱ�
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


FLOAT UsePerformanceCounter()													// �ϵ����� �����Ǵ� UsePerformanceCounter�� �̿��� FPS���ϱ�
{
	static DOUBLE			fFps = 0.;
	static INT				iCnt = 0;
	static LARGE_INTEGER	Freq = {0};
	static LARGE_INTEGER	dBgn = {0};
	static BOOL		bEnable=1;

	LARGE_INTEGER	dCur = {0};

	if(0 == bEnable)
		return -1.f;															// �ϵ���� ������ �ȵ�

	if(0 == Freq.QuadPart)														// ���� �Լ� ȣ���
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



// ���ػ� Ÿ�̸�
// �ð��� 1/1,000,000 �� ������ ���� �� �ִ�.
// QueryPerformanceCounter()���� ���� ���� QueryPerformanceFrequency() �Լ����� ���� ������
// ������ �ð�(��)�� �ȴ�.
// ���⿡ 1000�� ���ϸ� timeGetTime()���� ���� ���� ���� ms �ȴ�.
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
// �ý����� timeGetTime()�� ���� ���� �˾ƿ���
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


