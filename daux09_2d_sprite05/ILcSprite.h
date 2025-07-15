// Interface for the ILcSprite class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _ILcSprite_H_
#define _ILcSprite_H_


#ifndef interface
#define interface	struct
#endif


interface ILcSprite
{
	virtual ~ILcSprite(){};

	virtual INT	Draw(void* pTx0, const RECT* pRcDrw=NULL
						, const FLOAT* pvcPos=NULL						// Position
						, DWORD Color=0xFFFFFFFF						// Duffuse color
						, const FLOAT* pvcRot=NULL, FLOAT fAngle=0.f	// Rotation and Rotation Angle(Degree)
						, const FLOAT* pvcScl=NULL						// Scaling
						)=0;

	virtual INT	DrawEx(void* pTx0										// Diffuse Texture
						, void* pTx1									// Alpha Texture
						, const RECT* pRcDrw=NULL
						, const FLOAT* pvcPos=NULL						// Position
						, DWORD Color=0xFFFFFFFF						// Duffuse color
						, const FLOAT* pvcRot=NULL, FLOAT fAngle=0.f	// Rotation and Rotation Angle(Degree)
						, const FLOAT* pvcScl=NULL						// Scaling
						)=0;
};


INT LcxCreateSprite(ILcSprite** pOut, void* pd3dDevice);

#endif


