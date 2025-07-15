//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "resource.h"
#include "Converter.h"



#define SAFE_RELEASE(p)	{ if(p){	(p)->Release(); p = NULL; } }


extern HINSTANCE			m_hInst		;
extern HWND					m_hWnd		;
extern LPDIRECT3D9			m_pD3D		;		// D3D
extern LPDIRECT3DDEVICE9	m_pd3dDevice;		// Device
extern LPD3DXSPRITE			m_pd3dSprite;		// Sprite


D3DXIMAGE_INFO		m_pImgSrc={0};
LPDIRECT3DTEXTURE9	m_pTexSrc=NULL;

D3DXIMAGE_INFO		m_pImgDst={0};
LPDIRECT3DTEXTURE9	m_pTexDst=NULL;


INT		Init();
void	Destroy();
INT		Render();
LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);




INT Init()
{
	return 0;
}

void Destroy()
{
	SAFE_RELEASE(	m_pTexSrc	);
	SAFE_RELEASE(	m_pTexDst	);
}



INT Render()
{
	if( NULL == m_pd3dDevice )
		return -1;

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,120,160), 1.0f, 0 );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	if(m_pTexSrc)
		m_pd3dSprite->Draw(m_pTexSrc, NULL, NULL, NULL, 0xFFFFFFFF);


	if(m_pTexDst)
	{
		D3DXVECTOR3	vcPos( FLOAT(m_pImgSrc.Width)+5, 0.f, 0.f);
		m_pd3dSprite->Draw(m_pTexDst, NULL, NULL, &vcPos, 0xFFFFFFFF);
	}



	m_pd3dSprite->End();



	m_pd3dDevice->EndScene();
	return m_pd3dDevice->Present( 0, 0, 0, 0);
}



LRESULT MsgProc(HWND hWnd , UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CONTEXTMENU:
		{
			static HMENU hMenu = GetMenu(hWnd);	//LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
			static HMENU hPopup= GetSubMenu(hMenu, 0);

			int	x = (short)LOWORD(lParam);
			int y = (short)HIWORD(lParam);

			TrackPopupMenu(hPopup, TPM_LEFTALIGN, x, y, 0, hWnd, NULL);
			return FALSE;
		}


		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_F_OPEN:
				{
					static BOOL bInit=TRUE;

					HRESULT	hr;
					char	sCurrentDirectory[MAX_PATH]="\0";
					GetCurrentDirectory(MAX_PATH, sCurrentDirectory);

					char			InitDir[MAX_PATH];
					GetWindowsDirectory(InitDir,MAX_PATH);

					strcpy(InitDir, sCurrentDirectory);


					BOOL			bOpen = FALSE;


					OPENFILENAME	OFN={0};
					char			lpstrFile[MAX_PATH]="";
					char			szFileTitle[MAX_PATH];

					OFN.lStructSize = sizeof(OPENFILENAME);
					OFN.hwndOwner=hWnd;
					OFN.lpstrFilter="PNG File(*.png)\0*.png\0BMP File(*.bmp)\0*.bmp\0All Files(*.*)\0*.*\0";
					OFN.lpstrFile=lpstrFile;
					OFN.nMaxFile=MAX_PATH;
					OFN.lpstrTitle="파일을 선택해 주십시요";
					OFN.lpstrFileTitle=szFileTitle;
					OFN.nMaxFileTitle=MAX_PATH;
					OFN.lpstrDefExt="png";

					if(bInit)
					{
						OFN.lpstrInitialDir=InitDir;
						bInit=FALSE;
					}

					bOpen = GetOpenFileName(&OFN);
					SetCurrentDirectory(sCurrentDirectory);

					if(FALSE == bOpen)
						return FALSE;

		//			lpstrFile,szFileTitle,OFN.nFileOffset,OFN.nFileExtension


					char	sTgaSrc[MAX_PATH]="Texture/png_Tst.png";
					char	sTgaDst[MAX_PATH]="Texture/png_Tst.tga";


					strcpy(sTgaSrc, lpstrFile);
					strcpy(sTgaDst, lpstrFile);

					char* p = strrchr(sTgaDst, '.');

					strcpy(p, ".tga");
					
					
					hr = ConvertToTga(sTgaDst, sTgaSrc, m_pd3dDevice);
					
					if(FAILED(hr))
						return FALSE;
					
					
					D3DXIMAGE_INFO		pImgSrc={0};
					LPDIRECT3DTEXTURE9	pTexSrc=NULL;
					
					D3DXIMAGE_INFO		pImgDst={0};
					LPDIRECT3DTEXTURE9	pTexDst=NULL;

					hr  = D3DXCreateTextureFromFileEx(m_pd3dDevice
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
						, &pImgSrc
						, NULL
						, &pTexSrc);
					
					if(FAILED(hr))
						return hr;
					hr  = D3DXCreateTextureFromFileEx(m_pd3dDevice
						, sTgaDst
						, D3DX_DEFAULT
						, D3DX_DEFAULT
						, 1
						, 0
						, D3DFMT_UNKNOWN
						, D3DPOOL_MANAGED
						, D3DX_FILTER_NONE
						, D3DX_FILTER_NONE
						, 0x00FFFFFF
						, &pImgDst
						, NULL
						, &pTexDst);
					
					if(FAILED(hr))
						return FALSE;
					

					memcpy(&m_pImgSrc, &pImgSrc, sizeof(D3DXIMAGE_INFO) );
					memcpy(&m_pImgDst, &pImgDst, sizeof(D3DXIMAGE_INFO) );

					SAFE_RELEASE(	m_pTexSrc	);
					SAFE_RELEASE(	m_pTexDst	);
					
					m_pTexSrc= pTexSrc;
					m_pTexDst= pTexDst;

					InvalidateRect(m_hWnd, NULL, FALSE);
					break;

				}// case ID_F_OPEN:
			}// switch(LOWORD(wParam))
		}// case WM_COMMAND:
	}//switch(uMsg)

	return FALSE;
}