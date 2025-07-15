//
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable: 4996)

#ifndef __STDAFX_H_
#define __STDAFX_H_

#define STRICT
#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT			0x0400
#define _WIN32_WINDOWS			0x0400

#define DIRECTINPUT_VERSION		0x0800


#pragma warning( disable : 4018)
#pragma warning( disable : 4098)
#pragma warning( disable : 4100)
#pragma warning( disable : 4238)
#pragma warning( disable : 4245)
#pragma warning( disable : 4503)
#pragma warning( disable : 4663)
#pragma warning( disable : 4786)


// Static Library
#pragma comment(lib, "shell32.lib"		)
#pragma comment(lib, "comctl32.lib"		)



#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <d3dx9.h>
#include <tchar.h>
#include "DXUtil.h"
#include "D3DUtil.h"

#include "D3DEnum.h"
#include "D3DSettings.h"
#include "D3DApp.h"

#include "resource.h"

#include "LcXSkin/ILcMdl.h"






#include "Main.h"

#endif



