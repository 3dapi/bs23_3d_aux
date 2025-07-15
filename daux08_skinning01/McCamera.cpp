// Implementation of the CMcCamera class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcCamera::CMcCamera()
{
	m_pDev	= NULL;
}

CMcCamera::~CMcCamera()
{

}



INT CMcCamera::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev	= pDev;

	m_fYaw		= 0.f;
	m_fPitch	= 0.f;
	m_vcEye		= D3DXVECTOR3( 0,  0.f, -100.f);
	m_vcLook	= D3DXVECTOR3( 0,  0.f,    0.f);
	m_vcUp		= D3DXVECTOR3( 0,  1.f,    0.f);

	D3DXMatrixPerspectiveFovLH(&m_mtPrj,D3DX_PI/4.f, 800.f/600.f, 1.f, 5000.f);
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);

	return 0;
}


INT CMcCamera::FrameMove()
{
	m_pDev->SetTransform(D3DTS_VIEW, &m_mtViw);
	m_pDev->SetTransform(D3DTS_PROJECTION, &m_mtPrj);

	return 0;
}

void CMcCamera::MoveSideward(FLOAT fSpeed)
{
	D3DXVECTOR3 tmp(m_mtViw._11, 0, m_mtViw._31);
	D3DXVec3Normalize(&tmp,&tmp);

	m_vcEye  += tmp * fSpeed;
	m_vcLook += tmp * fSpeed;

	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);
}


void CMcCamera::MoveForward(FLOAT fSpeed, FLOAT fY)
{
	D3DXVECTOR3 tmp(m_mtViw._13, m_mtViw._23*fY, m_mtViw._33);
	D3DXVec3Normalize(&tmp,&tmp);

	m_vcEye  += tmp * fSpeed;
	m_vcLook += tmp * fSpeed;


	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);
}




void CMcCamera::Rotate(FLOAT Yaw, FLOAT Pitch)
{
	m_fYaw	= D3DXToRadian(Yaw * 0.1f);
	m_fPitch= D3DXToRadian(Pitch * 0.1f);

	D3DXMATRIX rot;
	D3DXVECTOR3 vcZ = m_vcLook-m_vcEye;
	D3DXVECTOR3 vcX;
	D3DXMatrixRotationY(&rot, m_fYaw);
	D3DXVec3TransformCoord(&vcZ, &vcZ, &rot);
	D3DXVec3TransformCoord(&m_vcUp, &m_vcUp, &rot);

	m_vcLook = vcZ + m_vcEye;
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);


	vcZ = m_vcLook - m_vcEye;
	vcX =D3DXVECTOR3(m_mtViw._11, m_mtViw._21, m_mtViw._31);

	D3DXMatrixRotationAxis(&rot, & vcX, m_fPitch);
	D3DXVec3TransformCoord(&vcZ, &vcZ, &rot);
	D3DXVec3TransformCoord(&m_vcUp, &m_vcUp, &rot);

	m_vcLook = vcZ + m_vcEye;
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);
}
