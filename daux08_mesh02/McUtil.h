#ifndef _MCUTIL_H_
#define _MCUTIL_H_


#define SAFE_NEWCREATE1(p, CLASSTYPE, P1)									\
{																			\
	if(0 == (p))															\
	{																		\
		p = new CLASSTYPE;													\
																			\
		if((p))																\
		{																	\
			if(FAILED(((CLASSTYPE *)p)->Create(P1)))						\
			{																\
				delete p; p=0;												\
			}																\
		}																	\
	}																		\
}



#define SAFE_NEWINIT(p, CLASSTYPE)												\
{																				\
	if(NULL == (p))																\
	{																			\
		p = new CLASSTYPE;														\
																				\
		if(!(p))																\
		{																		\
			return -1;															\
		}																		\
																				\
		if(FAILED((p)->Init()))													\
		{																		\
			delete p;															\
			p = NULL;															\
			return -1;															\
		}																		\
	}																			\
}

	
#define SAFE_RESTORE(p)															\
{																				\
	if(p)																		\
	{																			\
		if(FAILED((p)->Restore()))												\
			return -1;															\
	}																			\
}



#define SAFE_FRAMEMOVE(p)														\
{																				\
	if(p)																		\
	{																			\
		if(FAILED(	(p)->FrameMove()))											\
			return -1;															\
	}																			\
}

#define SAFE_DESTROY(p)		{	if(p)	(p)->Destroy();			}
#define SAFE_INVALIDATE(p)	{	if(p)	(p)->Invalidate();		}
#define SAFE_RENDER(p)		{	if(p)	(p)->Render();			}


#endif