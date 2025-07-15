// Interface for the IMcXFile class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _IMcXFile_H_
#define _IMcXFile_H_


#ifndef interface
#define interface struct
#endif

interface IMcXFile
{
	virtual ~IMcXFile(){};

	virtual INT		Create(LPDIRECT3DDEVICE9 pDev=NULL, char* sFileName=NULL, void* pOriginal=NULL)=0;
	virtual void	Destroy()=0;

	virtual INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual IMcXFile* GetOrigin()=0;

	virtual void	SetWorldMatrix(const D3DXMATRIX*)=0;
	virtual D3DXMATRIX* GetWorldMatrix()=0;
};


INT McXCreate_Xfile(IMcXFile** pOut, LPDIRECT3DDEVICE9 pDev, char* sFile, void* pOriginal=NULL);


#endif


