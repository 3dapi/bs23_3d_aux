#ifndef __STDAFX_H_
#define __STDAFX_H_

#pragma warning(disable: 4996)

#pragma once

#pragma comment(lib, "dinput8.lib")

#define _WIN32_WINNT			0x0400
#define DIRECTINPUT_VERSION		0x0800

#define STRICT



#include <windows.h>

#include <windowsx.h>
#include <mmsystem.h>

#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>

#include <math.h>
#include <stdio.h>
#include <tchar.h>

#include <D3D9.h>
#include <d3dx9.h>

#include <dinput.h>

#include "D3DUtil.h"
#include "DXUtil.h"

#include "D3DEnum.h"
#include "D3DSettings.h"
#include "D3DApp.h"
#include "resource.h"



#include "VtxFmt.h"
#include "McUtil.h"
#include "McInput.h"
#include "McCamera.h"

#include "McField.h"															// Height field




#include "Main.h"

#endif __STDAFX_H_

