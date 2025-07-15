// Interface for the CMcCamera class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MCCAMERA_H_
#define _MCCAMERA_H_

class CMcCamera
{
protected:
	LPDIRECT3DDEVICE9	m_pDev;

	D3DXMATRIX		m_mtViw;													// View Matrix
	D3DXMATRIX		m_mtPrj;													// Projection Matrix

	D3DXVECTOR3		m_vcEye;													// Camera position
	D3DXVECTOR3		m_vcLook;													// Look vector
	D3DXVECTOR3		m_vcUp;														// up vector

	FLOAT			m_fYaw;
	FLOAT			m_fPitch;
	
public:
	CMcCamera();
	virtual ~CMcCamera();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	INT		FrameMove();

	void	SetTransform();

	void	MoveSideward(FLOAT	fSpeed);
	void	MoveForward	(FLOAT	fSpeed, FLOAT fY=0);

	void	Rotate(FLOAT Yaw, FLOAT Pitch);
};

#endif
