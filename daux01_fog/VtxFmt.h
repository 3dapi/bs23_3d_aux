#ifndef _VTXFMT_H_
#define _VTXFMT_H_


struct VtxNDUV1
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	n;
	DWORD		d;
	FLOAT		u,v;

	VtxNDUV1()	{}
	VtxNDUV1(FLOAT X,FLOAT Y,FLOAT Z,FLOAT nX,FLOAT nY,FLOAT nZ
		,FLOAT U,FLOAT V, DWORD D=0xFFFFFFFF):p(X,Y,Z),n(nX,nY,nZ)
		,u(U),v(V), d(D){}
	enum	{FVF = ((D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)),};
};




struct VtxIdx
{
	union	{	struct	{	WORD a;	WORD b;	WORD c;	};	WORD m[3];	};

	VtxIdx()							{	a = 0;	  b = 1;		 c = 2;	}
	VtxIdx(WORD A, WORD B, WORD C)		{	a = A;    b = B;		 c = C;	}
	VtxIdx(WORD* R)						{	a = R[0]; b = R[1];	 c = R[2];	}
	operator WORD* ()					{		return (WORD *) &a;			}
	operator CONST WORD* () const		{		return (CONST WORD *) &a;	}
};

#endif _VTXFMT_H_