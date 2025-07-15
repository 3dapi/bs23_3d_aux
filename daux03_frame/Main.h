// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
public:
    CMain();
    virtual HRESULT Init();
    virtual HRESULT Render();
};

extern CMain*	g_pApp;

#endif