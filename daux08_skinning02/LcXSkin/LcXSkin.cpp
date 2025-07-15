



#include <windows.h>
#include <D3D9.h>
#include <d3dx9.h>


#include "ILcMdl.h"
#include "LcXSkin.h"


CLcXSkin::CLcXSkin()
{
	D3DXMatrixIdentity(&m_mtWorld);
	
	m_dTimeCur		= 0;
}


CLcXSkin::~CLcXSkin()
{
	Destroy();
}


void CLcXSkin::Destroy()
{
}


INT CLcXSkin::Create(void* p1, void* p2, void* p3, void* p4)
{
	return 0;
}


INT CLcXSkin::FrameMove()
{
	return 0;
}


void CLcXSkin::Render()
{
}


INT CLcXSkin::SetAttrib(char* sCmd, void* pVal)
{
	if(0 ==_stricmp(sCmd, "World Matrix"))
	{
		m_mtWorld =*((D3DXMATRIX*)pVal);
		return 0;
	}
	
	else if(0 ==_stricmp(sCmd, "Elapsed Time"))
	{
		FLOAT fTime =*((FLOAT*)pVal);
		m_dTimeCur += fTime;
		return 0;
	}
	
	else if(0 ==_stricmp(sCmd, "Current Time"))
	{
		FLOAT fTime =*((FLOAT*)pVal);
		m_dTimeCur = fTime;
		return 0;
	}
	
	return -1;
}

INT CLcXSkin::GetAttrib(char* sCmd, void* pVal)
{
	if(0 ==_stricmp(sCmd, "World Matrix"))
	{
		*((D3DXMATRIX*)pVal) = m_mtWorld;
		return 0;
	}
	
	else if(0 ==_stricmp(sCmd, "Current Time"))
	{
		*((DOUBLE*)pVal) = m_dTimeCur;
		return 0;
	}
	
	return -1;
}


INT CLcXSkin::SetPosition(FLOAT* float3)
{
	m_mtWorld._41 = float3[0];
	m_mtWorld._42 = float3[1];
	m_mtWorld._43 = float3[2];
	
	return 0;
}



INT CLcXSkin::GetPosition(FLOAT* float3)
{
	float3[0] = m_mtWorld._41;
	float3[1] = m_mtWorld._42;
	float3[2] = m_mtWorld._43;
	
	return 0;
}


