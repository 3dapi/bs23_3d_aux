// Implementation of the IMcXFile class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "IMcXFile.h"
#include "McXFile.h"
#include "McXFileIns.h"


INT McXCreate_Xfile(IMcXFile** pOut, LPDIRECT3DDEVICE9 pDev, char* sFile, void* pOriginal)
{
	IMcXFile*	pObj= NULL;

	if(sFile)
	{
		pObj = new CMcXFile;
	}

	else if(pOriginal)
	{
		pObj = new CMcXFileIns;
	}
	else
	{
		*pOut = NULL;
		return -1;
	}



	
	if(FAILED(pObj->Create(pDev, sFile, pOriginal)))
	{
		delete pObj;
		*pOut = NULL;
		return -1;
	}
	

	*pOut = pObj;
	return 0;
}
