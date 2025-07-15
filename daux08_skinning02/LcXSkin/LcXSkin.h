// Interface for the CLcXSkin class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _LcXSkin_H_
#define _LcXSkin_H_


class CLcXSkin : public ILcMdl
{
protected:
	D3DXMATRIX		m_mtWorld		;	// ��ü�� ���� ���
	DOUBLE			m_dTimeCur		;	// ��ü�� �ð�

public:
	CLcXSkin();
	virtual ~CLcXSkin();

	virtual INT		Create(void* =0, void* =0, void* =0, void* =0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual void	Render();

	virtual const char*	const GetName()const	{	return NULL;	}
	virtual ILcMdl* GetOriginal()				{	return NULL;	}

	virtual INT		SetAttrib(char* sCmd, void* pVal);
	virtual INT		GetAttrib(char* sCmd, void* pVal);

	virtual INT		SetPosition(FLOAT* float3);
	virtual INT		GetPosition(FLOAT* float3);
};

#endif


