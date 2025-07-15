#ifndef _VTXFMT_H_
#define _VTXFMT_H_

struct VtxIdx
{
	WORD a, b, c;

	VtxIdx() : a(0), b(1), c(2){}
	VtxIdx(WORD A, WORD B, WORD C) : a(A), b(B), c(C){}
};


struct VtxD
{
	D3DXVECTOR3	p;
	DWORD	d;

	VtxD() : d(0xFFFFFFFF){}
	VtxD(FLOAT X, FLOAT Y, FLOAT Z, DWORD D) : p(X,Y,Z), d(D){}
	enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE)};
};




#endif