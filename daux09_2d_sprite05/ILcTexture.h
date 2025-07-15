// Interface for the ILcTexture class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _ILcTexture_H_
#define _ILcTexture_H_


#ifndef interface
#define interface	struct
#endif


interface ILcTexture  
{
	virtual ~ILcTexture(){};

	virtual	void*	GetTexture()=0;	// LPDIRECT3DTEXTURE9

	virtual	INT		GetImgW()=0;	// Image Width
	virtual	INT		GetImgH()=0;	// Image Height
	virtual	INT		GetImgD()=0;	// Image Depth
};

INT LcxCreateTextureFromFile(ILcTexture** pOut, void* pd3dDevice, char* sFile, DWORD dFilter=-1, DWORD dColor=0x00FFFFFF);

#endif



