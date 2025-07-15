


#ifndef _Converter_H_
#define _Converter_H_

struct TGAHEADER
{
	BYTE	ImgType;
	SHORT	ImgW;		// Image Width
	SHORT	ImgH;		// Image Height
	BYTE	BitCnt;		// Bit Count Pixel Bit 16, 24, 32
};


INT		LoadTGAFile(BYTE** pOut, INT* pImgType, INT* pImgWidth, INT* pImgHeight, INT* pImgBit, char *filename);
INT		SaveTGAFile(char *filename, BYTE* pPixel, SHORT imageWidth, SHORT imageHeight, DWORD RealWidth, BYTE bitCount=32);
INT		ConvertToTga(char* sTgaDst, char* sTgaSrc, void* pDev);

#endif

