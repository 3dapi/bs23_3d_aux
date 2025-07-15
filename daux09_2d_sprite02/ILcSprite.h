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

	virtual INT	Draw(void* pTx0, const RECT* pRcDrw=NULL, const FLOAT* pvcPos=NULL, DWORD Color=0xFFFFFFFF)=0;
};


INT LcxCreateSprite(ILcSprite** pOut, void* pd3dDevice);

#endif


