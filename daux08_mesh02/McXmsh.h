// Interface for the CMcXmsh class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _McXmsh_H_
#define _McXmsh_H_


class CMcXmsh
{
public:
	struct VtxIdx
	{
		WORD	a;	WORD b;	WORD c;

		VtxIdx() : a(0), b(0), c(0){}
		VtxIdx(WORD A, WORD B, WORD C): a(A), b(B), c(C){}

		VtxIdx( const VtxIdx* pV)		{	a = pV->a; b = pV->b; c = pV->c;	}
		VtxIdx( const WORD* pV)			{	a = pV[0]; b = pV[1]; c = pV[2];	}
		VtxIdx( const VtxIdx& pV)		{	a = pV.a;	b = pV.b;	c = pV.c;	}

		operator WORD*()				{	return (WORD *) &a;					}
		operator const WORD*() const	{	return (const WORD*)&a;				}
		enum	{	FVF = D3DFMT_INDEX16,	};
	};

	struct VtxUV1
	{
		D3DXVECTOR3	p;
		FLOAT		u, v;

		VtxUV1() : p(0,0,0), u(0), v(0){}
		VtxUV1(FLOAT X,FLOAT Y,FLOAT Z
			,	FLOAT U, FLOAT V) : p(X,Y,Z), u(U), v(V){}


		enum {	FVF = (D3DFVF_XYZ| D3DFVF_TEX1),	};
	};



	struct McGeo			// Geometry
	{
		UINT	nFce;		// Face 숫자
		UINT	nVtx;		// Vertex 숫자
		VtxIdx*	pIdx;		// Face List

		McGeo()
		{
			nFce	= 0;
			nVtx	= 0;
			pIdx	= NULL;
		}

		~McGeo()
		{
			SAFE_DELETE(	pIdx	);
		}
	};

	
protected:
	LPDIRECT3DDEVICE9		m_pDev;

	LPDIRECT3DTEXTURE9	m_pTex[6];

	INT			m_nGeo;		// Number of Geometry
	McGeo*		m_pGeo;		// Geometry Pointer
	UINT		m_nVtx;		// Number of Vertex
	UINT		m_dVtx;		// Stride of Vertex
	DWORD		m_dFVF;		// FVF
	void*		m_pVtx;		// Vertex Buffer



public:
	CMcXmsh();
	virtual ~CMcXmsh();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();

protected:
	INT		CreateMeshData(LPDIRECT3DDEVICE9 pDev, ID3DXMesh** pOut);
};

#endif


