// Implementation of the CMcInput class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcInput::CMcInput()
{
	m_pDev		= NULL;
	m_hWnd		= NULL;

	m_pDInput	= NULL;
	m_pDiKey	= NULL;
	m_pDiMs		= NULL;
}

CMcInput::~CMcInput()
{
	if(m_pDiKey)
	{
		m_pDiKey->Release();
		m_pDiKey	= NULL;
	}

	if(m_pDiMs)
	{
		m_pDiMs->Release();
		m_pDiMs	= NULL;
	}

	if(m_pDInput)
	{
		m_pDInput->Release();
		m_pDInput	= NULL;
	}
}


INT CMcInput::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev	= pDev;

	D3DDEVICE_CREATION_PARAMETERS	ppm={0};
	pDev->GetCreationParameters( &ppm);
	m_hWnd	= ppm.hFocusWindow;

	memset(KeyCur, 0, sizeof(KeyCur));
	memset(KeyOld, 0, sizeof(KeyOld));

	memset(&m_vcMsCur, 0, sizeof (m_vcMsCur));
	memset(&m_vcMsOld, 0, sizeof(m_vcMsOld));
	memset(&MsStCur, 0, sizeof(MsStCur));
	memset(&MsStOld, 0, sizeof(MsStOld));


	InitDInput();

	return 0;
}

INT CMcInput::FrameMove()
{
	m_vcDelta = m_vcMsCur - m_vcMsOld;

	memcpy(KeyOld		, KeyCur	, sizeof(KeyCur		));
	memcpy(&m_vcMsOld	, &m_vcMsCur, sizeof(m_vcMsCur	));
	memcpy(&MsStOld		, &MsStCur	, sizeof(MsStCur	));

	memset(&MsStCur, 0  , sizeof(MsStCur));

//	UpdateGeneral();
	UpdateDInput();

	return 0;
}


bool CMcInput::GetKey(BYTE cKey)
{
	return (KeyCur[cKey] & 0x80 && KeyOld[cKey] != KeyCur[cKey]);
}

bool CMcInput::KeyState(BYTE cKey)
{
	return (KeyCur[cKey] & 0x80 && true);
}

bool CMcInput::GetMouseSt(INT nBtn)
{
	return MsStOld.rgbButtons[nBtn]? true: false;
}

D3DXVECTOR3	CMcInput::GetMousePos()
{
	return m_vcMsCur;
}


D3DXVECTOR3	CMcInput::GetMouseDelta()
{
	return m_vcDelta;
}

INT CMcInput::InitDInput()
{
	HINSTANCE	hInst = (HINSTANCE) GetModuleHandle(NULL);
	DWORD		flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY;

	// Device
	if (FAILED(DirectInput8Create(	hInst,	DIRECTINPUT_VERSION,	IID_IDirectInput8,	(void **)&m_pDInput,	NULL)))
		return -1;


	// Keyboard
	if (FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDiKey, NULL)))
	{
		MessageBox(m_hWnd, "Create keboard device failed","Err" , MB_OK | MB_ICONERROR);
		return -1;
	}

	if (FAILED(m_pDiKey->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(m_hWnd, "Set Data format failed", "Err", MB_OK | MB_ICONERROR);
		return -1;
	}

	if (FAILED(m_pDiKey->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		MessageBox(m_hWnd, "Set cooperativelevel failed", "Err", MB_OK | MB_ICONERROR);
		return -1;
	}


	// Mouse
	if (FAILED(m_pDInput->CreateDevice(GUID_SysMouse, &m_pDiMs, NULL)))
	{
		MessageBox(m_hWnd, "Create mouse device failed", "Err", MB_ICONERROR);
		return -1;
	}

	if (FAILED(m_pDiMs->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(m_hWnd, "Set mouse data format failed", "Err", MB_ICONERROR);
		return -1;
	}

	if (FAILED(m_pDiMs->SetCooperativeLevel(m_hWnd, flags)))
	{
		MessageBox(m_hWnd, "Set mouse cooperativelevel failed", "Err", MB_ICONERROR);
		return -1;
	}


	return 0;
}


INT CMcInput::UpdateDInput()
{
	// Keyboard
	if (FAILED(m_pDiKey->GetDeviceState(sizeof(KeyCur), (LPVOID)KeyCur)))
	{
		memset(KeyCur, 0, sizeof(KeyCur));

		if (FAILED(m_pDiKey->Acquire()))
		{
			return -1;
		}

		if (FAILED(m_pDiKey->GetDeviceState(sizeof(KeyCur), (LPVOID)KeyCur)))
		{
			return -1;
		}
	}



	// Mouse
	if (FAILED(m_pDiMs->GetDeviceState(sizeof(DIMOUSESTATE), &MsStCur)))
	{
		if (FAILED(m_pDiMs->Acquire()))
		{
			return -1;
		}

		if (FAILED(m_pDiMs->GetDeviceState(sizeof(DIMOUSESTATE), &MsStCur)))
		{
			return -1;
		}
	}

	POINT	MsPos;
	GetCursorPos(&MsPos);
	::ScreenToClient(m_hWnd, &MsPos);
	m_pDev->SetCursorPosition( MsPos.x, MsPos.y, 0 );

	m_vcMsCur.x = FLOAT(MsPos.x);
	m_vcMsCur.y = FLOAT(MsPos.y);

	m_vcMsCur.z += FLOAT(MsStCur.lZ);

	return 0;
}