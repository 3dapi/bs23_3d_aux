#if _MSC_VER >=1200
  #pragma warning(disable: 4996)
#endif

#pragma warning(disable: 4324)



#ifndef __STDAFX_H_
#define __STDAFX_H_

#define STRICT
#define	WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT			0x0500
#define DIRECTINPUT_VERSION		0x0800

#pragma comment(lib, "dinput8.lib")

#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <commctrl.h>
#include <commdlg.h>

#include <math.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>


#include <D3D9.h>
#include <d3dx9.h>

#include <dinput.h>

#include "DXUtil.h"
#include "D3DUtil.h"
#include "D3DEnum.h"
#include "D3DSettings.h"

#include "resource.h"
#include "D3DApp.h"

#include "VtxFmt.h"
#include "McUtil.h"

#include "McInput.h"
#include "McCamera.h"

#include "McScene.h"




#include "Main.h"

#endif