
#pragma warning( disable : 4996)

#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "resource.h"


#include "Converter.h"





INT LoadTGAFile(BYTE** pOut, INT* pImgType, INT* pImgWidth, INT* pImgHeight, INT* pImgBit, char *filename)
{
	FILE*		fp;
	BYTE		ucharBad	=0;			// garbage data
	SHORT		sintBad		=0;			// garbage data
	long		imageSize	=0;			// size of TGA image
	INT			colorMode	=0;			// 4 for RGBA, 3 for RGB
	BYTE*		pPixel		= NULL;		// Read Pixel Data

	BYTE		ImgType		= 0;
	SHORT		ImgWidth	= 0;
	SHORT		ImgHeight	= 0;
	BYTE		ImgBit		= 0;

	// open the TGA file
	fp = fopen(filename, "rb");

	if (!fp)
		return -1;
	
	// read first two bytes of garbage
	fread(&ucharBad, sizeof(BYTE), 1, fp);
	fread(&ucharBad, sizeof(BYTE), 1, fp);

	// read in the image type
	fread(&ImgType, sizeof(BYTE), 1, fp);

	// 2: 압축하지 않은 RGB, 3: 압축하지 않은 Grayscale.
	if ((ImgType != 2) && (ImgType != 3))
	{
		fclose(fp);
		return -2;
	}

	// read 13 bytes of garbage data
	fread(&sintBad, sizeof(SHORT), 1, fp);
	fread(&sintBad, sizeof(SHORT), 1, fp);
	fread(&sintBad, sizeof(BYTE ), 1, fp);
	fread(&sintBad, sizeof(SHORT), 1, fp);
	fread(&sintBad, sizeof(SHORT), 1, fp);

	// read image dimensions
	fread(&ImgWidth, sizeof(SHORT), 1, fp);
	fread(&ImgHeight, sizeof(SHORT), 1, fp);

	// read bit depth
	fread(&ImgBit, sizeof(BYTE), 1, fp);

	// read garbage
	fread(&ucharBad, sizeof(BYTE), 1, fp);

	// colormode -> 3 = BGR, 4 = BGRA
	colorMode = ImgBit / 8;
	imageSize = ImgWidth * ImgHeight  * colorMode;

	pPixel = new BYTE[imageSize];

	fread(pPixel, sizeof(BYTE), imageSize, fp);
	fclose(fp);


	if(pImgType)	*pImgType	= ImgType;
	if(pImgWidth)	*pImgWidth	= ImgWidth;
	if(pImgHeight)	*pImgHeight	= ImgHeight;
	if(pImgBit)		*pImgBit	= 32;

	// bgr ->bgra
	if(3 == colorMode)
	{
		BYTE* pPixel4	= new BYTE[ImgWidth * ImgHeight * 4];

		for (INT i = 0; i < imageSize/3; ++i)
		{
			INT idx1 =  i*3;
			INT idx2 =  i*4;

			pPixel4[idx2+0] = pPixel[idx1 + 0];
			pPixel4[idx2+1] = pPixel[idx1 + 1];
			pPixel4[idx2+2] = pPixel[idx1 + 2];
			pPixel4[idx2+3] = 0xFF;
		}

		delete [] pPixel;

		pPixel = pPixel4;
	}

	*pOut = pPixel;

	return 0;
}




INT SaveTGAFile(char *filename, BYTE* pPixel, SHORT imageWidth, SHORT imageHeight, DWORD RealWidth, BYTE bitCount)
{
	FILE*	fp;
	BYTE	ucharBad=0;			// garbage data
	SHORT	sintBad=0;			// garbage data
	long	imageSize=0;		// size of TGA image
	INT		colorMode=0;		// 4 for RGBA, 3 for RGB

	BYTE	imageTypeCode =2;	// 압축하지 않은 RGB


	// open the TGA file
	fp = fopen(filename, "wb");

	if(!fp)
		return -1;
	
	// read first two bytes of garbage
	fwrite(&ucharBad, sizeof(BYTE), 1, fp);			//	offset: 0
	fwrite(&ucharBad, sizeof(BYTE), 1, fp);			//	offset: 1

	// writein the image type
	fwrite(&imageTypeCode, sizeof(BYTE), 1, fp);	//	offset: 2

	
	// write 13 bytes of garbage data
	fwrite(&sintBad, sizeof(SHORT), 1, fp);			//	offset: 3
	fwrite(&sintBad, sizeof(SHORT), 1, fp);			//	offset: 5
	fwrite(&sintBad, sizeof(BYTE ), 1, fp);			//	offset: 6
	fwrite(&sintBad, sizeof(SHORT), 1, fp);			//	offset: 8
	fwrite(&sintBad, sizeof(SHORT), 1, fp);			//	offset: 10

	// write image dimensions
	fwrite(&imageWidth, sizeof(SHORT), 1, fp);		//	offset: 12
	fwrite(&imageHeight, sizeof(SHORT), 1, fp);		//	offset: 14

	// write bit depth
	fwrite(&bitCount, sizeof(BYTE), 1, fp);			//	offset: 16
	// write garbage
	ucharBad = 8;
	fwrite(&ucharBad, sizeof(BYTE), 1, fp);			//	offset: 17

	// ARGB -> BGRA;
	BYTE *b, *g, *r, *a;


	for(int j=0; j<imageHeight; ++j)
	{
		for(int i=0; i<imageWidth; ++i)
		{
			BYTE* pData = pPixel + (  (imageHeight-1-j)*RealWidth + i)*4;

			a = pData + 3;
			r = pData + 2;
			g = pData + 1;
			b = pData + 0;

			if(0==*a)
			{
				*r = 255;
				*g = 255;
				*b = 255;
			}

			fwrite(b, sizeof(BYTE), 1, fp);
			fwrite(g, sizeof(BYTE), 1, fp);
			fwrite(r, sizeof(BYTE), 1, fp);
			fwrite(a, sizeof(BYTE), 1, fp);
		}
	}


	fclose(fp);

	return 0;
}




INT ConvertToTga(char* sTgaDst, char* sTgaSrc, void* pDev)
{
	HRESULT hr;
	D3DXIMAGE_INFO		pImgOrg={0};
	LPDIRECT3DTEXTURE9	pTexOrg=NULL;
	
	D3DXIMAGE_INFO		pImgDst={0};
	LPDIRECT3DTEXTURE9	pTexDst=NULL;

	LPDIRECT3DDEVICE9 pDevice = (LPDIRECT3DDEVICE9)pDev;
	
	hr  = D3DXCreateTextureFromFileEx(pDevice
		, sTgaSrc
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 1
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, D3DX_FILTER_NONE
		, D3DX_FILTER_NONE
		, 0x00FFFFFF
		, &pImgOrg
		, NULL
		, &pTexOrg);
	
	if(FAILED(hr))
		return -1;
	
	
	D3DLOCKED_RECT rc={0};
	D3DSURFACE_DESC dsc;
	memset(&dsc, 0, sizeof dsc);
	
	hr = pTexOrg->GetLevelDesc(0, &dsc);
	hr = pTexOrg->LockRect(0, &rc, NULL, 0);
	
	BYTE* pPixel = (BYTE*)rc.pBits;
	
	hr = SaveTGAFile(sTgaDst, pPixel, pImgOrg.Width, pImgOrg.Height, dsc.Width);
	hr = pTexOrg->UnlockRect(0);

	pTexOrg->Release();

	return hr;
}