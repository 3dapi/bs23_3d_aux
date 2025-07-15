// Interface for the CMcInput class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCINPUT_H_
#define _MCINPUT_H_

class CMcInput
{
protected:
	LPDIRECT3DDEVICE9		m_pDev;
	HWND					m_hWnd;
	LPDIRECTINPUT8			m_pDInput;
	LPDIRECTINPUTDEVICE8	m_pDiKey;
	LPDIRECTINPUTDEVICE8	m_pDiMs;

	BYTE					KeyCur[256];
	BYTE					KeyOld[256];
	D3DXVECTOR3				m_vcMsCur;
	D3DXVECTOR3				m_vcMsOld;
	D3DXVECTOR3				m_vcDelta;
	DIMOUSESTATE			MsStCur;
	DIMOUSESTATE			MsStOld;

public:
	CMcInput();
	~CMcInput();

	INT			Create(LPDIRECT3DDEVICE9 pDev);
	INT			FrameMove();

	bool		GetKey(BYTE cKey);
	bool		KeyState(BYTE cKey);

	D3DXVECTOR3	GetMousePos();
	D3DXVECTOR3	GetMouseDelta();
	bool		GetMouseSt(INT nM);


protected:
	INT			InitDInput();
	INT			UpdateDInput();
};

#endif
